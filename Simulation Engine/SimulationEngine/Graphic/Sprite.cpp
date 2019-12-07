#include "Sprite.h"

#include <bitset>

Sprite::Sprite(IImage* image)
{
	Pixel			pixel;
	ColorDef		colorDef;
	unsigned int	x, y;

	_w = (int)image->GetWidth();
	_h = (int)image->GetHeight();

	for (y = 0; y < image->GetHeight(); y += 1)
	{
		for (x = 0; x < image->GetWidth(); x += 1)
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

Sprite::Sprite(IImage* image, ColorDef colorTransparency)
{
	Pixel					pixel;
	ColorDef				colorDef;
	std::vector<Color>		lineBuffer;
	unsigned int			linePixelCount;

	unsigned int	x, y;

	_w = (int)image->GetWidth();
	_h = (int)image->GetHeight();

	// std::cout << "Sprite Image Format (" << ColorFormatToString(image->GetFormat()) << ")" << std::endl;

	for (y = 0; y < image->GetHeight(); y += 1)
	{
		linePixelCount = 0;
		for (x = 0; x < image->GetWidth(); x += 1)
		{
			image->GetPixel(x, y, colorDef.r, colorDef.g, colorDef.b, colorDef.a);
			if (colorDef != colorTransparency || colorDef.a == 255)
			{
				pixel.color = Color(colorDef);
				pixel.x = (int)x;
				pixel.y = (int)y;
				_pixels.push_back(pixel);
				// Push IN Line
				lineBuffer.push_back(pixel.color);
				linePixelCount += 1;
			}
			else {
				// Push Line
				PushLine(x, y, linePixelCount, lineBuffer);
			}
		}
		// If line left
		PushLine(x, y, linePixelCount, lineBuffer);
	}
}

Sprite::~Sprite()
{
}

const std::vector<Sprite::Pixel>& Sprite::GetPixels() const
{
	return (_pixels);
}

const std::vector<Sprite::Line>& Sprite::GetLines() const
{
	return (_lines);
}

void Sprite::PushLine(const unsigned int& x, const unsigned int& y, unsigned int& linePixelCount, std::vector<Color>& lineBuffer)
{
	if (linePixelCount > 0)
	{
		size_t pos = 0;
		// First
		_lines.push_back(Line());
		// Set Position
		_lines.back().x = (x - linePixelCount);
		_lines.back().xE = (x - 1);
		_lines.back().y = y;
		_lines.back().lineSize = linePixelCount;

		if (ColorFactory::Get().GetFormatBytesPerPixel() == 3) {
			_lines.back().lineBytesSize = (sizeof(unsigned char) * 3 * linePixelCount);
			_lines.back().line = (unsigned char*)malloc(_lines.back().lineBytesSize);
			pos = 0;
			for (size_t i = 0; i < _lines.back().lineBytesSize; i += 3) {
				_lines.back().line[i + 0] = (lineBuffer.at(pos).value() & 0x000000FF);
				_lines.back().line[i + 1] = (lineBuffer.at(pos).value() & 0x0000FF00) >> 8;
				_lines.back().line[i + 2] = (lineBuffer.at(pos).value() & 0x00FF0000) >> 16;
				pos += 1;
			}
		}
		else if (ColorFactory::Get().GetFormatBytesPerPixel() == 4) {
			_lines.back().lineBytesSize = (sizeof(unsigned char) * 4 * linePixelCount);
			_lines.back().line = (unsigned char*)malloc(_lines.back().lineBytesSize);
			pos = 0;
			for (size_t i = 0; i < _lines.back().lineBytesSize; i += 4) {
				_lines.back().line[i + 0] = (lineBuffer.at(pos).value() & 0x000000FF);
				_lines.back().line[i + 1] = (lineBuffer.at(pos).value() & 0x0000FF00) >> 8;
				_lines.back().line[i + 2] = (lineBuffer.at(pos).value() & 0x00FF0000) >> 16;
				_lines.back().line[i + 3] = (lineBuffer.at(pos).value() & 0xFF000000) >> 24;
				pos += 1;
			}
		}
		// std::cout << "Start Line At (" << _lines.back().x << ") (" << _lines.back().y << ") of Size (" << linePixelCount << ")" << std::endl;
		// Reset Line
		lineBuffer.clear();
		linePixelCount = 0;
	}
}
