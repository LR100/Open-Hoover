#include "RobovacControl.h"


Robovac::FindPathInfo::FindPathInfo()
{
	_pathFov = 100;
	//					//  // This Number ! is the PRECISION
	_angleRotate = (_pathFov / 20); // Must Have Value with no decimals
	_stepsMax = (360 / _angleRotate); // Must Have Value with no decimals
	_distances = new uint16_t[_stepsMax];
	_maxDist = 40000; // 40 meters 
	Reset();
}

void Robovac::FindPathInfo::Reset()
{
	_pathFov = 100;
	_pathFound = false;
	_step = 0;
	_stepMiddle = 0;
	_pathAngle = 0;
}

void Robovac::FindPathInfo::Set(uint16_t minDist)
{
	Reset();
	_minDist = minDist;
}

bool Robovac::FindPathInfo::PushDistance(uint16_t distance)
{
	_step += 1;
	if (_step == _stepsMax)
	{
		if (SearchDesperatePath()) {
			return (true);
		}
		return (false);
	}
	_distances[_step] = distance;
	SearchPath();
	return (true);
}

bool Robovac::FindPathInfo::SearchDesperatePath()
{
	_minDist = 30;
	_pathFov = 80;
	if (SearchPath())
		return (true);
	return (false);
}

bool Robovac::FindPathInfo::SearchPath()
{
	uint8_t	stepsToValid = _pathFov / _angleRotate;
	uint8_t	stepsValid = 0;
	if (_step < stepsToValid) { // NO way to find a path with so few measures
		return (false);
	}
	for (uint8_t s = 0; s < _step; s += 1) // Check All steps
	{
		if (_distances[s] > _minDist && _distances[s] < _maxDist) {
			stepsValid += 1;
			if (stepsValid == stepsToValid)
			{

				//std::cout << "OK Steps To Valid (" << (int)stepsToValid << ")" << std::endl;
				uint8_t stepsToComeBack = (_step - s) + (stepsToValid / 2);
				_stepMiddle = (_step - stepsToComeBack);
				//std::cout << "Steps TO ComeBack" << (int)stepsToComeBack << std::endl;
				_pathAngle = (uint16_t)((float)_angleRotate * (float)stepsToComeBack);
				//std::cout << "Path Angle:" << (int)_pathAngle << std::endl;
				_pathFound = true;
				return (true);
			}
		}
		else {
			stepsValid = 0; // Unvalid Step - Restart from 0
		}
	}
}

uint16_t Robovac::FindPathInfo::GetPathAngleFromStart() const
{
	return ((_step * _angleRotate) - _pathAngle);
}

uint16_t Robovac::FindPathInfo::GetShortestDistanceInFov(const uint8_t& radius)
{
	uint8_t	stepsToValidHalf = (uint8_t)(((float)(_pathFov / 2) / _angleRotate) / 2.0f);
	uint8_t	stepsEnd = (_stepMiddle + stepsToValidHalf);
	uint16_t distanceMin = UINT16_MAX;
	uint8_t s = (_stepMiddle - stepsToValidHalf);
	std::cout << "Step Middle (" << (int)_stepMiddle << ")" << std::endl;
	for (; s < stepsEnd; s += 1)
	{
		std::cout << "_distances[" << (int)s << "]=" << _distances[s] << std::endl;
		if (_distances[s] < distanceMin)
		{
			distanceMin = _distances[s];
		}
	}
	std::cout << "Distance MIn: (" << distanceMin << ")" << std::endl;
	return (distanceMin);
}
