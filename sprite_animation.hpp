#ifndef SPRITE_ANIMATION_HPP
#define SPRITE_ANIMATION_HPP

#include "stdafx.hpp"

class SpriteAnimation : public QObject
{
	Q_OBJECT
public:
	SpriteAnimation(QObject* parent = nullptr);
	~SpriteAnimation();
	void				init(float _fps, QList<QPixmap>* _frames, int _loopStart = 0);
	bool				isEmpty();
	void				play(int fromFrame = 0);
	void				stop(int atFrame = 0);
signals:
	void				frameChanged(QPixmap* frame);
	void				frameNumberChanged(int frameNumber);
private slots:
	void				advanceFrame();
private:
	void				changeFrame(int _frameNumber);
	
	QPixmap*			currentFrame;
	float				fps;
	int					frameNumber;
	QList<QPixmap>*	frames;
	QTimer*				frameTimer;
	int					loopStart;
};

#endif