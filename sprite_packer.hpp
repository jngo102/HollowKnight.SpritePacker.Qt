#ifndef SPRITE_PACKER_HPP
#define SPRITE_PACKER_HPP

#include "stdafx.hpp"
#include <QtWidgets/QMainWindow>
#include "ui_sprite_packer.h"
#include "sprite_animation.hpp"
#include "sprites_folder.hpp"

enum InspectMode
{
    animation,
    collection,
    genCollection,
    animFrame,
    collectionFrame,
    backup,
};

class SpritePacker : public QMainWindow
{
    Q_OBJECT

public:
    SpritePacker(QWidget* parent = Q_NULLPTR);
    ~SpritePacker();

    static QString                  jsonPath;
    static QString                  jsonVersion;
private slots:
    void                            loadClips(QListWidgetItem* item);
    void                            loadFrames(QListWidgetItem* item);
    void                            selectFrame(QListWidgetItem*);
    void                            updateFrameList(int frameNumber);
    void                            updatePixmap(QPixmap* pixmap);

    void                            on_animationsFilter_textChanged(const QString &arg1);
    void                            on_clipsFilter_textChanged(const QString &arg1);
    void                            on_framesFilter_textChanged(const QString &arg1);
    void                            on_jsonVersionComboBox_activated(const QString &arg1);
    void                            on_packButton_clicked();
    void                            on_refreshButton_clicked();
private:
    QString                         getAnimation();
    QString                         getClip();
    QString                         getFrame();
    QString                         getOriginalAtlas();
    QString                         getGeneratedAtlas();
    QString                         getBackup();
    void                            setAnimation(QString& _animation);
    void                            setClip(QString& _clip);
    void                            setFrame(QString& _frame);
    void                            setOriginalAtlas(QString& _originalAtlas);
    void                            setGeneratedAtlas(QString& _generatedAtlas);
    void                            setBackup(QString& _backup);
    InspectMode                     getInspectMode();
    void                            setInspectMode(InspectMode& _inspectMode);

    void                            loadAnimations();
    void                            refreshAnimationsList();
    void                            refreshClipsList();
    void                            refreshFramesList();
    void                            refreshOriginalAtlasList();
    void                            refreshGeneratedAtlasList();
    void                            refreshBackupFilesList();
    void                            refreshChangedFilesList();

    QString                         animation;
    QString                         clip;
    QString                         frame;
    QString                         originalAtlas;
    QString                         generatedAtlas;
    QString                         backup;
    QList<Frame>*                   backups;
    InspectMode                     inspectMode;
    Ui::SpritePackerClass           ui;
    SpriteAnimation*                currentAnimation;
    SpritesFolder*                  spritesFolder;
};

#endif
