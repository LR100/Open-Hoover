#include "IEventHandler.h"

IEventHandler::IEventHandler()
{
	_curPriority = Priority::MEDIUM;
}

void IEventHandler::SetPriority(Priority priority)
{
	_curPriority = priority;
}

void IEventHandler::Clear()
{
	_handlers.clear();
	_handlers = _handlersPermanent;
}

const Mouse * IEventHandler::GetMouse() const
{
	return (&_mouse);
}

#include <iostream> // TMP

void IEventHandler::HandleEvent(EVENT_TYPE_HASH_ idevent)
{
	if (_handlers.count(idevent))
	{
		WorkList*	workList = &_handlers.at(idevent);
		// Do Work
		for (size_t i = 0; i < workList->_works.size(); i += 1)
		{
			//std::cout << "HandleEvent (" << idevent << ")" << std::endl;
			workList->_works.at(i)._function();
			
		}
	}
}

IEventHandler::Work::Work(std::function<void()> f, Priority priority) : _function(f), _priority(priority)
{
}
