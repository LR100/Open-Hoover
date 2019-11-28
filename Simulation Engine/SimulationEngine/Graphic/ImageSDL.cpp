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

void ImageSDL::Init(const unsigned int & width, const unsigned int & height, const ColorFormat & format)
{
	_width = width;
	_height = height;
	_format = format;
	
	if (format == ColorFormat::RGBA)
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

#include <bitset>
#include <map>
#include <deque>

ColorFormat ImageSDL::GetFormatBySDLFormat(SDL_PixelFormat * format)
{
	Uint32 f = format->format;
	std::map<uint32_t, unsigned char>			formatMap;
	std::string									formatStr("    "); // 4 spaces


	formatMap.emplace(format->Rmask, 'R');
	formatMap.emplace(format->Gmask, 'G');
	formatMap.emplace(format->Bmask, 'B');
	formatMap.emplace(format->Amask, 'A');	
	
	std::map<uint32_t, unsigned char>::iterator	it = formatMap.begin();
	std::map<uint32_t, unsigned char>::iterator	itEnd = formatMap.end();
	uint8_t placed = 4;
	std::deque<unsigned char> unplaced;

	for (; it != itEnd; it++) {
		if (it->first == 4278190080) { // 1000
			formatStr[0] = it->second;
		} else if (it->first == 16711680) { // 0100
			formatStr[1] = it->second;
		} else if (it->first == 65280) { // 0010
			formatStr[2] = it->second;
		} else if (it->first == 255) { // 0001
			formatStr[3] = it->second;
		} else {
			placed -= 1;
			unplaced.push_back(it->second);
		}
	}

	while (format->BytesPerPixel > placed && unplaced.size()) {
		for (size_t i = 0; i < formatStr.size(); i += 1) {
			if (formatStr.at(i) == ' ') {
				formatStr[i] = unplaced.back();
				unplaced.pop_back();
				placed += 1;
			}
		}
	}

	uint32_t a;
	std::bitset<32> x;

	a = format->Rmask;
	x = std::bitset<32>(a);
	std::cout << "rmask" << x << ' ' << a << ")\n";

	a = format->Gmask;
	x = std::bitset<32>(a);
	std::cout << "gmask" << x << ' ' << a << ")\n";

	a = format->Bmask;
	x = std::bitset<32>(a);
	std::cout << "bmask" << x << ' ' << a << ")\n";

	a = format->Amask;
	x = std::bitset<32>(a);
	std::cout << "amask" << x << ' ' << a << ")\n";

	std::cout << "Format STR (" << formatStr << ")" << std::endl;

	return (ColorFormatFromString(formatStr));
}

void ImageSDL::InitFormatBySDLFormat(SDL_PixelFormat * format)
{
	_format = GetFormatBySDLFormat(format);
	std::cout << "InitFormatBySDLFormat (" << ColorFormatToString(_format) << ")" << std::endl;
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
