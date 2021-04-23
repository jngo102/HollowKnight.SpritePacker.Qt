#ifndef SPRITES_FOLDER_HPP
#define SPRITES_FOLDER_HPP
#include <qdir.h>
#include "animation.hpp"

class SpritesFolder
{
public:
	SpritesFolder()
	{
		dir = new QDir();
		animations = new QList<Animation>();
	}
	SpritesFolder(QDir* _dir)
	{
		dir = _dir;
		animations = findAllAnimations();
	}
	
	QDir*					dir;
	QList<Animation>*		animations;
private:
	QList<Animation>* findAllAnimations()
	{
		QList<Animation>* animations = new QList<Animation>();
		for (QString animDirName : dir->entryList(QDir::Dirs | QDir::NoDotAndDotDot))
		{
			QDir animDir(dir->absoluteFilePath(animDirName));
			animations->append(*new Animation(&animDir));	
		}
		return animations;
	}
};

#endif
