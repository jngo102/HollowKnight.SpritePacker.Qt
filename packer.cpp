#include "stdafx.hpp"
#include "packer.hpp"
#include "sprite_packer.hpp"
#include "logger.hpp"
#include <chrono>

Packer::Packer(Collection _collection, QObject* parent) : QObject(parent)
{
    collection = _collection;
}

void Packer::pack(Collection collection)
{
	auto start = std::chrono::high_resolution_clock::now();
	collection.sortFrames();
    QImage genAtlas(collection.file ->fileName());
	for (const Frame& frame : *collection.frames)
	{
		QImage image(frame.file->fileName());
        for (int i = 0; i < image.width(); i++)
		{
            for (int j = 0; j < image.height(); j++)
			{
                int x = frame.sprite->flipped ? frame.sprite->x + j - frame.sprite->yr : frame.sprite->x + i - frame.sprite->xr;
                int y = frame.sprite->flipped ? genAtlas.height() - (frame.sprite->y + i) - 1 + frame.sprite->xr : genAtlas.height() - (frame.sprite->y + j) - 1 + frame.sprite->yr;
				if ((i >= frame.sprite->xr && i < frame.sprite->xr + frame.sprite->width && j >= frame.sprite->yr && j < frame.sprite->yr + frame.sprite->height) && (x >= 0 && x < genAtlas.width() && y >= 0 && y < genAtlas.height()))
				{
                    genAtlas.setPixelColor(x, y, image.pixelColor(i, image.height() - j - 1));
				}
			}
        }
	}
	
	QFileInfo saveInfo(QFileInfo(collection.file->fileName()));
	QString saveFile = "Gen-" + saveInfo.fileName();
	QString savePath = QDir(saveInfo.absolutePath()).absoluteFilePath(saveFile);
    genAtlas.save(savePath, "PNG", 100);
    auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	Logger::write("Time to pack " + collection.name + ": " + QString::number(elapsed.count()) + " ms");
}

void Packer::run()
{
    pack(collection);
}
