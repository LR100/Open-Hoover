#include "ConsoleParser.h"

#include <iostream>

ConsoleParser::ConsoleParser()
{
	_thread = NULL;
	_running = false;
}

ConsoleParser::~ConsoleParser()
{
	_running = false;
	if (_thread != NULL)
	{
		delete (_thread);
		_thread = NULL;
	}
}

void ConsoleParser::Launch()
{
	if (_thread == NULL)
	{
		_running = true;
		_thread = new std::thread(&ConsoleParser::GetEntry, this);
		_thread->detach();
	}
}

void ConsoleParser::Stop()
{
	_running = false;
	if (_thread != NULL)
	{
		delete (_thread);
		_thread = NULL;
	}
}

void ConsoleParser::ParseEntry()
{
	if (_actions.count(_entry))
	{
		_actions.at(_entry)._action();
	}
}

void ConsoleParser::GetEntry()
{
	while (_running)
	{
		// Could be upgraded with non blocking read
		std::cin >> _entry;
		ParseEntry();
	}
}

