#pragma once

#include "TimeDateFormatter.h"

#define BUFFER_TIME 50

std::string TimeDateFormatter::GetFormatCurrentTime(const char * format)
{
	char sTime[50];
	time_t CurrentTime = time(NULL);
	strftime(sTime, sizeof(sTime), format, localtime(&CurrentTime));
	return sTime;
}