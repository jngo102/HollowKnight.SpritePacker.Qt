#ifndef SPRITE_HPP
#define SPRITE_HPP

struct Sprite
{
	Sprite()
	{
		id = x = y = xr = yr = width = height = 0;
		collectionName = path = "";
		flipped = false;
	}
	Sprite(
		int _id,
		int _x,
		int _y,
		int _xr,
		int _yr,
		int _width,
		int _height,
		QString _collectionName,
		QString _path,
		bool _flipped)
	{
		id = _id;
		x = _x;
		y = _y;
		xr = _xr;
		yr = _yr;
		width = _width;
		height = _height;
		collectionName = _collectionName;
		path = _path;
		flipped = _flipped;
	}

	int id;
	int x;
	int y;
	int xr;
	int yr;
	int width;
	int height;

	QString collectionName;
	QString path;
	bool flipped;
};

#endif