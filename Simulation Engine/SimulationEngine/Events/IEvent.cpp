#include "IEvent.h"

#include <sstream>

std::string EventTypeToString(const Event::Type& type)
{
	if (type == Event::Type::NONE)
		return ("NONE");
	else if (type == Event::Type::KEY)
		return ("KEY");
	else if (type == Event::Type::MOUSE)
		return ("MOUSE");
	else if (type == Event::Type::WINDOW)
		return ("WINDOW");
	else if (type == Event::Type::QUIT)
		return ("QUIT");
	return ("UNKNOWN");
}

std::string ControlKeyStateToString(const ControlKeyState& type)
{
	if (type == ControlKeyState::NONE)
		return ("NONE");
	else if (type == ControlKeyState::PRESSED)
		return ("PRESSED");
	else if (type == ControlKeyState::RELEASED)
		return ("RELEASED");
	return ("UNKNOWN");
}

std::string ControlKeyWindowToString(const ControlKey& key)
{
	if (key == ControlKey::KEY_NONE)
		return ("NONE");
	else if (key == ControlKey::WINDOW_CLOSE)
		return ("CLOSE");
	else if (key == ControlKey::WINDOW_RESIZE)
		return ("RESIZE");
	else if (key == ControlKey::WINDOW_MOVE)
		return ("MOVE");
	else if (key == ControlKey::WINDOW_FOCUS)
		return ("FOCUS");
	return ("UNKNOWN");
}

std::string ControlKeyMouseToString(const ControlKey& key)
{
	if (key == ControlKey::KEY_NONE)
		return ("NONE");
	else if (key == ControlKey::MOUSE_LEFT)
		return ("LEFT");
	else if (key == ControlKey::MOUSE_RIGHT)
		return ("RIGHT");
	else if (key == ControlKey::MOUSE_SCROLL)
		return ("SCROLL");
	else if (key == ControlKey::MOUSE_MOTION)
		return ("MOTION");
	return ("UNKNOWN");
}

std::string Event::ToString() const
{
	std::stringstream ss;

	ss << "Hash (" << GetHash() << ") Type (" << EventTypeToString(type) << ")";
	if (type == Event::Type::KEY)
		ss << " (" << (unsigned char)key << ") (" << ControlKeyStateToString(state) << ")";
	else if (type == Event::Type::MOUSE)
	{
		ss << " (" << ControlKeyMouseToString(key) << ") (" << ControlKeyStateToString(state) << ")";
	}
	else if (type == Event::Type::WINDOW)
	{
		ss << "(" << idDevice << ") (" << ControlKeyWindowToString(key) << ")";
	}
	ss << std::endl;
	return (ss.str());
}

Event::Event(const Type & _type, const ControlKey & _key, const ControlKeyState & _state) : type(_type), key(_key), state(_state)
{
	idDevice = 0;
}

Event::Event()
{
	idDevice = 0;
	type = Event::Type::NONE;
	state = ControlKeyState::NONE;
}

Event::Type Event::GetTypeFromControlKey(const ControlKey & key)
{
	if (key > ControlKey::KEY_START && key < ControlKey::KEY_END)
		return (Event::Type::KEY);
	else if (key > ControlKey::MOUSE_START && key < ControlKey::MOUSE_END)
		return (Event::Type::MOUSE);
	else if (key > ControlKey::WINDOW_START && key < ControlKey::WINDOW_END)
		return (Event::Type::WINDOW);
	else
		return (Event::Type::NONE);
}

ControlKeyState Event::GetDefaultControlKeyStateFromControlKey(const ControlKey & key)
{
	if (key > ControlKey::KEY_START && key < ControlKey::KEY_END)
		return (ControlKeyState::PRESSED);
	else if (key > ControlKey::MOUSE_START && key < ControlKey::MOUSE_END)
		return (ControlKeyState::PRESSED);
	else if (key > ControlKey::WINDOW_START && key < ControlKey::WINDOW_END)
		return (ControlKeyState::NONE);
	return (ControlKeyState::NONE);
}

uint64_t Event::GetHash() const
{
	uint64_t hash;

	hash = (uint64_t)idDevice;
	hash << 32;
	hash += type;
	hash += key << 8;
	hash += state << 24;
	
	return (hash);
}
