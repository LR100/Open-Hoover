#pragma once

#include "IEventHandler.h"
#include "IEvent.h"
// STD 
#include <unordered_map>

// SDL
#include <SDL.h>

class EventHandlerSDL : public IEventHandler
{
public:
	EventHandlerSDL();
	~EventHandlerSDL();

	virtual void	UpdateEvents() override;

private:

	void			HandleTypeQuit(const SDL_Event& event);
	void			HandleTypeKeyDown(const SDL_Event& event);
	void			HandleTypeKeyUp(const SDL_Event& event);
	void			HandleTypeMouseWheel(const SDL_Event& event);
	void			HandleTypeMouseButtonDown(const SDL_Event& event);
	void			HandleTypeMouseButtonUp(const SDL_Event& event);
	void			HandleTypeMouseMotion(const SDL_Event& event);
	void			HandleTypeWindow(const SDL_Event& event);

	typedef			void(EventHandlerSDL::*HandleTypeFunction)(const SDL_Event&);


	std::unordered_map<ControlKey, Event>			_pressedRetainer;
	std::unordered_map<Uint32, HandleTypeFunction>	_typeLink;
	std::unordered_map<SDL_Keycode, ControlKey>		_keysLink;
	std::unordered_map<Uint8, ControlKey>			_mouseLink;
	std::unordered_map<Uint8, ControlKey>			_windowLink;
	Event											_evt;
};

