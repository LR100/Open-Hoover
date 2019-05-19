#include "WindowSDL.h"

#include <iostream>

WindowSDL::WindowSDL()
{
	InitBasics();
}

WindowSDL::WindowSDL(unsigned int width, unsigned int height, const std::string & name)
{
	InitBasics();
	Open(width, height, name);
}

WindowSDL::WindowSDL(IEventHandler * eHandler, unsigned int width, unsigned int height, const std::string & name)
{
	InitBasics();
	Open(width, height, name);
	SetEventHandler(eHandler);
}

WindowSDL::~WindowSDL()
{
	BGContextSDL::Get().NotifyDeletedItem();
}


void WindowSDL::InitBasics()
{
	BGContextSDL::Get().NotifyCreatedItem();
	_linkedEventHandler = NULL;
	_window = NULL;
	_open = false;
	_id = 0;
	_showCursor = true;
	_borderless = false;
}

void WindowSDL::SetEventHandler(IEventHandler * eHandler)
{
	// Remove From Old IeventHandler if present one
	// Then Set New One
	_linkedEventHandler = eHandler;
	// And Assign Event
	Event	event;

	event.idDevice = GetID();
	event.type = Event::Type::WINDOW;
	
	_linkedEventHandler->SetPriority(IEventHandler::Priority::PERMANENT);

	event.key = ControlKey::WINDOW_CLOSE;
	_linkedEventHandler->AddHandlerToEvent(event, &WindowSDL::EventClose, this);
	event.key = ControlKey::WINDOW_RESIZE;
	_linkedEventHandler->AddHandlerToEvent(event, &WindowSDL::EventResize, this);
	event.key = ControlKey::WINDOW_FOCUS;
	_linkedEventHandler->AddHandlerToEvent(event, &WindowSDL::EventFocus, this);
	event.key = ControlKey::WINDOW_UNFOCUS;
	_linkedEventHandler->AddHandlerToEvent(event, &WindowSDL::EventUnFocus, this);

	_linkedEventHandler->SetPriority(IEventHandler::Priority::MEDIUM);
}

bool WindowSDL::Open(IEventHandler * eHandler, unsigned int width, unsigned int height, const std::string & name)
{
	if (Open(width, height, name))
	{
		SetEventHandler(eHandler);
		return (true);
	}
	return (false);
}

bool WindowSDL::Open(unsigned int width, unsigned int height, const std::string & name)
{
	if (!_open)
	{
		_width = width;
		_height = height;
		_window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height,  /* SDL_WINDOW_FULLSCREEN | */ /* SDL_WINDOW_BORDERLESS */ SDL_WINDOW_RESIZABLE);
		if (this->_window == NULL)
		{
			std::cerr << "SDL: Unable to create window: " << SDL_GetError() << std::endl;
			return (false);
		}
		
		SDL_UpdateWindowSurface(_window);
		_screen = SDL_GetWindowSurface(_window);
		
		
		_buffer = new ImageSDL();
		_buffer->InitFromSurface(_screen);
		_open = true;
		_id = SDL_GetWindowID(_window);

		SDL_SetWindowBordered(_window, (SDL_bool)!_borderless);
	}
}

bool WindowSDL::IsOpen() const
{
	return (_open);
}

void WindowSDL::Close()
{
	if (_window)
	{
		SDL_DestroyWindow(_window);
		_window = NULL;
		_open = false;
	}
}

bool WindowSDL::IsFocus() const
{
	return (_focus);
}

void WindowSDL::Focus()
{
	if (_window)
	{
		SDL_RaiseWindow(_window);
	}
}

void WindowSDL::Resize(const unsigned int & width, const unsigned int & height)
{
	if (_window)
	{
		_width = width;
		_height = height;
		SDL_SetWindowSize(_window, width, height);
	}
}

void WindowSDL::SetPosition(const int & x, const int & y)
{
	if (_window)
		SDL_SetWindowPosition(_window, x, y);
}

void WindowSDL::Refresh()
{
	if (_window)
		SDL_UpdateWindowSurface(_window);
}

const unsigned int & WindowSDL::GetWidth() const
{
	return (_width);
}

const unsigned int & WindowSDL::GetHeight() const
{
	return (_height);
}

IImage * WindowSDL::GetBackBuffer() const
{
	return (_buffer);
}

const uint32_t & WindowSDL::GetID() const
{
	return (_id);
}

void WindowSDL::SetBorderless(bool state)
{
	_borderless = state;
	if (_window)
		SDL_SetWindowBordered(_window, (SDL_bool)!_borderless);
}

void WindowSDL::CursorHide(bool state)
{
	_showCursor = !state;
}

void WindowSDL::CursorToCenter()
{
	SDL_WarpMouseInWindow(_window, (int)((float)this->_width / 2.0f), (int)((float)this->_height / 2.0f));
}


void WindowSDL::EventResize()
{
	int w, h;
	SDL_GetWindowSize(_window, &w, &h);
	_width = (unsigned int)w;
	_height = (unsigned int)h;
	Refresh();
	_screen = SDL_GetWindowSurface(_window);
	_buffer->InitFromSurface(_screen);
}

void WindowSDL::EventClose()
{
	Close();
}

void WindowSDL::EventFocus()
{
	_focus = true;
	SDL_ShowCursor(_showCursor); // Set Cursor Mode
}

void WindowSDL::EventUnFocus()
{
	_focus = false;
	SDL_ShowCursor(true); // Set Cursor Mode
}

