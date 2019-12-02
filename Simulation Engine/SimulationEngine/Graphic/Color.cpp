#include "Color.h"

#include <sstream>

const Color::ComputeValueFCT Color::_computeValues[COLOR_FORMAT_COUNT] = { &Color::ComputeValueRGBA , &Color::ComputeValueRGBA, &Color::ComputeValueARGB,\
	NULL, NULL, &Color::ComputeValueRGB };

Color::Color()
{
	_init(0, 0, 0, 0, ColorFactory::Get().GetFormat());
}

Color::Color(const ColorDef& def)
{
	_init(def.r, def.g, def.b, def.a, ColorFactory::Get().GetFormat());
}

Color::Color(const ColorFDef& fdef)
{
	_init((unsigned char)fdef.r, (unsigned char)fdef.g, (unsigned char)fdef.b, (unsigned char)fdef.a, ColorFactory::Get().GetFormat());
}

Color::Color(const ColorName& name)
{
	_format = ColorFactory::Get().GetFormat();
	_initFromName(name);
}

Color::Color(const ColorFormat& format)
{
	_init(0, 0, 0, 0, format);
}

Color::Color(const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
	_init(r, g, b, 0, ColorFactory::Get().GetFormat());
}

Color::Color(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	_init(r, g, b, a, ColorFactory::Get().GetFormat());
}

Color::Color(const unsigned char& r, const unsigned char& g, const unsigned char& b, const ColorFormat& format)
{
	_init(r, g, b, 0, format);
}

Color::Color(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a, const ColorFormat& format)
{
	_init(r, g, b, a, format);
}

Color& Color::operator=(const Color& other)
{
	if (this != &other)
	{
		_init(other._def.r, other._def.g, other._def.b, other._def.a, other._format);
	}
	return (*this);
}

Color::Color(const ColorName& name, const ColorFormat& format)
{
	_format = format;
	_initFromName(name);
}

void Color::ComputeValue()
{
	_value = _computeValue(_def.r, _def.g, _def.b, _def.a);
}

unsigned int Color::ComputeValueRGB(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	unsigned int value = b;
	value += g << 8;
	value += b << 16;
	return (value);
}

unsigned int Color::ComputeValueRGBA(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	unsigned int value = a;
	value += b << 8;
	value += g << 16;
	value += r << 24;
	return (value);
}

unsigned int Color::ComputeValueARGB(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	unsigned int value = b;
	value += g << 8;
	value += r << 16;
	value += a << 24;
	return (value);
}

#include <iostream>

void Color::_init(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a, const ColorFormat& format)
{
	_def.r = r;
	_def.g = g;
	_def.b = b;
	_def.a = a;
	_format = format;
	_computeValue = _computeValues[_format];
	ComputeValue();
	// std::cout << "Color Is Format (" << _format << ")" << std::endl;
}

void Color::_initFromName(const ColorName& name)
{
	if (name == ColorName::RED) {
		_init(255, 0, 0, 0, _format);
	}
	else if (name == ColorName::WHITE) {
		_init(255, 255, 255, 0, _format);
	}
	else if (name == ColorName::GREY) {
		_init(120, 120, 120, 0, _format);
	}
	else if (name == ColorName::GREEN) {
		_init(0, 255, 0, 0, _format);
	}
	else if (name == ColorName::BLUE) {
		_init(0, 0, 255, 0, _format);
	}
	else if (name == ColorName::ORANGE) {
		_init(255, 165, 0, 0, _format);
	}
	else if (name == ColorName::YELLOW) {
		_init(255, 255, 0, 0, _format);
	}
	else if (name == ColorName::PURPLE) {
		_init(130, 0, 130, 0, _format);
	}
	else {
		_init(0, 0, 0, 0, _format); // BLACK
	}
}

bool Color::operator==(const Color& other) const
{
	return (_value == other._value);
}

bool Color::operator!=(const Color& other) const
{
	return (_value != other._value);
}

const ColorFormat& Color::GetFormat() const
{
	return (_format);
}

const ColorDef& Color::GetDef() const
{
	return (_def);
}

std::string Color::ToString() const
{
	std::stringstream ss;
	ss << "R: (" << (int)(_def.r) << ") G:(" << (int)_def.g << ") B:(" << (int)_def.b << ") A:(" << (int)_def.a << ") V:(" << _value << ")";
	return (ss.str());
}
void Color::Set(const unsigned char& r, const unsigned char& g, const unsigned char& b)
{
	_def.r = r;
	_def.g = g;
	_def.b = b;
	ComputeValue();
}
void Color::Set(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	_def.r = r;
	_def.g = g;
	_def.b = b;
	_def.a = a;
	ComputeValue();
}

void Color::Set(const ColorDef& def)
{
	_def.r = def.r;
	_def.g = def.g;
	_def.b = def.b;
	_def.a = def.a;
	ComputeValue();
}


const unsigned char& Color::r() const
{
	return (_def.r);
}

const unsigned char& Color::g() const
{
	return (_def.g);
}

const unsigned char& Color::b() const
{
	return (_def.b);
}

const unsigned char& Color::a() const
{
	return (_def.a);
}

const unsigned int& Color::value() const
{
	return (_value);
}

ColorFactory::ColorFactory()
{
	SetFormat(ColorFormat::RGBA);
}

void ColorFactory::SetFormat(ColorFormat format)
{
	_format = format;
	
	if (format == ColorFormat::RGB || format == ColorFormat::DEFAULT) {

		_formatBytesPerPixel = 3;
	}
	else  if (format ==  ColorFormat::RGBA || format == ColorFormat::ABGR
		|| format == ColorFormat::ARGB || format == ColorFormat::BGRA) 
	{
		_formatBytesPerPixel = 4;
	} 
	else 
	{
		_formatBytesPerPixel = 1; // No Overflow
	}
}

Color ColorFactory::Create()
{
	return (Color(0, 0, 0, 0, _format));
}

Color ColorFactory::Create(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	return (Color(r, g, b, a, _format));
}

const ColorFormat& ColorFactory::GetFormat() const
{
	return (_format);
}

const unsigned int& ColorFactory::GetFormatBytesPerPixel() const
{
	return (_formatBytesPerPixel);
}
