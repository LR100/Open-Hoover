#include "FPSComputer.h"

FpsComputer::FpsComputer()
{
	_frame = 1;
	_passedTime = 0;
}

FpsComputer::~FpsComputer()
{

}

void FpsComputer::AddFrame()
{
	_frame++;
}

bool FpsComputer::ShowFps()
{
	_timer.SetElapsedTime();
	_passedTime += _timer.GetElapsedSecond();
	if (_passedTime > 1.0f)
	{
		_fps = (_frame / _passedTime);
		std::cout << "Fps: (" << _fps << ")\n";
		_frame = 1;
		_passedTime = 0;
		return (true);
	}
	return (false);
}
