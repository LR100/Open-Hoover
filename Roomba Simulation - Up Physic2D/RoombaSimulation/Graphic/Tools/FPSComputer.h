#ifndef _FPSCOMPUTER_H_
#define _FPSCOMPUTER_H_

#include <iostream> // To Show Fps
#include "../../Tools/MyTimer.h" // Require MyTimer

class FpsComputer
{
public:
	FpsComputer();
	~FpsComputer();

	void	AddFrame();
	// Show FPS every 1 second and return true if it's the case
	bool	ShowFps();
private:

	float	_passedTime;
	Timer	_timer;
	float	_frame;
	float	_fps;
};

#endif /* !_FPSCOMPUTER_H_ */
