#pragma once

#include "IImage.h"
#include "Color.h"

#include <vector>

class Sprite
{
public:

	Sprite(IImage * image);
	Sprite(IImage * image, ColorDef colorTransparency);
	~Sprite();

	struct Pixel
	{
		int		x, y;
		Color	color;
	};

	struct Line
	{
		unsigned int	x, y;
		unsigned int	xE;
		unsigned int	lineBytesSize;
		unsigned int	lineSize;
		unsigned char*	line;
	};

	const std::vector<Pixel>&		GetPixels() const;
	const std::vector<Line>&		GetLines() const;

private:

	void	PushLine(const unsigned int& x, const unsigned int& y, unsigned int& linePixelCount, std::vector<Color>& lineBuffer);

	int						_w, _h;
	std::vector<Pixel>		_pixels;
	std::vector<Line>		_lines;
};

