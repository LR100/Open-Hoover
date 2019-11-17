#include "ImageSDL.h"

#include <iostream> // TMP DEBUG

ImageSDL::ImageSDL()
{
	_surface = NULL;
}

ImageSDL::~ImageSDL()
{
	if (_surface != NULL)
	{
		SDL_FreeSurface(_surface);
	}
}

void ImageSDL::Init(const unsigned int & width, const unsigned int & height, const Format & format)
{
	_width = width;
	_height = height;
	_format = format;
	
	if (format == Format::RGBA)
	{
		if (_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, SDL_RED_FLAG, SDL_GREEN_FLAG, SDL_BLUE_FLAG, SDL_ALPHA_FLAG))
		{
			_data = (unsigned char*)(_surface->pixels);
		}
		else
			std::cerr << "ImageSDL: Error at init: " << SDL_GetError() << "\n" ;
	}
	else // DEFAULT
	{
		SDL_Surface*	nsurface;
		_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, SDL_RED_FLAG, SDL_GREEN_FLAG, SDL_BLUE_FLAG, SDL_ALPHA_FLAG);
		if (_surface)
		{
			SDL_DisplayMode modeScreen;

			SDL_GetCurrentDisplayMode(0, &modeScreen);
			if ((nsurface = SDL_ConvertSurfaceFormat(_surface, modeScreen.format, SDL_RLEACCEL)))
			{
				SDL_FreeSurface(_surface);
				_surface = nsurface;
			}
			_data = (unsigned char*)(_surface->pixels);
		}
	}
	InitFromSurface(_surface);
}

const unsigned int& ImageSDL::GetWidth() const
{
	return (_width);
}

const unsigned int& ImageSDL::GetHeight() const
{
	return (_height);
}

IImage::Format ImageSDL::GetFormatBySDLFormat(SDL_PixelFormat * format)
{
	Uint32 f = format->format;
	return (GetFormatBySDLFormat(f));
}

IImage::Format ImageSDL::GetFormatBySDLFormat(Uint32 f)
{
	std::cout << "SDL FORMAT (" << SDL_GetPixelFormatName(f) << ")" << std::endl;
	if (f == SDL_PIXELFORMAT_BGRA8888)
		return (Format::BGRA);
	else if (f == SDL_PIXELFORMAT_ARGB8888 || f == SDL_PIXELFORMAT_RGB888) // For SDL RGB888 is in fact XXRRGGBB (4by)
		return (Format::ARGB);
	else if (f == SDL_PIXELFORMAT_ABGR8888)
		return (Format::ABGR);
	else if (f == SDL_PIXELFORMAT_RGBA8888)
		return (Format::RGBA);
	else if (f == SDL_PIXELFORMAT_RGB24)
		return (Format::RGB);
	else
	{
		std::cout << "UNKOWN FORMAT:" << std::endl;
		return (Format::DEFAULT);
	}
}

void ImageSDL::InitFormatBySDLFormat(SDL_PixelFormat * format)
{
	_format = GetFormatBySDLFormat(format);
	std::cout << "Format IMAGE SDL (" << IImage::FormatToString(_format) << ")" << std::endl;
}

void ImageSDL::InitFromSurface(SDL_Surface * surface)
{
	InitFormatBySDLFormat(surface->format);
	_surface = surface;
	_width = surface->w;
	_height = surface->h;
	_data = (unsigned char*)(_surface->pixels);
	_bpp = surface->format->BytesPerPixel;
	_sizeLine = _width * _bpp;
	_size = ((_sizeLine) * _height);
}

bool ImageSDL::Export(const std::string & path) const
{
	std::string fpath = path + ".bmp";
	SDL_SaveBMP(_surface, fpath.c_str());
	return (true);
}
