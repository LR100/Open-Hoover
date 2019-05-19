#ifndef __RobovacDefines_H__
#define __RobovacDefines_H__

#ifdef _WIN32
#include <stdint.h>
#else

#endif

enum MoveType
{
	STOP = 0,
	FORWARD = 1,
	BACKWARD = 2,
	ROTATE_LEFT = 3,
	ROTATE_RIGHT = 4
};

struct RobovacCalibrateInfo
{

};

#endif // __RobovacDefines_H__