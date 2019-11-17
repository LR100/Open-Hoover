#include "ImageRaw.h"

ImageRaw::ImageRaw()
{
}

ImageRaw::~ImageRaw()
{
}

void ImageRaw::Init(const unsigned int & width, const unsigned int & height, const Format & format)
{
	InitByFormat(format);
	this->_width = width;
	this->_height = height;
	this->_sizeLine = (_bpp * _width);
	this->_size = (_width * _height * _bpp);
	this->_data = new unsigned char[_size];
}

const unsigned int& ImageRaw::GetWidth() const
{
	return (_width);
}

const unsigned int& ImageRaw::GetHeight() const
{
	return (_height);
}

void ImageRaw::InitByFormat(const Format & format)
{
	if (format == RGBA || format == ABGR || format == BGRA)
		_bpp = 4;
	else if (format == RGB)
		_bpp = 3;
}
