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

	const std::vector<Pixel>&		GetPixels() const;

private:

	int						_w, _h;
	std::vector<Pixel>		_pixels;
};

