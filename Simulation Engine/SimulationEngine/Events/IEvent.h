#pragma once

#include <stdint.h>
#include <string>

enum ControlKeyState : uint8_t
{
	NONE = 0,
	PRESSED = 1, // Can be used as START
	RELEASED = 3 // Can be used as STOP
};

enum ControlKey : uint16_t
{
	KEY_NONE = 0, // Stay AT TOP !
	// KEYS
	KEY_START,
	/////////

	// Using Value of Ascii Table
	KEY_ESCAPE = 27,
	KEY_SPACE = 32,
	KEY_NUM0 = 48, KEY_NUM1 = 49, KEY_NUM2 = 50, KEY_NUM3 = 51, KEY_NUM4,
	KEY_NUM5, KEY_NUM6, KEY_NUM7, KEY_NUM8, KEY_NUM9 = 57,
	KEY_A = 97, KEY_B = 98, KEY_C = 99, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
	KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
	KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,

	// Stop Using Ascii Table
	KEY_QUIT = 258,
	KEY_RETURN,

	KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT,
	KEY_LCTRL, KEY_RCTRL,
	KEY_LSHIFT, KEY_RSHIFT,
	KEY_TAB,

	// !! ALWAYS at END of KEYS !! //
	KEY_END,

	// MOUSE
	MOUSE_START,

	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_SCROLL,
	MOUSE_MOTION,

	// !! ALWAYS at END of MOUSE !! //
	MOUSE_END,

	// WINDOW
	WINDOW_START,

	WINDOW_CLOSE,
	WINDOW_RESIZE,
	WINDOW_MOVE,
	WINDOW_FOCUS,
	WINDOW_UNFOCUS,

	// !! ALWAYS at END of WINDOW !! //
	WINDOW_END
};

struct Mouse
{
	int xScroll, yScroll;
	unsigned int x, y;
};

struct Event
{
	enum Type : uint8_t
	{
		NONE = 0,
		KEY = 1,
		MOUSE = 2,
		WINDOW,
		QUIT
	};

	Event(const Type& _type, const ControlKey& _key, const ControlKeyState& _state);
	Event();

	static Event::Type GetTypeFromControlKey(const ControlKey& key);
	static ControlKeyState GetDefaultControlKeyStateFromControlKey(const ControlKey& key);

#define EVENT_TYPE_HASH_ uint64_t

	std::string			ToString() const;
	EVENT_TYPE_HASH_	GetHash() const;

	Type						type;
	ControlKey					key;
	ControlKeyState				state;
	uint32_t					idDevice; // For Multiple GamePad / KeyBoard
};