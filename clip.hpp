#ifndef CLIP_HPP
#define CLIP_HPP

#include "stdafx.hpp"
#include "sprite_info.hpp"
#include "collection.hpp"
#include "logger.hpp"
#include <fstream>
#include <qdir.h>

struct Clip
{
public:
	Clip()
	{
		dir = new QDir();
		spriteInfo = new SpriteInfo();
        frames = new QList<Frame>();
		fps = 12.0f;
		loopStart = 0;
	}
	Clip(QDir* _dir, SpriteInfo* _spriteInfo)
	{
		dir = _dir;
		spriteInfo = _spriteInfo;
		frames = findAllFrames(_dir, _spriteInfo);
		QPair<float, int> clipData = getClipData(_dir);
		fps = clipData.first;
		loopStart = clipData.second;
	}
	
	QDir* dir;
	SpriteInfo* spriteInfo;
    QList<Frame>* frames;
	float fps;
	int loopStart;
private:
    QList<Frame>* findAllFrames(QDir* _dir, SpriteInfo* _spriteInfo)
	{
        QList<Frame>* frames = new QList<Frame>();
		for (QString frameName : _dir->entryList(QStringList() << "*.png" << "*.PNG", QDir::Files))
		{
			QString framePath(_dir->absoluteFilePath(frameName));
			if (frameName.contains("-") && frameName.length() == 14 && !frameName.contains("position") && !frameName.contains("backup"))
			{
				Frame newFrame(framePath, _spriteInfo);
				frames->append(newFrame);
			}
		}

		return frames;
	}
	QPair<float, int> getClipData(QDir* _dir)
	{
        QString jsonPath(_dir->absoluteFilePath("AnimInfo.json"));
        QFile jsonFile(jsonPath);
        if (!jsonFile.open(QIODevice::ReadOnly))
        {
            Logger::write("Could not open " + jsonPath);
            return QPair<float, int>(0.0f, 0);
        }

        QByteArray jsonDocument = jsonFile.readAll();
        QJsonDocument loadedJson(QJsonDocument::fromJson(jsonDocument));
        QJsonObject json(loadedJson.object());

        float fps = json.value("fps").toDouble();
        int loopStart = json.value("loopStart").toInt();
		return qMakePair(fps, loopStart);
	}
};

#endif
