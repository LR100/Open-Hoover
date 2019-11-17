#pragma once

#include <stdlib.h>
#include <string>
#include <ctime>

class TimeDateFormatter
{
public:
	static std::string GetFormatCurrentTime(const char * format);
};