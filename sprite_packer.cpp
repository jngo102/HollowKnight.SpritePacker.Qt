#include "sprite_packer.hpp"
#include "stdafx.hpp"
#include "packer.hpp"
#include "logger.hpp"

QString SpritePacker::jsonPath;
QString SpritePacker::jsonVersion;

bool checkSavePath()
{
    return QDir(ROOT_SPRITES_DIRECTORY).exists();
}

QString refreshList(const QString chosen, QListWidget* listWidget, QList<QString>* list)
{
    listWidget->clear();
    if (checkSavePath())
    {
        for (QString item : *list)
        {
            listWidget->addItem(item);
        }
    }

    return chosen;
}

SpritePacker::SpritePacker(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    Logger::open(QStandardPaths::locate(QStandardPaths::DownloadLocation, nullptr, QStandardPaths::LocateDirectory) + "/SpritePacker.log");
    Logger::write("Root sprites dir: " + ROOT_SPRITES_DIRECTORY);
    currentAnimation = new SpriteAnimation(this);
    connect(currentAnimation, &SpriteAnimation::frameChanged, this, &SpritePacker::updatePixmap);
    connect(currentAnimation, &SpriteAnimation::frameNumberChanged, this, &SpritePacker::updateFrameList);

    ui.animationsList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui.clipsList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui.framesList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    connect(ui.animationsList, &QListWidget::itemClicked, this, &SpritePacker::loadClips);
    connect(ui.clipsList, &QListWidget::itemClicked, this, &SpritePacker::loadFrames);
    connect(ui.framesList, &QListWidget::itemClicked, this, &SpritePacker::selectFrame);

    backups = new QList<Frame>();

    loadAnimations();

    QThreadPool::globalInstance()->setMaxThreadCount(4);
}

SpritePacker::~SpritePacker()
{
    Logger::close();
}

void SpritePacker::loadAnimations()
{
    QDir rootSpritesDirectory(ROOT_SPRITES_DIRECTORY);
    ui.animationsList->clear();
    for (QString animation : rootSpritesDirectory.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
    {
        if (animation.toLower().contains(ui.animationsFilter->text().toLower()))
        {
            ui.animationsList->addItem(animation);
        }
    }
    if (ui.animationsList->count() <= 0) return;
    ui.animationsList->item(0)->setSelected(true);
    loadClips(ui.animationsList->item(0));
}

void SpritePacker::loadClips(QListWidgetItem* item)
{
    QDir animationDir(QDir(ROOT_SPRITES_DIRECTORY).absoluteFilePath(item->text()));
    ui.clipsList->clear();
    for (QString clip : animationDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        if (clip.toLower().contains(ui.clipsFilter->text().toLower()) && !clip.startsWith("0."))
        {
            ui.clipsList->addItem(clip);
        }
    }
    if (ui.clipsList->count() <= 0) return;
    ui.clipsList->item(0)->setSelected(true);
    loadFrames(ui.clipsList->item(0));
}

void SpritePacker::loadFrames(QListWidgetItem* item)
{
    QDir animationDir(QDir(ROOT_SPRITES_DIRECTORY).absoluteFilePath(ui.animationsList->selectedItems()[0]->text()));
    QDir clipDir(animationDir.absoluteFilePath(item->text()));
    ui.framesList->clear();
    QList<QPixmap>* pixmaps = new QList<QPixmap>();
    for (QString frame : clipDir.entryList(QStringList() << "*.png" << "*.PNG", QDir::Files))
    {
        if (frame.toLower().contains(ui.framesFilter->text().toLower()))
        {
            ui.framesList->addItem(frame);
            QString framePath = clipDir.absoluteFilePath(frame);
            QPixmap pixmap(framePath);
            if (pixmap.width() >= 384 || pixmap.height() >= 384)
            {
                pixmap = pixmap.scaled(384, 384, Qt::KeepAspectRatio);
            }
            pixmaps->push_back(pixmap);
        }
    }
    if (pixmaps->count() <= 0) return;

    QString jsonPath(clipDir.absoluteFilePath("AnimInfo.json"));
    QFile jsonFile(jsonPath);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        Logger::write("Could not load JSON at " + jsonPath);
        return;
    }

    QByteArray jsonDocument = jsonFile.readAll();
    QJsonDocument loadedJson(QJsonDocument::fromJson(jsonDocument));
    QJsonObject json(loadedJson.object());
    float fps = json.value("animInfo").toObject().value("fps").toDouble();
    int loopStart = json.value("animInfo").toObject().value("loopStart").toInt();

    currentAnimation->init(fps, pixmaps, loopStart);
    currentAnimation->play();
}

void SpritePacker::selectFrame(QListWidgetItem*)
{
    currentAnimation->stop(ui.framesList->currentIndex().row());
}

void SpritePacker::updateFrameList(int frameNumber)
{
    if (ui.framesList->item(frameNumber) != nullptr)
    {
        ui.framesList->item(frameNumber)->setSelected(true);
    }
}

void SpritePacker::updatePixmap(QPixmap* pixmap)
{
    ui.spritesView->setPixmap(*pixmap);
}

void SpritePacker::on_animationsFilter_textChanged(const QString&)
{
    loadAnimations();
}

void SpritePacker::on_clipsFilter_textChanged(const QString&)
{
    if (ui.animationsList->count() <= 0) return;
    loadClips(ui.animationsList->item(0));
}

void SpritePacker::on_framesFilter_textChanged(const QString&)
{
    if (ui.clipsList->count() <= 0) return;
    loadFrames(ui.clipsList->item(0));
}

void SpritePacker::on_jsonVersionComboBox_activated(const QString& _jsonVersion)
{
    jsonVersion = _jsonVersion;
}

void SpritePacker::on_packButton_clicked()
{
    if (jsonVersion == "1.2.2.1")
    {
        QString jsonFile = QFileDialog::getOpenFileName(nullptr, "Select SpriteInfo.json", ROOT_SPRITES_DIRECTORY, "*.json");
        jsonPath = QFileInfo(jsonFile).absoluteFilePath();
    }
    else
    {
        jsonPath = QDir(ROOT_SPRITES_DIRECTORY).absoluteFilePath("Knight/0.Atlases/SpriteInfo.json");
    }

    spritesFolder = new SpritesFolder(new QDir(ROOT_SPRITES_DIRECTORY));

    for (Collection collection : *Collection::collections)
    {
        Packer* packer = new Packer(collection, this);
        packer->setAutoDelete(true);
        QThreadPool::globalInstance()->start(packer);
    }
}

void SpritePacker::on_refreshButton_clicked()
{
    if (checkSavePath())
    {
        Collection::collections->clear();
        Collection::genCollections->clear();
        spritesFolder = new SpritesFolder(new QDir(ROOT_SPRITES_DIRECTORY));
    }
}

QString SpritePacker::getAnimation()
{
    if (ui.animationsList->currentItem() != nullptr)
    {
        animation = ui.animationsList->currentItem()->text();
    }
    else if (ui.animationsList->count() > 0)
    {
        animation = ui.animationsList->item(0)->text();
    }
    else
    {
        animation = "";
    }
    return animation;
}

void SpritePacker::setAnimation(QString& _animation)
{
    int index = ui.animationsList->indexFromItem(ui.animationsList->findItems(_animation, Qt::MatchContains).at(0)).row();
    if (index >= 0)
    {
        ui.animationsList->setCurrentItem(ui.animationsList->itemAt(0, index));
        animation = _animation;
    }
}

QString SpritePacker::getClip()
{
    if (ui.clipsList->currentItem() != nullptr)
    {
        clip = ui.clipsList->currentItem()->text();
    }
    else if (ui.clipsList->count() > 0)
    {
        clip = ui.clipsList->item(0)->text();
    }
    else
    {
        clip = "";
    }
    return clip;
}

void SpritePacker::setClip(QString& _clip)
{
    int index = ui.clipsList->indexFromItem(ui.clipsList->findItems(_clip, Qt::MatchContains).at(0)).row();
    if (index >= 0)
    {
        ui.clipsList->setCurrentItem(ui.clipsList->itemAt(0, index));
        clip = _clip;
    }
}

QString SpritePacker::getFrame()
{
    if (ui.framesList->currentItem() != nullptr)
    {
        frame = ui.framesList->currentItem()->text();
    }
    else if (ui.framesList->count() > 0)
    {
        frame = ui.framesList->item(0)->text();
    }
    else
    {
        frame = "";
    }
    return frame;
}

void SpritePacker::setFrame(QString& _frame)
{
    int index = ui.framesList->indexFromItem(ui.framesList->findItems(_frame, Qt::MatchContains).at(0)).row();
    if (index >= 0)
    {
        ui.framesList->setCurrentItem(ui.framesList->itemAt(0, index));
        frame = _frame;
    }
}

QString SpritePacker::getOriginalAtlas()
{
    if (ui.originalAtlasList->currentItem() != nullptr)
    {
        originalAtlas = ui.originalAtlasList->currentItem()->text();
    }
    else if (ui.originalAtlasList->count() > 0)
    {
        originalAtlas = ui.originalAtlasList->item(0)->text();
    }
    else
    {
        originalAtlas = "";
    }
    return originalAtlas;
}

void SpritePacker::setOriginalAtlas(QString& _originalAtlas)
{
    int index = ui.originalAtlasList->indexFromItem(ui.originalAtlasList->findItems(_originalAtlas, Qt::MatchContains).at(0)).row();
    if (index >= 0)
    {
        ui.originalAtlasList->setCurrentItem(ui.originalAtlasList->itemAt(0, index));
        originalAtlas = _originalAtlas;
    }
}

QString SpritePacker::getGeneratedAtlas()
{
    if (ui.generatedAtlasList->currentItem() != nullptr)
    {
        generatedAtlas= ui.generatedAtlasList->currentItem()->text();
    }
    else if (ui.generatedAtlasList->count() > 0)
    {
        generatedAtlas = ui.generatedAtlasList->item(0)->text();
    }
    else
    {
        generatedAtlas = "";
    }
    return generatedAtlas;
}

void SpritePacker::setGeneratedAtlas(QString& _generatedAtlas)
{
    int index = ui.generatedAtlasList->indexFromItem(ui.generatedAtlasList->findItems(_generatedAtlas, Qt::MatchContains).at(0)).row();
    if (index >= 0)
    {
        ui.generatedAtlasList->setCurrentItem(ui.generatedAtlasList->itemAt(0, index));
        generatedAtlas = _generatedAtlas;
    }
}

QString SpritePacker::getBackup()
{
    if (ui.backupFilesList->currentItem() != nullptr)
    {
        backup = ui.backupFilesList->currentItem()->text();
    }
    else if (ui.backupFilesList->count() > 0)
    {
        backup = ui.backupFilesList->item(0)->text();
    }
    else
    {
        backup = "";
    }
    return backup;
}

void SpritePacker::setBackup(QString& _backup)
{
    int index = ui.backupFilesList->indexFromItem(ui.backupFilesList->findItems(_backup, Qt::MatchContains).at(0)).row();
    if (index >= 0)
    {
        ui.backupFilesList->setCurrentItem(ui.backupFilesList->itemAt(0, index));
        backup = _backup;
    }
}

InspectMode SpritePacker::getInspectMode()
{
    return inspectMode;
}

void SpritePacker::setInspectMode(InspectMode& _inspectMode)
{

}

void SpritePacker::refreshAnimationsList()
{
    QList<QString>* animations = new QList<QString>();
    for (Animation animation : *spritesFolder->animations)
    {
        animations->append(animation.dir->dirName());
    }
    QString _animation = refreshList(animation, ui.animationsList, animations);
    setAnimation(_animation);
}

void SpritePacker::refreshClipsList()
{
    Animation firstAnimation;
    for (Animation _animation : *spritesFolder->animations)
    {
        if (_animation.dir->dirName() == animation)
        {
            firstAnimation = _animation;
        }
    }

    QList<QString>* clips = new QList<QString>();
    for (Clip clip : *firstAnimation.clips)
    {
        clips->append(clip.dir->dirName());
    }
    QString _clip = refreshList(clip, ui.clipsList, clips);
    setClip(_clip);
}

void SpritePacker::refreshFramesList()
{
    if (getInspectMode() == InspectMode::animation || getInspectMode() == InspectMode::animFrame)
    {
        Animation firstAnimation;
        for (Animation _animation : *spritesFolder->animations)
        {
            if (_animation.dir->dirName() == animation)
            {
                firstAnimation = _animation;
            }
        }
        Clip firstClip;
        for (Clip _clip : *firstAnimation.clips)
        {
            if (_clip.dir->dirName() == clip)
            {
                firstClip = _clip;
            }
        }
        QList<QString>* frames = new QList<QString>();
        for (Frame frame : *firstClip.frames)
        {
            frames->append(frame.file->fileName());
        }

        QString _frame = refreshList(frame, ui.framesList, frames);
        setFrame(_frame);
    }
    if (getInspectMode() == InspectMode::collection || getInspectMode() == InspectMode::collectionFrame || getInspectMode() == InspectMode::backup)
    {
        Collection collection;
        for (Collection _collection : *Collection::collections)
        {
            if (_collection.file->fileName().contains(animation) && _collection.name == originalAtlas)
            {
                collection = _collection;
            }
        }
        collection.sortFrames();
        QList<QString>* frames = new QList<QString>();
        for (Frame _frame : *collection.frames)
        {
            frames->append(_frame.file->fileName());
        }
        QString _frame = refreshList(frame, ui.framesList, frames);
        setFrame(_frame);
    }
}

void SpritePacker::refreshOriginalAtlasList()
{
    QList<QString>* collections = new QList<QString>();
    for (Collection _collection : *Collection::collections)
    {
        if (_collection.file->fileName().contains(animation) && _collection.name == originalAtlas)
        {
            collections->append(_collection.name);
        }
    }
    QString _originalAtlas = refreshList(originalAtlas, ui.originalAtlasList, collections);
    setOriginalAtlas(_originalAtlas);
}

void SpritePacker::refreshGeneratedAtlasList()
{
    QList<QString>* genCollections = new QList<QString>();
    for (Collection _genCollection : *Collection::genCollections)
    {
        if (_genCollection.file->fileName().contains(animation) && _genCollection.name == generatedAtlas)
        {
            genCollections->append(_genCollection.name);
        }
    }
    QString _generatedAtlas = refreshList(generatedAtlas, ui.generatedAtlasList, genCollections);
    setGeneratedAtlas(_generatedAtlas);
}

void SpritePacker::refreshBackupFilesList()
{
    ui.backupFilesList->clear();
    if (checkSavePath())
    {

    }
    else
    {

    }
}

void SpritePacker::refreshChangedFilesList()
{
    ui.changedFilesList->clear();
    if (checkSavePath())
    {
        QDir rootSpritesDirectory(ROOT_SPRITES_DIRECTORY);
        QDir animationDir(QDir(rootSpritesDirectory.absoluteFilePath(animation)));

    }
}
