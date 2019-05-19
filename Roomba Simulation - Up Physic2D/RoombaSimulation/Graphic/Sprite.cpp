#include "Sprite.h"

Sprite::Sprite(IImage * image)
{
	Pixel			pixel;
	unsigned int	x, y;

	_w = (int)image->GetWidth();
	_h = (int)image->GetHeight();

	for (x = 0; x < image->GetWidth(); x += 1)
	{
		for (y = 0; y < image->GetHeight(); y += 1)
		{
			image->GetPixel(x, y, pixel.color.r, pixel.color.g, pixel.color.b);
			pixel.color.ComputeValue();
			pixel.x = (int)x;
			pixel.y = (int)y;
			_pixels.push_back(pixel);
		}
	}
}

#include <iostream> // TMP

Sprite::Sprite(IImage * image, Color colorTransparency)
{
	Pixel			pixel;
	unsigned int	x, y;

	_w = (int)image->GetWidth();
	_h = (int)image->GetHeight();

	std::cout << "Sprite Image Format (" << IImage::FormatToString(image->GetFormat()) << ")" << std::endl;
	
	for (x = 0; x < image->GetWidth(); x += 1)
	{
		for (y = 0; y < image->GetHeight(); y += 1)
		{
			image->GetPixel(x, y, pixel.color.r, pixel.color.g, pixel.color.b);
			pixel.color.ComputeValue();
			if (pixel.color != colorTransparency)
			{
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
