#include "EventHandlerSDL.h"

#include "../BGContextSDL.h"

EventHandlerSDL::EventHandlerSDL()
{
	BGContextSDL::Get().NotifyCreatedItem();
	/// KEYS
	// Basics
	_keysLink[SDLK_RETURN] = ControlKey::KEY_RETURN;
	_keysLink[SDLK_ESCAPE] = ControlKey::KEY_ESCAPE;
	_keysLink[SDLK_RSHIFT] = ControlKey::KEY_RSHIFT;
	_keysLink[SDLK_LSHIFT] = ControlKey::KEY_LSHIFT;
	_keysLink[SDLK_LCTRL] = ControlKey::KEY_LCTRL;
	_keysLink[SDLK_RCTRL] = ControlKey::KEY_RCTRL;
	_keysLink[SDLK_SPACE] = ControlKey::KEY_SPACE;
	_keysLink[SDLK_TAB] = ControlKey::KEY_TAB;
	// Arrows
	_keysLink[SDLK_UP] = ControlKey::KEY_UP;
	_keysLink[SDLK_DOWN] = ControlKey::KEY_DOWN;
	_keysLink[SDLK_RIGHT] = ControlKey::KEY_RIGHT;
	_keysLink[SDLK_LEFT] = ControlKey::KEY_LEFT;
	// Chars
	_keysLink[SDLK_a] = ControlKey::KEY_A;
	_keysLink[SDLK_b] = ControlKey::KEY_B;
	_keysLink[SDLK_c] = ControlKey::KEY_C;
	_keysLink[SDLK_d] = ControlKey::KEY_D;
	_keysLink[SDLK_e] = ControlKey::KEY_E;
	_keysLink[SDLK_f] = ControlKey::KEY_F;
	_keysLink[SDLK_g] = ControlKey::KEY_G;
	_keysLink[SDLK_h] = ControlKey::KEY_H;
	_keysLink[SDLK_i] = ControlKey::KEY_I;
	_keysLink[SDLK_j] = ControlKey::KEY_J;
	_keysLink[SDLK_k] = ControlKey::KEY_K;
	_keysLink[SDLK_l] = ControlKey::KEY_L;
	_keysLink[SDLK_m] = ControlKey::KEY_M;
	_keysLink[SDLK_n] = ControlKey::KEY_N;
	_keysLink[SDLK_o] = ControlKey::KEY_O;
	_keysLink[SDLK_p] = ControlKey::KEY_P;
	_keysLink[SDLK_q] = ControlKey::KEY_Q;
	_keysLink[SDLK_r] = ControlKey::KEY_R;
	_keysLink[SDLK_s] = ControlKey::KEY_S;
	_keysLink[SDLK_t] = ControlKey::KEY_T;
	_keysLink[SDLK_u] = ControlKey::KEY_U;
	_keysLink[SDLK_v] = ControlKey::KEY_V;
	_keysLink[SDLK_w] = ControlKey::KEY_W;
	_keysLink[SDLK_x] = ControlKey::KEY_X;
	_keysLink[SDLK_y] = ControlKey::KEY_Y;
	_keysLink[SDLK_z] = ControlKey::KEY_Z;
	// Nums
	_keysLink[SDLK_0] = ControlKey::KEY_NUM0;
	_keysLink[SDLK_1] = ControlKey::KEY_NUM1;
	_keysLink[SDLK_2] = ControlKey::KEY_NUM2;
	_keysLink[SDLK_3] = ControlKey::KEY_NUM3;
	_keysLink[SDLK_4] = ControlKey::KEY_NUM4;
	_keysLink[SDLK_5] = ControlKey::KEY_NUM5;
	_keysLink[SDLK_6] = ControlKey::KEY_NUM6;
	_keysLink[SDLK_7] = ControlKey::KEY_NUM7;
	_keysLink[SDLK_8] = ControlKey::KEY_NUM8;
	_keysLink[SDLK_9] = ControlKey::KEY_NUM9;

	/// MOUSE
	_mouseLink[SDL_BUTTON_LEFT] = ControlKey::MOUSE_LEFT;
	_mouseLink[SDL_BUTTON_RIGHT] = ControlKey::MOUSE_RIGHT;

	/// WINDOW
	_windowLink[SDL_WINDOWEVENT_MOVED] = ControlKey::WINDOW_MOVE;
	_windowLink[SDL_WINDOWEVENT_CLOSE] = ControlKey::WINDOW_CLOSE;
	_windowLink[SDL_WINDOWEVENT_FOCUS_GAINED] = ControlKey::WINDOW_FOCUS;
	_windowLink[SDL_WINDOWEVENT_FOCUS_LOST] = ControlKey::WINDOW_UNFOCUS;
	_windowLink[SDL_WINDOWEVENT_RESIZED] = ControlKey::WINDOW_RESIZE;

	/// TYPE
	_typeLink[SDL_QUIT] = &EventHandlerSDL::HandleTypeQuit;
	_typeLink[SDL_KEYDOWN] = &EventHandlerSDL::HandleTypeKeyDown;
	_typeLink[SDL_KEYUP] = &EventHandlerSDL::HandleTypeKeyUp;
	_typeLink[SDL_MOUSEWHEEL] = &EventHandlerSDL::HandleTypeMouseWheel;
	_typeLink[SDL_MOUSEBUTTONDOWN] = &EventHandlerSDL::HandleTypeMouseButtonDown;
	_typeLink[SDL_MOUSEBUTTONUP] = &EventHandlerSDL::HandleTypeMouseButtonUp;
	_typeLink[SDL_MOUSEMOTION] = &EventHandlerSDL::HandleTypeMouseMotion;
	_typeLink[SDL_WINDOWEVENT] = &EventHandlerSDL::HandleTypeWindow;
}

EventHandlerSDL::~EventHandlerSDL()
{
	BGContextSDL::Get().NotifyDeletedItem();
}

#include <iostream>

void EventHandlerSDL::UpdateEvents()
{
	SDL_Event	event;
	int			mouseX, mouseY;

	SDL_GetMouseState(&mouseX, &mouseY);
	_mouse.x = (unsigned int)mouseX;
	_mouse.y = (unsigned int)mouseY;

	// Pressed Retainer
	std::unordered_map<ControlKey, Event>::iterator itpr = _pressedRetainer.begin();
	std::unordered_map<ControlKey, Event>::iterator itprEnd = _pressedRetainer.end();
	while (itpr != itprEnd)
	{
		//std::cout << "Function Hash(" << itpr->second.GetHash() << ")" << std::endl;
		HandleEvent(itpr->second.GetHash());
		itpr++;
	}
	//std::cout << "UpdateEvents" << std::endl;;
	while (SDL_PollEvent(&event))
	{
		_evt.key = ControlKey::KEY_NONE;
		_evt.type = Event::Type::NONE;
		_evt.state = ControlKeyState::NONE;
		_evt.idDevice = 0; // No set yet for keyboard and mouses
		if (_typeLink.count(event.type))
			(this->*_typeLink.at(event.type))(event);
		//std::cout << "Event: " << _evt.ToString();

		HandleEvent(_evt.GetHash());
	}
}

void EventHandlerSDL::HandleTypeQuit(const SDL_Event & event)
{
	_evt.type = Event::QUIT;
}

void EventHandlerSDL::HandleTypeKeyDown(const SDL_Event & event)
{
	if (_keysLink.count(event.key.keysym.sym))
		_evt.key = _keysLink.at(event.key.keysym.sym);
	_evt.type = Event::KEY;
	_evt.state = ControlKeyState::PRESSED;
}

void EventHandlerSDL::HandleTypeKeyUp(const SDL_Event & event)
{
	if (_keysLink.count(event.key.keysym.sym))
		_evt.key = _keysLink.at(event.key.keysym.sym);
	_evt.type = Event::KEY;
	_evt.state = ControlKeyState::RELEASED;
}

void EventHandlerSDL::HandleTypeMouseWheel(const SDL_Event & event)
{
	_evt.type = Event::MOUSE;
	_evt.key = ControlKey::MOUSE_SCROLL;
	_evt.state = Event::GetDefaultControlKeyStateFromControlKey(_evt.key);
	_mouse.xScroll = event.wheel.x;
	_mouse.yScroll = event.wheel.y;
}

void EventHandlerSDL::HandleTypeMouseButtonDown(const SDL_Event & event)
{
	_evt.type = Event::MOUSE;	
	if (_mouseLink.count(event.button.button))
		_evt.key = _mouseLink.at(event.button.button);
	_evt.state = ControlKeyState::PRESSED;
	// Correct Specification of SDL Handling by Retaining mouse pressed
	//std::cout << "Mouse Down" << std::endl;
	_pressedRetainer[_evt.key] = _evt;
}

void EventHandlerSDL::HandleTypeMouseButtonUp(const SDL_Event & event)
{
	_evt.type = Event::MOUSE;
	if (_mouseLink.count(event.button.button))
		_evt.key = _mouseLink.at(event.button.button);
	// Release Mouse ControlKey Pressed
	if (_pressedRetainer.count(_evt.key))
		_pressedRetainer.erase(_evt.key);
	//std::cout << "Mouse Up" << std::endl;
	_evt.state = ControlKeyState::RELEASED;
}

void EventHandlerSDL::HandleTypeMouseMotion(const SDL_Event & event)
{
	_evt.type = Event::MOUSE;
	_evt.key = ControlKey::MOUSE_MOTION;
	_evt.state = Event::GetDefaultControlKeyStateFromControlKey(_evt.key);
}

void EventHandlerSDL::HandleTypeWindow(const SDL_Event & event)
{
	_evt.type = Event::WINDOW;
	_evt.idDevice = event.window.windowID;
	if (_windowLink.count(event.window.event))
	{
		_evt.key = _windowLink.at(event.window.event);
		_evt.state = Event::GetDefaultControlKeyStateFromControlKey(_evt.key);
	}
	// !! HANDLE EVENT WITH ID DEVICE !!
	HandleEvent(_evt.GetHash());
	// Then Set It for Handling WITHOUT Id Device
	// This Procedure allow the user of EventHandler 
	// to handle any kind of Window Event whatever the window it is
	_evt.idDevice = 0;
}
