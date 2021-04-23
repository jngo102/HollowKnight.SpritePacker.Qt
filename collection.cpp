#include "collection.hpp"
#include "logger.hpp"
#include "stdafx.hpp"

QList<Collection>* Collection::collections = new QList<Collection>();
QList<Collection>* Collection::genCollections = new QList<Collection>();

Collection::Collection()
{
	name = "Collection";
	file = new QFile();
	frames = new QList<Frame>();
}

Collection::Collection(QString _name, QString infoPath)
{
	name = _name;
	file = new QFile(infoPath);
	frames = new QList<Frame>();
}

Collection* Collection::getCollectionByName(QString _name)
{
	for (Collection collection : *collections)
	{
		if (collection.name == _name)
		{
			return &collection;
		}
	}
	return nullptr;
}

Collection* Collection::getGenCollectionByName(QString _name)
{
	for (Collection collection : *genCollections)
	{
		if (collection.name == _name)
		{
            return &collection;
		}
	}
	return nullptr;
}

bool frameLessThan(const Frame& frameA, const Frame& frameB)
{
	return frameA.file->fileName().section("-", 2) < frameB.file->fileName().section("-", 2);
}

void Collection::sortFrames()
{
	std::sort(frames->begin(), frames->end(), frameLessThan);
}

Frame::Frame()
{
	file = new QFile();
	spriteInfo = new SpriteInfo();
	sprite = new Sprite();
	collection = new Collection();
	collection->frames->push_back(*this);
}

Frame::Frame(const QString filePath, SpriteInfo* _spriteInfo)
{
	file = new QFile(filePath);
	spriteInfo = _spriteInfo;
	sprite = new Sprite();
	*sprite = getSprite(file, _spriteInfo);
	if (sprite->id == 0) return;
	collection = Collection::getCollectionByName(sprite->collectionName + ".png");
	collection->frames->push_back(*this);
}

Sprite Frame::getSprite(QFile* _file, SpriteInfo* _spriteInfo)
{
	for (int i = 0; i < _spriteInfo->length(); i++)
	{
		if (_file->fileName().endsWith(_spriteInfo->at(i).path))
		{
			Sprite sprite = _spriteInfo->at(i);
			return sprite;
		}
	}

	QStringList splitPath = _file->fileName().replace("\\", "/").split("/");
	QString clipName = splitPath.at(splitPath.length() - 2);
	QString animFrameNumber = splitPath.last().section("-", 1);
	if (clipName == "NA Charge Effect")
	{
		QDir naChargeClipDir(QDir(QDir(ROOT_SPRITES_DIRECTORY).absoluteFilePath("Knight/034.NA Charge")));
		QStringList naChargeFileNames = naChargeClipDir.entryList();
		for (QString naChargeFileName : naChargeFileNames)
		{
			if (naChargeFileName.section("-", 1) == animFrameNumber)
			{
				QFile naChargeFile(naChargeClipDir.absoluteFilePath(naChargeFileName));
				return getSprite(&naChargeFile, _spriteInfo);
			}
		}
	}

	Logger::write("Failed to get a sprite.");
	return *new Sprite();
}
