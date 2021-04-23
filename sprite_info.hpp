#ifndef SPRITE_INFO_HPP
#define SPRITE_INFO_HPP

#include <QObject>
#include "sprite.hpp"

struct SpriteInfo
{
	SpriteInfo()
	{
		sid = new QList<int>();
		sx = new QList<int>();
		sy = new QList<int>();
		sxr = new QList<int>();
		syr = new QList<int>();
		sWidth = new QList<int>();
		sHeight = new QList<int>();

		sCollectionName = new QList<QString>();
		sPath = new QList<QString>();
		sFlipped = new QList<bool>();
	}

	int length() { return sid->count(); }
	Sprite at(int i)
	{
		Sprite sprite(
			sid->at(i),
			sx->at(i),
			sy->at(i),
			sxr->at(i),
			syr->at(i),
			sWidth->at(i),
			sHeight->at(i),
			sCollectionName->at(i),
			sPath->at(i),
			sFlipped->at(i));
		return sprite;
	}
	void add(
		int _sid,
		int _sx,
		int _sy,
		int _sxr,
		int _syr,
		int _sWidth,
		int _sHeight,
		QString _sCollectionName,
		QString _sPath,
		bool _sFlipped)
	{
		sid->append(_sid);
		sx->append(_sx);
		sy->append(_sy);
		sxr->append(_sxr);
		syr->append(_syr);
		sWidth->append(_sWidth);
		sHeight->append(_sHeight);
		sCollectionName->append(_sCollectionName);
		sPath->append(_sPath);
		sFlipped->append(_sFlipped);
	}

	QList<int>* sid;
	QList<int>* sx;
	QList<int>* sy;
	QList<int>* sxr;
	QList<int>* syr;
	QList<int>* sWidth;
	QList<int>* sHeight;

	QList<QString>* sCollectionName;
	QList<QString>* sPath;

	QList<bool>* sFlipped;
};

#endif