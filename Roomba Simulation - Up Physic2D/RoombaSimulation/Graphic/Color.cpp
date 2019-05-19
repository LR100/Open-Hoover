#include "Color.h"

#include <sstream>

Color::Color(const unsigned int & avalue)
{
	value = avalue;
	r = (unsigned char)(value >> 24);
	g = (unsigned char)(value >> 16);
	b = (unsigned char)(value >> 8);
}

Color::Color(const unsigned char & _r, const unsigned char & _g, const unsigned char & _b)
{
	r = _r;
	g = _g;
	b = _b;
	ComputeValue();
}

Color::Color()
{
	r = 0;
	g = 0;
	b = 0;
	value = 0;
}

Color::~Color()
{
}

Color::Color(const Color & other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	value = other.value;
}

Color & Color::operator=(const Color & other)
{
	if (this != &other)
	{
		r = other.r;
		g = other.g;
		b = other.b;
		value = other.value;
	}
	return (*this);
}

bool Color::operator==(const Color & other) const
{
	// Considering That Value is Compute
	return (value == other.value);
}

bool Color::operator!=(const Color & other) const
{
	return (value != other.value);
}

void Color::ComputeValue()
{
	value = r << 24;
	value += g << 16;
	value += b << 8;
}

std::string Color::ToString() const
{
	std::stringstream ss;

	ss << "R: (" << (int)(r) << ") G:(" << (int)g << ") B:(" << (int)b << ") V:(" << value << ")";
	return (ss.str());
}

ColorF::ColorF(const unsigned int & avalue)
{
	value = avalue;
	r = (float)(value >> 24);
	g = (float)(value >> 16);
	b = (float)(value >> 8);
}

ColorF::ColorF(const float & _r, const float & _g, const float & _b)
{
	r = _r;
	g = _g;
	b = _b;
	ComputeValue();
}

ColorF::ColorF()
{
	r = 0;
	g = 0;
	b = 0;
	value = 0;
}

ColorF::~ColorF()
{
}

ColorF::ColorF(const ColorF & other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	value = other.value;
}

ColorF & ColorF::operator=(const Color & other)
{
	r = (float)other.r;
	g = (float)other.g;
	b = (float)other.b;
	value = other.value;
	return (*this);
}

ColorF & ColorF::operator=(const ColorF & other)
{
	if (this != &other)
	{
		r = other.r;
		g = other.g;
		b = other.b;
		value = other.value;
	}
	return (*this);
}

ColorF & ColorF::operator*=(const float & value)
{
	r *= value;
	g *= value;
	b *= value;
	return (*this);
}

ColorF ColorF::operator*(const float & value)
{
	return ColorF(r * value, g * value, b * value);
}

ColorF & ColorF::operator+=(const ColorF & other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	return (*this);
}

void ColorF::ComputeValue()
{
	value = (unsigned char)r << 24;
	value += (unsigned char)g << 16;
	value += (unsigned char)b << 8;
}

std::string ColorF::ToString() const
{
	std::stringstream ss;

	ss.precision(2);
	ss << "R: (" << r << ") G:(" << g << ") B:(" << b << ") V:(" << value << ")";
	return (ss.str());
}
