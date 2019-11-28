#include "Sprite.h"

#include <bitset>

Sprite::Sprite(IImage * image)
{
	Pixel			pixel;
	ColorDef		colorDef;
	unsigned int	x, y;

	_w = (int)image->GetWidth();
	_h = (int)image->GetHeight();

	for (x = 0; x < image->GetWidth(); x += 1)
	{
		for (y = 0; y < image->GetHeight(); y += 1)
		{
			image->GetPixel(x, y, colorDef.r, colorDef.g, colorDef.b);
			pixel.color = Color(colorDef);
			std::cout << "Format Sprite Color : (" << ColorFormatToString(pixel.color.GetFormat()) << ")" << std::endl;
			pixel.x = (int)x;
			pixel.y = (int)y;
			_pixels.push_back(pixel);
		}
	}
}

#include <iostream> // TMP

Sprite::Sprite(IImage * image, ColorDef colorTransparency)
{
	Pixel			pixel;
	ColorDef		colorDef;
	
	unsigned int	x, y;

	_w = (int)image->GetWidth();
	_h = (int)image->GetHeight();

	
	std::cout << "Sprite Image Format (" << ColorFormatToString(image->GetFormat()) << ")" << std::endl;

	for (x = 0; x < image->GetWidth(); x += 1)
	{
		for (y = 0; y < image->GetHeight(); y += 1)
		{
			image->GetPixel(x, y, colorDef.r, colorDef.g, colorDef.b, colorDef.a);
			
			if (colorDef != colorTransparency || colorDef.a == 255)
			{
				pixel.color = Color(colorDef);
				pixel.x = (int)x;
				pixel.y = (int)y;
				_pixels.push_back(pixel);
			}
		}
	}
}

Sprite::~Sprite()
{
}

const std::vector<Sprite::Pixel>& Sprite::GetPixels() const
{
	return (_pixels);
}
