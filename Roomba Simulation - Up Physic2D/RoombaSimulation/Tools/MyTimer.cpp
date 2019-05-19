#include "MyTimer.h"

Timer::Timer() : _time(std::clock())
{
	_elapsedTime = 1;
	_elapsedFTime = 1;
}

Timer::~Timer()
{
}

void Timer::Reset()
{
	_elapsedTime = 1;
	_elapsedFTime = 1;
	_time = std::clock();
}

void Timer::SetElapsedTime()
{
	const std::clock_t tmp = std::clock();
	_elapsedFTime = float(tmp - _time);
	_elapsedTime = (clock_t)_elapsedFTime;
	_time = tmp;
}

float Timer::GetElapsedSecond()
{
	return (_elapsedFTime / (float)CLOCKS_PER_SEC);
}

float Timer::GetElapsedMilliSecond()
{
	return (_elapsedFTime / ((float)CLOCKS_PER_SEC * 1000.0f));
}
