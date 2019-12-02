#include "DImage.h"


//															DEFAULT,				RGBA,					ARGB,
//	ABGR, BGRA, 			RGB (5)										
const DImage::SetAPix DImage::_setAPix[COLOR_FORMAT_COUNT] = { &DImage::SetAPixelRGBA , &DImage::SetAPixelRGBA, &DImage::SetAPixelARGB,\
	NULL, NULL, &DImage::SetAPixelRGB };
const DImage::SetBPix DImage::_setBPix[COLOR_FORMAT_COUNT] = { &DImage::SetBPixelRGBA , &DImage::SetBPixelRGBA, &DImage::SetBPixelARGB,\
	NULL, NULL, &DImage::SetBPixelRGB };
const DImage::GetAPix DImage::_getAPix[COLOR_FORMAT_COUNT] = { &DImage::GetAPixelRGBA , &DImage::GetAPixelRGBA, &DImage::GetAPixelARGB,\
	NULL, &DImage::GetAPixelBGRA, &DImage::GetAPixelRGB };
const DImage::GetBPix DImage::_getBPix[COLOR_FORMAT_COUNT]
= { &DImage::GetBPixelRGBA , &DImage::GetBPixelRGBA, &DImage::GetBPixelARGB,\
	NULL, &DImage::GetBPixelBGRA, &DImage::GetBPixelRGB };


DImage::DImage()
{
	_format = ColorFormat::DEFAULT;
	_size = 0;
	_width = 0;
	_height = 0;
	_data = 0;
	_bpp = 0;
	_sizeLine = 0;
}

DImage::~DImage()
{
}

void DImage::Clear()
{
	if (_data)
		memset(_data, 0, _size);
}

void DImage::SetLine(const unsigned int& x, const unsigned int& y, unsigned char* line, const unsigned int& lineSize)
{
	unsigned char* p = _data;
	unsigned int	pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	memcpy(p, line, lineSize);
}

// COLOR Received here is ALWAYS in RGBA Mode (Always !)
void DImage::SetPixel(const unsigned int& x, const unsigned int& y, const unsigned int& color)
{
	unsigned char*	p = _data;
	unsigned int	pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	_setAPix[_format](p, color);
}

void DImage::SetPixel(const unsigned int& x, const unsigned int& y, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	unsigned char* p = _data;
	unsigned int	pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	_setBPix[_format](p, r, g, b, a);
}

void DImage::GetPixel(const unsigned int& x, const unsigned int& y, unsigned char& r, unsigned char& g, unsigned char& b) const
{
	unsigned char* p = _data;
	unsigned int pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	_getAPix[_format](p, r, g, b);
}

void DImage::GetPixel(const unsigned int& x, const unsigned int& y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const
{
	unsigned char* p = _data;
	unsigned int pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	_getBPix[_format](p, r, g, b, a);
}

const ColorFormat& DImage::GetFormat() const
{
	return (_format);
}

const unsigned int& DImage::GetSizeLine() const
{
	return (_sizeLine);
}

const unsigned int& DImage::GetBytesPerPixel() const
{
	return (_bpp);
}

void DImage::SetAPixelRGBA(unsigned char* p, const unsigned int& color)
{
	*((int*)(p)) = color;
}

void DImage::SetBPixelRGBA(unsigned char* p, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	p[0] = a;
	p[1] = b;
	p[2] = g;
	p[3] = r;
}

#include <iostream>

void DImage::SetAPixelRGB(unsigned char* p, const unsigned int& color)
{
	p[0] = (color & 0x0000FF00) >> 8;
	p[1] = (color & 0x00FF0000) >> 16;
	p[2] = (color & 0xFF000000) >> 24;
}

void DImage::SetBPixelRGB(unsigned char* p, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	p[0] = b;
	p[1] = g;
	p[2] = r;
}

void DImage::SetAPixelARGB(unsigned char* p, const unsigned int& color)
{
	//std::cout << "SetAPixelARGB(" << std::endl;
	memcpy(p, &color, sizeof(unsigned int));
	//*p = color;
	//p[3] = 255;
}

void DImage::SetBPixelARGB(unsigned char* p, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	//std::cout << "SetAPixelARGB(rgba" << std::endl;
	p[1] = r;
	p[2] = g;
	p[3] = b;
}

void DImage::GetAPixel(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, const unsigned int& pr, const unsigned int& pg, const unsigned int& pb)
{
	//std::cout << "GET A PIXEL RGB" << std::endl;
	r = p[pr];
	g = p[pg];
	b = p[pb];
	//std::cout << "R (" << (int)r << ")" << std::endl;
}

void DImage::GetBPixel(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a, const unsigned int& pr, const unsigned int& pg, const unsigned int& pb, const unsigned int& pa)
{
	r = p[pr];
	g = p[pg];
	b = p[pb];
	a = p[pa];
}

void DImage::GetAPixelRGBA(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b)
{
	GetAPixel(p, r, g, b, 0, 1, 2);
}

void DImage::GetBPixelRGBA(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a)
{
	GetBPixel(p, r, g, b, a, 0, 1, 2, 3);
}

void DImage::GetAPixelRGB(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b)
{
	GetAPixel(p, r, g, b, 0, 1, 2);
}

void DImage::GetBPixelRGB(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a)
{
	GetAPixel(p, r, g, b, 0, 1, 2);
}

void DImage::GetAPixelARGB(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b)
{
	GetAPixel(p, r, g, b, 2, 1, 0);
}

void DImage::GetBPixelARGB(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a)
{
	GetBPixel(p, r, g, b, a, 2, 1, 0, 3);
}

void DImage::GetAPixelBGRA(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b)
{
	GetAPixel(p, r, g, b, 2, 1, 0);
}

void DImage::GetBPixelBGRA(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a)
{
	GetBPixel(p, r, g, b, a, 2, 1, 0, 3);
}

void DImage::InitByFormat()
{
	if (_format == DEFAULT || _format == RGBA || _format == ARGB || _format == ABGR
		|| _format == BGRA)
		_bpp = 4;
	else // Cannot Use Less then RGB values so -> 3 
		_bpp = 3;
	std::cout << "BPP (" << _bpp << ")" << std::endl;
	_sizeLine = _width * _bpp;
	_size = ((_sizeLine)* _height);
}