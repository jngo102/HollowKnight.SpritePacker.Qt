#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <qdir.h>
#include "sprite_info.hpp"
#include "clip.hpp"

struct Animation
{
	Animation();
	Animation(QDir* _dir);
	
	QDir*			dir;
	SpriteInfo*		spriteInfo;
    QList<Clip>*	clips;
private:
    QList<Clip>* findAllClips(QDir* _dir, SpriteInfo* _spriteInfo);
};

#endif
