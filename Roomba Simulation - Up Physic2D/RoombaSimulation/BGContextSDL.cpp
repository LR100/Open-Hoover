#include "BGContextSDL.h"

#include <iostream>
#include <stdlib.h>


#include <SDL.h>
#include <SDL_ttf.h> // SDL_TTF 
#include <SDL_image.h> // SDL Image


BGContextSDL::BGContextSDL()
{
	_items = 0;
	_isInit = false;
}

BGContextSDL::~BGContextSDL()
{
	Quit();
}


bool BGContextSDL::Init()
{
	if (!_isInit)
	{
		// If are Already Running -> QUIT
		atexit(SDL_Quit); 
		TTF_Quit();

		// Init SDL //
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "SDL: Unable to init SDL video: " << SDL_GetError() << std::endl;
			return (false);
		}
	
		// Init SDL TTF	//
		if (TTF_Init() < 0)
		{
			std::cerr << "SDL TFF: Unable to init SDL TTF: " << SDL_GetError() << std::endl;
			return (false);
		}
		_isInit = true;
		std::cout << "BGContextSDL is Init" << std::endl;
	}
	return (true);
}

bool BGContextSDL::Quit()
{
	if (_isInit)
	{
		atexit(SDL_Quit);
		TTF_Quit();
		std::cout << "BGContextSDL QUIT" << std::endl;
		_isInit = false;
	}
	return (true);
}

void BGContextSDL::NotifyCreatedItem()
{
	_items += 1;
}

void BGContextSDL::NotifyDeletedItem()
{
	_items -= 1;
}
