#include "IImage.h"


///////////////////////////
////// ALL Code HERE 
///////// Is 
///////// For
//////	 LITTLE - ENDIAN ///////
////////////////////////////////////////

//															DEFAULT,				RGBA,					ARGB,
//	ABGR, BGRA, 			RGB (5)										
const AImage::SetAPix AImage::_setAPix[COLOR_FORMAT_COUNT] = { &AImage::SetAPixelRGBA , &AImage::SetAPixelRGBA, &AImage::SetAPixelARGB,\
	NULL, NULL, &AImage::SetAPixelRGB };
const AImage::SetBPix AImage::_setBPix[COLOR_FORMAT_COUNT] = { &AImage::SetBPixelRGBA , &AImage::SetBPixelRGBA, &AImage::SetBPixelARGB,\
	NULL, NULL, &AImage::SetBPixelRGB };
const AImage::GetAPix AImage::_getAPix[COLOR_FORMAT_COUNT] = { &AImage::GetAPixelRGBA , &AImage::GetAPixelRGBA, &AImage::GetAPixelARGB,\
	NULL, &AImage::GetAPixelBGRA, &AImage::GetAPixelRGB };
const AImage::GetBPix AImage::_getBPix[COLOR_FORMAT_COUNT]
= { &AImage::GetBPixelRGBA , &AImage::GetBPixelRGBA, &AImage::GetBPixelARGB,\
	NULL, &AImage::GetBPixelBGRA, &AImage::GetBPixelRGB };


AImage::AImage()
{
	_format = ColorFormat::DEFAULT;
	_size = 0;
	_width = 0;
	_height = 0;
	_data = 0;
	_bpp = 0;
	_sizeLine = 0;
}

AImage::~AImage()
{
}

void AImage::Clear()
{
	if (_data)
		memset(_data, 0, _size);
}

void AImage::SetLine(const unsigned int& x, const unsigned int& y, unsigned char* line, const unsigned int& lineSize)
{
	unsigned char* p = _data;
	unsigned int	pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	memcpy(p, line, lineSize);
}


// COLOR Received here is ALWAYS in RGBA Mode (Always !)
void AImage::SetPixel(const unsigned int & x, const unsigned int & y, const unsigned int & color)
{
	unsigned char * p = _data;
	unsigned int	pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	_setAPix[_format](p, color);
}

void AImage::SetPixel(const unsigned int & x, const unsigned int & y, const unsigned char & r, const unsigned char & g, const unsigned char & b, const unsigned char & a)
{
	unsigned char * p = _data;
	unsigned int	pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	_setBPix[_format](p, r, g, b, a);
}

void AImage::GetPixel(const unsigned int & x, const unsigned int & y, unsigned char & r, unsigned char & g, unsigned char & b) const
{
	unsigned char * p = _data;
	unsigned int pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	_getAPix[_format](p, r, g, b);
}

void AImage::GetPixel(const unsigned int & x, const unsigned int & y, unsigned char & r, unsigned char & g, unsigned char & b, unsigned char & a) const
{
	unsigned char * p = _data;
	unsigned int pos = (y * _sizeLine) + (x * _bpp);
	if (pos >= _size)
		return;
	p += pos;
	_getBPix[_format](p, r, g, b, a);
}

const ColorFormat& AImage::GetFormat() const
{
	return (_format);
}

const unsigned int& AImage::GetSizeLine() const
{
	return (_sizeLine);
}

const unsigned int& AImage::GetBytesPerPixel() const
{
	return (_bpp);
}

void AImage::SetAPixelRGBA(unsigned char * p, const unsigned int & color)
{
	*((int*)(p)) = color;
}

void AImage::SetBPixelRGBA(unsigned char * p, const unsigned char& r,  const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	p[0] = a;
	p[1] = b;
	p[2] = g;
	p[3] = r;
}

#include <iostream>

void AImage::SetAPixelRGB(unsigned char * p, const unsigned int & color)
{
	p[0] = (color & 0x0000FF00) >> 8;
	p[1] = (color & 0x00FF0000) >> 16;
	p[2] = (color & 0xFF000000) >> 24;
}

void AImage::SetBPixelRGB(unsigned char * p, const unsigned char& r,  const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	p[0] = b;
	p[1] = g;
	p[2] = r;
}

void AImage::SetAPixelARGB(unsigned char * p, const unsigned int & color)
{
//	std::cout << "SetAPixelARGB(" << std::endl;
	p[0] = (color & 0x0000FF00) >> 8; // B
	p[1] = (color & 0x00FF0000) >> 16; // G
	p[2] = (color & 0xFF000000) >> 24; // R
	//p[3] = 255;
}

void AImage::SetBPixelARGB(unsigned char * p, const unsigned char & r, const unsigned char & g, const unsigned char & b, const unsigned char & a)
{
	p[0] = b;
	p[1] = g;
	p[2] = r;
}

void AImage::GetAPixel(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b, const unsigned int & pr, const unsigned int & pg, const unsigned int & pb)
{
	//std::cout << "GET A PIXEL RGB" << std::endl;
	r = p[pr];
	g = p[pg];
	b = p[pb];
	//std::cout << "R (" << (int)r << ")" << std::endl;
}

void AImage::GetBPixel(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b, unsigned char & a, const unsigned int & pr, const unsigned int & pg, const unsigned int & pb, const unsigned int & pa)
{
	r = p[pr];
	g = p[pg];
	b = p[pb];
	a = p[pa];
}

void AImage::GetAPixelRGBA(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b)
{
	GetAPixel(p, r, g, b, 0, 1, 2);
}

void AImage::GetBPixelRGBA(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b, unsigned char & a)
{
	GetBPixel(p, r, g, b, a, 0, 1, 2, 3);
}

void AImage::GetAPixelRGB(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b)
{
	GetAPixel(p, r, g, b, 0, 1, 2);
}

void AImage::GetBPixelRGB(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b, unsigned char & a)
{
	GetAPixel(p, r, g, b, 0, 1, 2);
}

void AImage::GetAPixelARGB(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b)
{
	GetAPixel(p, r, g, b, 2, 1, 0);
}

void AImage::GetBPixelARGB(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b, unsigned char & a)
{
	GetBPixel(p, r, g, b, a, 2, 1, 0, 3);
}

void AImage::GetAPixelBGRA(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b)
{
	GetAPixel(p, r, g, b, 2, 1, 0);
}

void AImage::GetBPixelBGRA(unsigned char * p, unsigned char & r, unsigned char & g, unsigned char & b, unsigned char & a)
{
	GetBPixel(p, r, g, b, a, 2, 1, 0, 3);
}

void AImage::InitByFormat()
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

