#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include <QFileDialog>
#include "sprite_info.hpp"

struct Frame;

struct Collection
{
	Collection();
	Collection(QString _name, QString infoPath);
	static Collection* getCollectionByName(QString _name);
	static Collection* getGenCollectionByName(QString _name);
	void sortFrames();

	QString						name;
	QFile*						file;
	QList<Frame>*				frames;

	static QList<Collection>* collections;
	static QList<Collection>* genCollections;
};

struct Frame
{
public:
	Frame();
	Frame(QString filePath, SpriteInfo* _spriteInfo);

	QFile*						file;
	SpriteInfo*					spriteInfo;
	Sprite*						sprite;
	Collection*					collection;
private:
	Sprite						getSprite(QFile* _file, SpriteInfo *_spriteInfo);
};

#endif
