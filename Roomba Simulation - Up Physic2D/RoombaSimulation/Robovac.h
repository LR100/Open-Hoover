
#ifdef _WIN32 // ON WINDOWS

#pragma once
#define WIN

#include "Maths/Vector.h"
#include <stdint.h>

#endif
// This class is Used in arduino !!!
// Should avoid all std functions or only for debugging

// Features:
// - Can Move Straight
// - Can Rotate (left or right)
// - Can Detect Distances
// - Depend of Time

// Processed Features:
// - Mapping of terrain (low low low memory and maybee should use rom/eeprom
// - PathFinding
// - Combine with clean detection ?

// Motor With simple action // No Intensity Control
class Motor
{
public:
	Motor(int pinP, int pinM)
	{
		_pinP = pinP;
		_pinM = pinM;
	}

	void Forward()
	{
#ifndef WIN
		digitalWrite(_pinP, LOW);
		digitalWrite(_pinM, HIGH);
#endif // !WIN
	}
	void Backward()
	{
#ifndef WIN
		digitalWrite(_pinP, HIGH);
		digitalWrite(_pinM, LOW);
#endif // !WIN
	}
	void Stop()
	{
#ifndef WIN
		digitalWrite(_pinP, LOW);
		digitalWrite(_pinM, LOW);
#endif // !WIN
	}

private:
	// P for Plus and M For Minus
	int _pinP, _pinM;
};

class DistanceSensor
{
public:

	DistanceSensor()
	{
		_distance = 0;
		_distancePrev = 0;
	}
	virtual ~DistanceSensor() {};

	virtual bool    Update(unsigned long dtMs) = 0;

	virtual void    SetDistance() = 0;
	const unsigned int& GetDistance() const { return (_distance); };
	const unsigned int& GetDistancePrev() const { return (_distancePrev); };

protected:
	// Distance in Cm
	unsigned int   _distance;
	unsigned int   _distancePrev;
};

#ifndef WIN

#include <NewPing.h>


#endif // !WIN

#define ULTRASONIC_MAX_DISTANCE 200 // In CM 
#define ULTRASONIC_MIN_TIME_REFRESH 33 // IN MS

class DistanceSensorUltrasonic : public DistanceSensor
{
public:

#ifndef WIN
	DistanceSensorUltrasonic(int pinP, int pinM) : _ultraSonic(pinP, pinM, ULTRASONIC_MAX_DISTANCE)
	{
		_pinP = pinP;
		_pinM = pinM;
	}
#else // !WIN
	DistanceSensorUltrasonic(int pinP, int pinM)
	{
		_pinP = pinP;
		_pinM = pinM;
	}
#endif

	virtual void  SetDistance() override
	{
		_distancePrev = _distance;
#ifndef WIN
		_distance = _ultraSonic.convert_cm(_ultraSonic.ping_median(3, ULTRASONIC_MAX_DISTANCE));
		if (_distance == NO_ECHO)
			_distance = ULTRASONIC_MAX_DISTANCE;
#endif
	}

	virtual bool  Update(unsigned long dtMs) override
	{
		_timePassed += dtMs;
		if (_timePassed >= ULTRASONIC_MIN_TIME_REFRESH)
		{
			_timePassed = 0;
			SetDistance();
			return (true);
		}
		return (false);
	}

private:
	int _pinP, _pinM;
#ifndef WIN


	NewPing _ultraSonic;

#endif // !WIN


	unsigned long   _timePassed; // also In Ms
};

#define MOTOR_LEFT_P 9
#define MOTOR_LEFT_M 8
#define MOTOR_RIGHT_P 11
#define MOTOR_RIGHT_M 10

class MovementControler
{
public:
#define MOVEMENTCONTROLLER_NBMOVE 5
	enum MovementType
	{
		STOP = 0,
		FORWARD = 1,
		BACKWARD = 2,
		ROTATE_LEFT = 3,
		ROTATE_RIGHT = 4
	};

	MovementControler() : _motorLeft(MOTOR_LEFT_P, MOTOR_LEFT_M), _motorRight(MOTOR_RIGHT_P, MOTOR_RIGHT_M)
	{
		_moveIsFinished = false,
			_moveType = MovementType::STOP;
		_speedLinear = 10.0f;
		_speedAngular = 20.0f; // 20° per second
		 // Movement is in 2D (The Ground) 
		// No management of highness (For the robot everything is Flat !)
		_initFunction[STOP] = &MovementControler::Stop;
		_initFunction[FORWARD] = &MovementControler::Forward;
		_initFunction[BACKWARD] = &MovementControler::Backward;
		_initFunction[ROTATE_LEFT] = &MovementControler::RotateLeft;
		_initFunction[ROTATE_RIGHT] = &MovementControler::RotateRight;
	}

	void  SetSpeedLinear(const float& speedLinear) { _speedLinear = speedLinear; };
	void  SetSpeedAngular(const float& speedAngular) { _speedAngular = speedAngular; };


	// Unit could be an Angle (in degree) or a Distance (in cm)
	void        Move(MovementType moveType, float unit, bool force = false)
	{
		if (_moveIsFinished || force)
		{
			_moveIsFinished = false;
			_moveType = moveType;
			_timePassedForCurrentMove = 0;
			_timeForCurrentMove = 0;
			(this->*_initFunction[_moveType])(unit);
		}
	}

	void        Update(const float& dtMs)
	{
		_timePassedForCurrentMove += dtMs;
		if (_timePassedForCurrentMove >= _timeForCurrentMove)
		{
			_timeForCurrentMove = 0;
			_timePassedForCurrentMove = 0;
			_moveType = STOP;
			_moveIsFinished = true;
			Stop(0); // Unlimited Stop
		}
	}

	const MovementType&   GetMovementType() const { return (_moveType); };
	const float&    GetSpeedLinear() const { return (_speedLinear); };
	const float&    GetSpeedAngular() const { return (_speedAngular); };
	const bool&     IsMoveFinished() const { return (_moveIsFinished); };

private:

	void        Forward(float unit)
	{
		_timeForCurrentMove = (unit / _speedLinear) * 1000.f; // to MS
		_motorLeft.Forward();
		_motorRight.Forward();
	}
	void        Backward(float unit)
	{
		_timeForCurrentMove = (unit / _speedLinear) * 1000.f; // to MS
		_motorLeft.Backward();
		_motorRight.Backward();
	}
	void        RotateLeft(float unit)
	{
		//Serial.print("RotateLeft");
		_timeForCurrentMove = (unit / _speedAngular) * 1000.f; // to MS
		_motorLeft.Forward();
		_motorRight.Backward();
	}
	void        RotateRight(float unit)
	{
		_timeForCurrentMove = (unit / _speedAngular) * 1000.f; // to MS
		_motorLeft.Backward();
		_motorRight.Forward();
	}

	void        Stop(float unit)
	{
		_timeForCurrentMove = unit;
		_motorLeft.Stop();
		_motorRight.Stop();
	}

	typedef void (MovementControler::*MoveFunction)(float);
	MoveFunction    _initFunction[MOVEMENTCONTROLLER_NBMOVE];

	bool        _moveIsFinished;

	float       _speedAngular; // in degree/s
	float       _speedLinear; // in cm/s Calculated from CALIBRATION
	// Precision max is in cm so could use an in to get better performance

	unsigned int    _timeForCurrentMove; // in Ms
	unsigned int    _timePassedForCurrentMove; // also In Ms

	Vec2        _pos;
	Vec2        _dir;

	Motor       _motorLeft;
	Motor       _motorRight;
	MovementType	_moveType;
};


#define ULTRASONIC_FONT_PIN_P 2
#define ULTRASONIC_FRONT_PIN_M 3

// !!!!!!!!!! ALL distance are in cm
class Robovac
{
public:
	Robovac()
	{
		_movementController.Move(MovementControler::MovementType::FORWARD, 20);
		_frontDistanceSensor = new DistanceSensorUltrasonic(ULTRASONIC_FONT_PIN_P, ULTRASONIC_FRONT_PIN_M);
#ifndef WIN
#define RELAY_ALIM_PIN 4
		pinMode(RELAY_ALIM_PIN, OUTPUT);
		digitalWrite(RELAY_ALIM_PIN, HIGH);
#endif
		_movementController.Move(MovementControler::MovementType::STOP, 10, true);
	}
	~Robovac()
	{

	}

	void Calibrate()
	{
		CalibrateAngularSpeed();
		// Then From Angular speed and around map get linear speed by good direction
		_frontDistanceSensor->SetDistance();
		unsigned int distance = _frontDistanceSensor->GetDistance();

		unsigned long timeStart = 0;
		unsigned long timeElapsed = 0;
		unsigned long timeCurrent = 0;
#ifndef WIN
		timeStart = millis();
		Serial.print("Distance:");
		Serial.print(distance);
		Serial.print("Cm\n");
#endif
		// Case Is Too Far From
		while (distance >= ULTRASONIC_MAX_DISTANCE)
		{
			_movementController.Move(MovementControler::MovementType::FORWARD, 10);
#ifndef WIN
			timeCurrent = millis();
#endif
			timeElapsed = timeCurrent - timeStart;
			timeStart = timeCurrent;
			if (_frontDistanceSensor->Update(timeElapsed))
			{
				distance = _frontDistanceSensor->GetDistance();
				if (distance == _frontDistanceSensor->GetDistancePrev()) // Is Stuck ! Try Random Rotate
				{
					if (distance % 2 == 0)
						_movementController.Move(MovementControler::MovementType::ROTATE_LEFT, 40, true);
					else
						_movementController.Move(MovementControler::MovementType::ROTATE_RIGHT, 40, true);
				}
#ifndef WIN
				Serial.print("Distance:");
				Serial.print(distance);
				Serial.print("Cm\n");
#endif
			}
		}
		_movementController.Move(MovementControler::MovementType::STOP, 10, true);


		bool isCalibrated = false;
		while (!isCalibrated)
		{
			_frontDistanceSensor->SetDistance();
			unsigned int distance = _frontDistanceSensor->GetDistance();

#ifndef WIN
			timeCurrent = millis();
#endif
			timeElapsed = timeCurrent - timeStart;
			timeStart = timeCurrent;

		}
	}

	// Update Robovac state with Delta T (in milliseconds)
	void Update(unsigned int dtMs)
	{
		_dtMs = (float)dtMs;
		_movementController.Update(_dtMs);
		// For FUN

	}

private:

#define ROTATE_CALIBRATE_COUNT_MAX 100
#define ROTATE_CALIBRATE_DELAY 36 // In Ms 

	void          CalibrateAngularSpeed()
	{
#ifndef WIN

		Serial.print("CalibrateAngularSpeed\n");
#endif
		unsigned int    distances[ROTATE_CALIBRATE_COUNT_MAX];
		float       angleSupposed = 0;
		float       rcdf = (float)ROTATE_CALIBRATE_DELAY;

		for (unsigned int i = 0; i < ROTATE_CALIBRATE_COUNT_MAX; i += 1)
		{
#ifndef WIN
			delay(ROTATE_CALIBRATE_DELAY);

#endif

			_movementController.Move(MovementControler::MovementType::STOP, 100, true);
			angleSupposed += rcdf * _movementController.GetSpeedAngular();
			_frontDistanceSensor->SetDistance();
			distances[i] = _frontDistanceSensor->GetDistance();
			_movementController.Move(MovementControler::MovementType::ROTATE_RIGHT, 100, true);
		}
		_movementController.Move(MovementControler::MovementType::STOP, 100, true);
#ifndef WIN
		for (unsigned int i = 0; i < ROTATE_CALIBRATE_COUNT_MAX; i += 1)
		{
			Serial.print("Distance:");
			Serial.print(distances[i]);
			Serial.print("Cm\n");
		}
#endif

	}

	DistanceSensor*     _frontDistanceSensor;
	MovementControler   _movementController;
	float         _dtMs;
};


