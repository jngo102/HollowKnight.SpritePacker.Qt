#include "animation.hpp"
#include "sprite_packer.hpp"

Animation::Animation()
{
	dir = new QDir();
	spriteInfo = new SpriteInfo();
	clips = new QList<Clip>();
}

Animation::Animation(QDir* _dir)
{
	dir = _dir;
	spriteInfo = new SpriteInfo();

	//QString atlasesPath = QDir(_dir->path()).absoluteFilePath("0.Atlases");
	//QString jPath = QDir(atlasesPath).absoluteFilePath("SpriteInfo.json");
	if (QFile::exists(SpritePacker::jsonPath))
    {
        QFile jsonFile(SpritePacker::jsonPath);
        if (!jsonFile.open(QIODevice::ReadOnly))
        {
            Logger::write("Could not load " + SpritePacker::jsonPath);
            return;
        }

        QByteArray jsonDocument = jsonFile.readAll();
        QJsonDocument loadedJson(QJsonDocument::fromJson(jsonDocument));
        QJsonObject json(loadedJson.object());

        for (int i = 0; i < json.value("sid").toArray().size(); i++)
		{
			spriteInfo->add(
                json.value("sid").toArray().at(i).toInt(),
                json.value("sx").toArray().at(i).toInt(),
                json.value("sy").toArray().at(i).toInt(),
                json.value("sxr").toArray().at(i).toInt(),
                json.value("syr").toArray().at(i).toInt(),
                json.value("swidth").toArray().at(i).toInt(),
                json.value("sheight").toArray().at(i).toInt(),
                json.value("scollectionname").toArray().at(i).toString(),
                json.value("spath").toArray().at(i).toString(),
                SpritePacker::jsonVersion == "1.2.2.1" ?
                        (json.value("sfilpped").toArray().at(i).toInt() == 1 ? true : false) :
                        json.value("sfilpped").toArray().at(i).toBool());
		}
	}
	clips = findAllClips(_dir, spriteInfo);
}

QList<Clip>* Animation::findAllClips(QDir* _dir, SpriteInfo* _spriteInfo)
{
	QList<Clip>* clips = new QList<Clip>();
	for (QString clipDirName : _dir->entryList(QDir::Dirs | QDir::NoDotAndDotDot))
	{
		QDir clipDir(_dir->absoluteFilePath(clipDirName));
		if (!clipDir.dirName().startsWith("0."))
		{
			clips->append(*new Clip(&clipDir, _spriteInfo));
		}
		else
		{
			for (QString atlasName : clipDir.entryList(QStringList() << "*.png" << "*.PNG", QDir::Files))
			{
				QFile atlas(clipDir.absoluteFilePath(atlasName));
				if (!atlas.fileName().startsWith("Gen-"))
				{
					if (Collection::getCollectionByName(atlas.fileName()) == nullptr)
					{
						QString collectionName = atlas.fileName().split("/").last();
						Collection newCollection(collectionName, atlas.fileName());
						Collection::collections->append(newCollection);
					}
				}
				else if (atlas.fileName().startsWith("Gen-"))
				{
					if (Collection::getGenCollectionByName(atlas.fileName()) == nullptr)
					{
						QString collectionName = atlas.fileName().split("/").last();
						Collection newCollection(collectionName, atlas.fileName());
						Collection::genCollections->append(newCollection);
					}
				}
			}
		}
	}

	return clips;
}
