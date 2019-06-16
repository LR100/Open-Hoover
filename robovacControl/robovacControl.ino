#ifdef _WIN32 // ON WINDOWS

#define WIN

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

template <typename T>
struct Vec2T
{
	Vec2T(T _x, T _y);
	Vec2T();

	virtual ~Vec2T() {};

	Vec2T<T>  operator-() const;
	Vec2T<T>& operator-=(const Vec2T<T>& other);
	Vec2T<T>& operator-=(const T& value);
	Vec2T<T>& operator+=(const Vec2T<T>& other);
	Vec2T<T>& operator+=(const T& value);

	Vec2T<T>& operator*=(const T& value);
	Vec2T<T>& operator/=(const T& value);

	// CONST Op
	bool    operator==(const Vec2T<T>& other) const;

	Vec2T<T>  operator-(const Vec2T& other) const;
	Vec2T<T>  operator+(const Vec2T<T>& other) const;

	Vec2T<T>  operator*(const T& value) const;
	Vec2T<T>  operator*(const Vec2T& other) const;

	Vec2T<T>  operator/(const T& value) const;

	T     Dot(const Vec2T<T>& other) const;
	T     Cross(const Vec2T<T>& other) const;
	void    Zero();


	T     x, y;
};


///////////
// VEC 2 //
///////////

template <typename T>
Vec2T<T>::Vec2T(T _x, T _y)
{
	x = _x;
	y = _y;
}

template <typename T>
Vec2T<T>::Vec2T()
{
	x = 0;
	y = 0;
}

template <typename T>
Vec2T<T> Vec2T<T>::operator-() const
{
	return Vec2T<T>(-x, -y);
}

template <typename T>
Vec2T<T> & Vec2T<T>::operator-=(const Vec2T<T> & other)
{
	x -= other.x;
	y -= other.y;
	return (*this);
}

template<typename T>
Vec2T<T>& Vec2T<T>::operator-=(const T & value)
{
	x -= value;
	y -= value;
	return (*this);
}

template <typename T>
Vec2T<T> & Vec2T<T>::operator+=(const Vec2T<T> & other)
{
	x += other.x;
	y += other.y;
	return (*this);
}

template<typename T>
Vec2T<T>& Vec2T<T>::operator+=(const T & value)
{
	x += value;
	y += value;
	return (*this);
}

template <typename T>
Vec2T<T> & Vec2T<T>::operator*=(const T & value)
{
	x *= value;
	y *= value;
	return (*this);
}

template<typename T>
Vec2T<T>& Vec2T<T>::operator/=(const T & value)
{
	x /= value;
	y /= value;
	return (*this);
}

template <typename T>
bool Vec2T<T>::operator==(const Vec2T<T> & other) const
{
	return (x == other.x && y == other.y);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator-(const Vec2T & other) const
{
	return Vec2T<T>(x - other.x, y - other.y);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator+(const Vec2T<T>& other) const
{
	return Vec2T<T>(x + other.x, y + other.y);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator*(const T& value) const
{
	return Vec2T<T>(x * value, y * value);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator*(const Vec2T & other) const
{
	return Vec2T<T>(x * other.x, y * other.y);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator/(const T& value) const
{
	return Vec2T<T>(x / value, y / value);
}

template<typename T>
void Vec2T<T>::Zero()
{
	x = 0;
	y = 0;
}

template<typename T>
T Vec2T<T>::Dot(const Vec2T<T>& other) const
{
	return T((x * other.x) + (y * other.y));
}

template<typename T>
T Vec2T<T>::Cross(const Vec2T<T>& other) const
{
	return T((x * other.y) - (y * other.x));
}

// Float 
template class Vec2T<float>;
typedef Vec2T<float> Vec2;
// Int
template class Vec2T<int>;
typedef Vec2T<int> Vec2i;

///////////////////////////////////////////////
/////////////////// ROBOVAC ///////////////////
///////////////////////////////////////////////
				  ///////////
					//////

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


#include <RobovacSC.h>

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

#define ULTRASONIC_MAX_DISTANCE 250 // In CM 
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

#define MOTOR_LEFT_P 28
#define MOTOR_LEFT_M 26
#define MOTOR_RIGHT_P 22
#define MOTOR_RIGHT_M 24

#include <RobovacDefines.h>

class MovementController
{
public:
#define MOVEMENTCONTROLLER_NBMOVE 5

	MovementController() : _motorLeft(MOTOR_LEFT_P, MOTOR_LEFT_M), _motorRight(MOTOR_RIGHT_P, MOTOR_RIGHT_M)
	{
		_moveIsFinished = true;
		_movementType = MovementType::STOP;
		_speedLinear = 10.0f;
		_speedAngular = 20.0f; // 20° per second
		 // Movement is in 2D (The Ground) 
		// No management of highness (For the robot everything is Flat !)
		_initFunction[STOP] = &MovementController::Stop;
		_initFunction[FORWARD] = &MovementController::Forward;
		_initFunction[BACKWARD] = &MovementController::Backward;
		_initFunction[ROTATE_LEFT] = &MovementController::RotateLeft;
		_initFunction[ROTATE_RIGHT] = &MovementController::RotateRight;
	}

	void  SetSpeedLinear(const float& speedLinear) { _speedLinear = speedLinear; };
	void  SetSpeedAngular(const float& speedAngular) { _speedAngular = speedAngular; };


	// Unit could be an Angle (in degree) or a Distance (in cm)
	void        Move(MovementType movementType, uint16_t unit, bool force = false)
	{
		if (_moveIsFinished || force)
		{
      Serial << "Movement Controller Move\n";
			_moveIsFinished = false;
			_movementType = movementType;
			_timeMoved = 0;
			_timeToMove = 0;
			(this->*_initFunction[_movementType])(unit);
		}
	}

 void        MoveNow(MovementType movementType, uint16_t unit)
 {
    Move(movementType, unit, true);
    while (!_moveIsFinished)
    {
      delay(5);
      Update(5);
    }
  }

  void        MoveNowDuring(MovementType movementType, uint16_t timeMs)
  {
     Move(movementType, 1000, true);
     uint16_t timeMoved = 0;
     
      while (timeMoved < timeMs)
      {
        delay(5);
        Update(5);
        timeMoved += 5;
      }
      Move(MovementType::STOP, 100, true); 
  }
  

	void        Update(const float& dtMs)
	{
		_timeMoved += dtMs;
		if (_timeMoved >= _timeToMove)
		{
			_timeToMove = 0;
			_timeMoved = 0;
			_movementType = STOP;
			_moveIsFinished = true;
			Stop(0); // Unlimited Stop
		}
	}

	const MovementType&   GetMovementType() const { return (_movementType); };
	const float&          GetSpeedLinear() const { return (_speedLinear); };
	const float&          GetSpeedAngular() const { return (_speedAngular); };
	const bool&           IsMoveFinished() const { return (_moveIsFinished); };

private:

	void        Forward(uint16_t unit)
	{
		_timeToMove = ((float)unit / _speedLinear) * 1000.f; // to MS
		_motorLeft.Forward();
		_motorRight.Forward();
	}
	void        Backward(uint16_t unit)
	{
		_timeToMove = ((float)unit / _speedLinear) * 1000.f; // to MS
		_motorLeft.Backward();
		_motorRight.Backward();
	}
	void        RotateLeft(uint16_t unit)
	{
		SetRotateUnitTime(unit);
		_motorLeft.Forward();
		_motorRight.Backward();
	}
	void        RotateRight(uint16_t unit)
	{
		SetRotateUnitTime(unit);
		_motorLeft.Backward();
		_motorRight.Forward();
	}

	void        Stop(uint16_t unit)
	{
		_timeToMove = (float)unit;
		_motorLeft.Stop();
		_motorRight.Stop();
    _moveIsFinished = true; // Stop -> Doing Nothing
	}

  // Correct Time with inertia if small movement
  void SetRotateUnitTime(uint16_t unit) 
  {
    float modSpeedAngular = _speedAngular; // More unit is low -> Slower it is
    if (unit < 45) {
       modSpeedAngular = (modSpeedAngular / (70.0f / (25.0f + (float)unit))); // From 3Times slower speed to -> 1 (from 0 to 10degree Rotation)
    }
    _timeToMove = ((float)unit / modSpeedAngular) * 1000.f; // to MS
  }

	typedef void (MovementController::*MoveFunction)(uint16_t);
	MoveFunction    _initFunction[MOVEMENTCONTROLLER_NBMOVE];

	bool        _moveIsFinished;

	float       _speedAngular; // in degree/s
	float       _speedLinear; // in cm/s Calculated from CALIBRATION
	// Precision max is in cm so could use an in to get better performance

	unsigned int    _timeToMove; // in Ms
	unsigned int    _timeMoved; // also In Ms

	Vec2        _pos;
	Vec2        _dir;

	Motor             _motorLeft;
	Motor             _motorRight;
	MovementType      _movementType;
  
};


#define ULTRASONIC_FRONT_PIN_P 2
#define ULTRASONIC_FRONT_PIN_M 3

#ifdef _WIN32
#include <stdint.h>
#else

#endif


#define RF24_PIN_CE 7
#define RF24_PIN_CS 8


// !!!!!!!!!! ALL distance are in cm
class Robovac : public RobovacSC
{
public:
  typedef enum RobovacState : uint8_t
  {
    REST = 0,
    CALIBRATE = 1,
    MAP = 2,
    NAVIGATE,
    WORK,
    BLOCKED,
  } RobovacState;

	Robovac() : RobovacSC(RF24_PIN_CE, RF24_PIN_CS)
	{
    _movementController.SetSpeedAngular(240.0f); // Value Is Known from precedent Calibrate Call 
    _movementController.SetSpeedLinear(100.0f); // Value Is Known from precedent Calibrate Call 
		//_movementController.Move(MovementType::FORWARD, 20);
    _dtMsVerification = 0;
    _robotState = REST;
    _robotDiameter = 30; // In Cm - Must be Set Manually for the moment - Could be self calibrate
		_frontDistanceSensor = new DistanceSensorUltrasonic(ULTRASONIC_FRONT_PIN_P, ULTRASONIC_FRONT_PIN_M);
#ifndef WIN

#define RELAY_ALIM_PIN 4
		pinMode(RELAY_ALIM_PIN, OUTPUT);
		digitalWrite(RELAY_ALIM_PIN, HIGH);
#endif
		_movementController.Move(MovementType::STOP, 1000, true);
	}
	~Robovac()
	{

	}
	void Initialize() {
    InitCommunication();
	}

	// Operate Robovac state with Delta T (in milliseconds)
	void Operate(unsigned long dtMs)
	{
		_dtMs = (float)dtMs;
    UpdateCommunication(); // Update Client
    UpdateFromState();
		_movementController.Update(_dtMs);
	}

private:

#define DISTANCE_TOO_CLOSE 10

  void UpdateFromState()
  {
    if (_robotState == CALIBRATE){
      _robotState = REST; // At End OF Calibrate -> Robot Is Resting
      _dtMs = 0; // Was calibrating _dtMs is reset
      // Demo();
    } else if (_robotState == MAP) {
      
    } else {
      SendMsg("I'm Resting");
    }
    _dtMsVerification += _dtMs; // If Verification is needed
    if (_dtMsVerification > 200)
    {
      _frontDistanceSensor->SetDistance();
      if (_frontDistanceSensor->GetDistance() < DISTANCE_TOO_CLOSE) // TOO Close !! -> STOP
      {
          ActionTooClose();
      } 
      _frontDistanceLast = _frontDistanceSensor->GetDistance();
    }
  }

  void ActionTooClose()
  {
    _movementController.Move(MovementType::STOP, 10, true); // Instantly Stop
    _movementController.Move(MovementType::BACKWARD, 5, true); // A Few Backward
    FindNavigation();
  }

  void FindNavigation()
  { 
      
  }

  void Demo()
  {
     _movementController.MoveNow(MovementType::ROTATE_LEFT, 45); // Notify That Robot is Calibrate
      delay(2000);
      _movementController.MoveNow(MovementType::ROTATE_RIGHT, 45);
      delay(2000);
      _movementController.MoveNow(MovementType::ROTATE_LEFT, 90); // Notify That Robot is Well Calibrate
      delay(2000);
      _movementController.MoveNow(MovementType::ROTATE_RIGHT, 90);
      delay(2000);
       _movementController.MoveNow(MovementType::ROTATE_LEFT, 180); // Notify That Robot is Well Calibrate
      delay(2000);
      _movementController.MoveNow(MovementType::ROTATE_RIGHT, 180);
      delay(2000);
  }

  void MapAngle(uint16_t angleStart, uint16_t angleEnd, uint8_t angleStep) // ex: angleStart: 80° angleEnd: 
  {
    
  }

	virtual void	OnReadCommandCalibrate(RobovacCommand* command) override
	{
    if (_robotState != CALIBRATE) // Already Calibrating
    {
      _robotState = CALIBRATE;
		  Calibrate();
    }
	}

	virtual void	OnReadCommandMovement(RobovacCommandMovement* command) override 
	{
    if (_robotState != CALIBRATE) // Already Calibrating
    {
      _movementController.Move(command->movementType, command->power * 10, true);
    }
	}

  void  RotateFromAngle(float angle)
  {
    if (angle < 0) {
        _movementController.MoveNow(MovementType::ROTATE_LEFT, (uint16_t)(-angle));
    } else {
        _movementController.MoveNow(MovementType::ROTATE_RIGHT, (uint16_t)(angle));
    }
  }

  bool FindFreePath(uint16_t minDist = 20, uint16_t maxDist = ULTRASONIC_MAX_DISTANCE) // Return a angle to Rotate for a free path direction 
  { // Negative Angle -> Rotate Left (anticlockwise) -- Positive Angle -> Rotate Right (clockwise)
    
     // Position Start
     const uint8_t fovToGo = 80;
     const uint8_t steps = 10;
     const uint8_t angleRotate = (fovToGo / steps);
     
     _movementController.MoveNow(MovementType::ROTATE_LEFT, (uint16_t)(fovToGo / 2));
     delay(200);
     uint8_t distOkCount = 0;
     const uint8_t maxSteps = (360 / steps);
     uint16_t distance;
     
     for (uint8_t s = 0; s < maxSteps; s+= 1)
     {
        delay(50);
        _frontDistanceSensor->SetDistance();
        distance = _frontDistanceSensor->GetDistance(); // Get Distance from Sensor
        SendMsg("FreeDist:"+String(distance));
        if (distance > minDist && distance < maxDist)
        {
          _movementController.MoveNow(MovementType::ROTATE_RIGHT, angleRotate);
          distOkCount += 1;
        }
        else 
        {
          distOkCount = 0;
          _movementController.MoveNow(MovementType::ROTATE_RIGHT, 15);
        }
        
        if (distOkCount == (steps - 1))
        {
          SendMsg("I GO");
           _movementController.MoveNow(MovementType::ROTATE_LEFT, (uint16_t)((float)fovToGo / (float)1.7f));
           return (true);
        }
     }
     return (false);
  }

	void Calibrate()
	{
    // Demo();
		// CalibrateAngularSpeed();
    // Find Free Path
    
    CalibrateLinearSpeed(35);
    
	}

	bool markersAreEqual(uint16_t* markersA, uint16_t* markersB, uint8_t nbMarkers, float diffAccept = 0.05, float percentAccept = 0.80f)
	{
		float diffDist = 0;

		uint8_t markerMatchToMatch = (uint8_t)((float)nbMarkers * percentAccept);
		uint8_t markerMatchCount = 0;

		for (uint16_t i = 0; i < nbMarkers; i += 1)
		{
			diffDist = fabsf((float)markersA[i] - (float)markersB[i]);
			if (diffDist < (diffAccept * (float)markersA[i]))
			{
				markerMatchCount += 1;
			}
		}

    //UpdateCommunication();
    // SendMsg("D:"+String(markersB[nbMarkers - 1])+"MMC:" + String(markerMatchCount)+"MMT:"+String(markerMatchToMatch));

		if (markerMatchCount < markerMatchToMatch)
			return (false);

		return (true);
	}

	float CalibrateAngularRotate(uint16_t angularVelocityMin, uint16_t angularVelocityMax, uint16_t markersCount, float angularRotateDesired, float diffAccept = 0.05f, float percentAccept = 0.80f)
	{
		float timeMintoMaxAngularRotate = (angularRotateDesired / (float)angularVelocityMax); // Time Min to reach the Max angular rotation per mesure Desired
		float timeMaxtoMinAngularRotate = (angularRotateDesired / (float)angularVelocityMin); // Time Max to reach the Min angular rotation per mesure Desired
		float timeMaxToReachFullRotate = (360.0f / (float)angularVelocityMin);
		float timeAverageToRotateDesired = ((timeMintoMaxAngularRotate + timeMaxtoMinAngularRotate) / 2.0f); // Still In Seconds
		float timePassed = 0;

		uint16_t  maxMeasure = (720 / angularRotateDesired);
		uint16_t* markers = new uint16_t[markersCount];
		uint16_t* dist = new uint16_t[maxMeasure]; // Fat distance buffers
		uint16_t  measureCount = 0;
		uint16_t  distance;

		float timeStop = timeMaxToReachFullRotate + (timeMaxToReachFullRotate * 0.5f);

    _movementController.Move(MovementType::ROTATE_RIGHT, 400, true);
    delay((int)(timeAverageToRotateDesired * 3000.0f)); // Prepare Motor
    _movementController.Move(MovementType::STOP, 10000, true);
    //SendMsg("Go Calibrate");

    int minGo = 10;
    int markerPosEnd = (markersCount + minGo);
    int waitedRotate = 0;
    int toWaitRotate = (timeAverageToRotateDesired * 1000.0f);
	
		while (timePassed < timeStop && measureCount < maxMeasure) // To avoid overflow in measure
		{
      UpdateCommunication();
      _frontDistanceSensor->SetDistance();
			distance = _frontDistanceSensor->GetDistance(); // Get Distance from Sensor
			dist[measureCount] = distance;
      if (measureCount == markerPosEnd)
      {
        delay(6000); // LET 6 seconds for user to see where markerPosEnd is !!!
      }

			if (measureCount > minGo && measureCount < markerPosEnd)
			{
				markers[measureCount] = distance;
        SendMsg("Mark:"+String(distance)+"CNT:"+String(measureCount));
			}
			else
			{
				if (measureCount > markerPosEnd) // Start to compare Markers and Current Distance
				{
					if (measureCount > (markerPosEnd + 10) && markersAreEqual(markers, &dist[measureCount - markersCount], markersCount, diffAccept, percentAccept))
					{
						delete(markers);
						delete(dist);
            SendMsg("OK CALIB:"+String(360.0f / timePassed));
						return ((360.0f / timePassed));
					}
          timePassed += timeAverageToRotateDesired; // So also start to count time passed to found markers again
				}
			}
     
      delay(100);
			_movementController.Move(MovementType::ROTATE_LEFT, 4000, true);
      waitedRotate = 0;
      while (waitedRotate < toWaitRotate) {
         delay(10);
         waitedRotate += 10;
      }
			_movementController.Move(MovementType::STOP, 10000, true);

			// std::this_thread::sleep_for(std::chrono::milliseconds((int)(timeAverageToRotateDesired * 1000.0f)));

			measureCount += 1;
			
		}

		delete(markers);
		delete(dist);
    SendMsg("BAD CALIB:"+String((((float)angularVelocityMax + (float)angularVelocityMin) / 2.0f)));
    if (_movementController.GetSpeedAngular() > 0)
    {
        return (_movementController.GetSpeedAngular());
    } // Else return Average of supposed AngularSpeed
		return (((float)angularVelocityMax + (float)angularVelocityMin) / 4.0f);
	}

	void          CalibrateAngularSpeed()
	{
     float angularSpeed = CalibrateAngularRotate(90, 360, 40, 4.0f, 0.34, 0.72);
     SendMsg("CALIB:"+String(angularSpeed));
     angularSpeed *= 1.80f; // Considering Inertia Speed - Should also be considered in MovementController
		_movementController.SetSpeedAngular(angularSpeed);
	}

 

 float         GetMeasureLinearSpeed(uint16_t timeMoveMs, bool forward)
 {
    _frontDistanceSensor->SetDistance();
    uint16_t distanceOld = _frontDistanceSensor->GetDistance(); // Get Distance from Sensor
    uint16_t distance = 0;
    uint16_t distanceDiff = 0;

    delay(100);
    if (forward) {
      _movementController.MoveNowDuring(MovementType::FORWARD, timeMoveMs);
    } else {
      _movementController.MoveNowDuring(MovementType::BACKWARD, timeMoveMs);
    }
    // Get New Distance
    _frontDistanceSensor->SetDistance();
     distance = _frontDistanceSensor->GetDistance();
     if (forward) {
        distanceDiff = distanceOld - distance;
     } else {
        distanceDiff = distance - distanceOld;
     }
     return ((float)((float)distanceDiff / ((float)timeMoveMs / 1000.0f)));
 }

 bool   FindGoodCalibrateLinearSpeedPosition(uint16_t freeDist, uint8_t calledCount = 0)
 {
    _frontDistanceSensor->SetDistance();
    uint16_t distance = _frontDistanceSensor->GetDistance(); // Get Distance from Sensor
    uint8_t tried = 0;
    const uint8_t maxTry = 15;

    while (distance > freeDist && tried < maxTry)
    {
      _movementController.MoveNowDuring(MovementType::FORWARD, 200);
      _frontDistanceSensor->SetDistance();  
      distance = _frontDistanceSensor->GetDistance(); // Get Distance from Sensor
      tried += 1;
    }
    if (tried == maxTry)
    {
        if (calledCount > 2)
        {
          return (false);
        }
        FindFreePath(freeDist, (ULTRASONIC_MAX_DISTANCE - 50));
        if (FindGoodCalibrateLinearSpeedPosition(freeDist, calledCount + 1))
        {
            return (true);
        }
        else {
            return (false);
        }
    }
    return (true);
 }

 void   CalibrateLinearSpeed(uint16_t freeDist)
 {
    if (FindFreePath(freeDist, (ULTRASONIC_MAX_DISTANCE - 50)))
    {
      delay(3000);
      _movementController.MoveNowDuring(MovementType::FORWARD, 800);
    } else {
      _movementController.MoveNowDuring(MovementType::BACKWARD, 800);
    }

    
//    if (FindGoodCalibrateLinearSpeedPosition(freeDist, 0))
//    {
//  
//      float linearSpeedA = GetMeasureLinearSpeed(500, true);
//      float linearSpeedB = GetMeasureLinearSpeed(500, false);
//
//      float linearSpeed = ((linearSpeedA + linearSpeedB) * 0.5f);
//      SendMsg("Linear Speed:"+String(linearSpeed));
//    }
 }

  typedef struct RobotPosition {
      float     angle = 0;
      Vec2      dir;
      Vec2      pos;
      void      Reset()
      {
        angle = 0;
        pos.x = 0;
        pos.y = 0;
        dir.x = 1;
        dir.y = 0;
      }
  } RobotPosition;

	DistanceSensor*     _frontDistanceSensor;
  float               _frontDistanceLast;
 
	MovementController   _movementController;
  float               _dtMsVerification;
	float               _dtMs;

  uint8_t              _robotDiameter;
  RobotPosition       _robotPosition;
  RobovacState        _robotState;
};




//////////////////////////////////////////////
//////////////// END ROBOVAC //////////////////////////////
///////////////////////////////////////////////////

Robovac       robovac;

unsigned long timeStart;
unsigned long timeElapsed;
unsigned long timeCurrent;



void setup() {
	Serial.begin(9600);
	// put your setup code here, to run once:
	timeStart = millis();
	//robovac.Calibrate();
	robovac.Initialize();
}


void loop()
{
	delay(10); // Avoid Useless CPU Consumption
	timeCurrent = millis();

	timeElapsed = timeCurrent - timeStart;
	timeStart = timeCurrent;
	robovac.Operate(timeElapsed);

	// put your main code here, to run repeatedly:

}
