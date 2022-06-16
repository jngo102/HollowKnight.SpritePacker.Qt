#include "stdafx.hpp"
#include "sprite_animation.hpp"
#include "logger.hpp"

SpriteAnimation::SpriteAnimation(QObject* parent) : QObject(parent)
{
	currentFrame = new QPixmap();
	fps = 12.0f;
	frameNumber = 0;
	frames = new QList<QPixmap>();
	loopStart = 0;

	frameTimer = new QTimer();
	connect(frameTimer, &QTimer::timeout, this, &SpriteAnimation::advanceFrame);
}

void SpriteAnimation::init(float _fps, QList<QPixmap>* _frames, int _loopStart)
{
	*currentFrame = _frames->at(0);
	fps = _fps;
	frameNumber = 0;
	frames->clear();
	frames = _frames;
	loopStart = _loopStart < frames->length() ? _loopStart : 0;
	changeFrame(frameNumber);
}

SpriteAnimation::~SpriteAnimation()
{
	frameTimer->stop();
}

bool SpriteAnimation::isEmpty()
{
	return frames->empty();
}

void SpriteAnimation::play(int fromFrame)
{
    changeFrame(fromFrame);
	frameTimer->start(1000 / fps);
}

void SpriteAnimation::stop(int atFrame)
{
	changeFrame(atFrame);
	frameTimer->stop();
}

void SpriteAnimation::advanceFrame()
{
	changeFrame(frameNumber + 1);
}

void SpriteAnimation::changeFrame(int _frameNumber)
{
	frameNumber = _frameNumber;
	if (frameNumber >= frames->length())
	{
		frameNumber = loopStart;
	}
	*currentFrame = frames->at(frameNumber);
	emit frameChanged(currentFrame);
	emit frameNumberChanged(frameNumber);
}
