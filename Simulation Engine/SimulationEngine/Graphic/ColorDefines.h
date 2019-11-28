#pragma once

#define COLOR_FORMAT_COUNT 6

enum ColorFormat
{
	DEFAULT = 0,
	RGBA = 1,
	ARGB = 2,
	ABGR = 3,
	BGRA = 4,
	RGB
};

enum ColorName
{
	WHITE,
	BLACK,
	GREY,
	RED,
	GREEN,
	BLUE,
	ORANGE,
	YELLOW,
	PURPLE
};

#include <string>

std::string ColorFormatToString(const ColorFormat& format);
ColorFormat ColorFormatFromString(const std::string& format);
std::string ColorNameToString(const ColorName& colorName);