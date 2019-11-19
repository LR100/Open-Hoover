#pragma once

#define MOVEMENTCONTROLLER_NBMOVE 5

enum MovementType : uint8_t
{
	STOP = 0,
	FORWARD = 1,
	BACKWARD = 2,
	ROTATE_LEFT = 3,
	ROTATE_RIGHT = 4
};

class MovementController
{
public:

	MovementController()
	{
		_moveIsFinished = true;
		_movementType = MovementType::STOP;
		_initFunction[STOP] = &MovementController::Stop;
		_initFunction[FORWARD] = &MovementController::Forward;
		_initFunction[BACKWARD] = &MovementController::Backward;
		_initFunction[ROTATE_LEFT] = &MovementController::RotateLeft;
		_initFunction[ROTATE_RIGHT] = &MovementController::RotateRight;
	}

	const MovementType& GetMovementType() const { return (_movementType); };
	const float& GetSpeedLinear() const { return (_speedLinear); };
	const float& GetSpeedAngular() const { return (_speedAngular); };
	const bool& IsMoveFinished() const { return (_moveIsFinished); };


	void				  Move(MovementType movementType, float unit, bool force = true)
	{
		if (_moveIsFinished || force)
		{
			_moveIsFinished = false;
			_movementType = movementType;
			_timeMoved = 0;
			_timeToMove = 0;
			(this->*_initFunction[_movementType])(unit);
		}
	}

	void        Update(const float& dtMs)
	{
		_timeMoved += dtMs;
		// std::cout << "MovementController Update TimeMoved: (" << _timeMoved << ") TimeToMove: (" << _timeToMove << ")" << std::endl;
		if (_timeMoved >= _timeToMove)
		{
			_timeToMove = 0;
			_timeMoved = 0;
			_movementType = STOP;
			_moveIsFinished = true;
			Stop(0); // Unlimited Stop
		}
	}

private:

	void        Forward(uint16_t unit)
	{
		_timeToMove = ((float)unit / _speedLinear) * 1000.f; // to MS
	}
	void        Backward(uint16_t unit)
	{
		_timeToMove = ((float)unit / _speedLinear) * 1000.f; // to MS
	}
	void        RotateLeft(uint16_t unit)
	{
		SetRotateUnitTime(unit);
	}
	void        RotateRight(uint16_t unit)
	{
		SetRotateUnitTime(unit);
	}

	void        Stop(uint16_t unit)
	{
		_timeToMove = (float)unit;
		_moveIsFinished = true;
	}

	void SetRotateUnitTime(uint16_t unit)
	{
		float modSpeedAngular = _speedAngular; // More unit is low -> Slower it is
		if (unit < 45) {
			modSpeedAngular = (modSpeedAngular / (70.0f / (25.0f + (float)unit))); // From 3Times slower speed to -> 1 (from 0 to 10degree Rotation)
		}
		_timeToMove = ((float)unit / modSpeedAngular) * 1000.f; // to MS
	}

	typedef void (MovementController::* MoveFunction)(uint16_t);
	MoveFunction    _initFunction[MOVEMENTCONTROLLER_NBMOVE];

	float			_speedAngular = 242.0f; // Known From Real Calibration
	float			_speedLinear = 100.0f;

	bool			_moveIsFinished;
	MovementType	_movementType;
	float			_timeToMove;
	float			_timeMoved;
};