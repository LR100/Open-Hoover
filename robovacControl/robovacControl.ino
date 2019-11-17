#ifdef _WIN32 // ON WINDOWS

#define WIN

#include <stdint.h>

#endif


#include <MemoryFree.h> // TO VERIFY Memomry Used

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



#define BIT_SET(a, pos) ((a) |= (1ULL << (pos)))
#define BIT_CLEAR(a,pos) ((a) &= ~(1ULL<<(pos)))
#define BIT_FLIP(a,pos) ((a) ^= (1ULL<<(pos)))
#define BIT_CHECK(a,pos) (!!((a) & (1ULL<<(pos))))

class BoolArray
{
  public:

    BoolArray(const uint8_t& width, const uint8_t& height, bool valueInit = false) {
      _width = width;
      _height = height;
      Allocate();
      Init(valueInit);
    }
#ifdef _WIN32

    std::string ToString() {
      std::stringstream ss;
      bool        state;
      for (uint8_t h = 0; h < _height; h += 1) {
        for (uint8_t w = 0; w < _width; w += 1) {

          state = Get(w, h);
          if (state) {
            ss << "1";
          }
          else {
            ss << "0";
          }
          if (w < (_width - 1))
            ss << " ";
        }
        ss << "\n";
      }
      return (ss.str());
    }

#endif // _WIN32

    void  Set(const uint8_t& x, const uint8_t& y, bool state) {
      if (!PosIsValid(x, y))
        return;
      float   posByteF = roundf((float)((float)((float)y * (float)_width) + (float)x));
      uint16_t  posByte = (uint16_t)posByteF;
      uint8_t   posBit = (uint8_t)(posByte % 8);
      posByte = (uint16_t)(posByteF / 8.0f);
      if (state)
        BIT_SET(_array[posByte], posBit);
      else
        BIT_CLEAR(_array[posByte], posBit);
    }

    bool  Get(const uint8_t& x, const uint8_t& y) {
      if (!PosIsValid(x, y))
        return (false);
      float   posByteF = roundf(((float)((float)y * (float)_width) + (float)x));
      uint16_t  posByte = (uint16_t)posByteF;
      uint8_t   posBit = (uint8_t)(posByte % 8);
      posByte = (uint16_t)(posByteF / 8.0f);
      return (BIT_CHECK(_array[posByte], posBit));
    }

    const uint8_t& GetWidth() const {
      return (_width);
    };
    const uint8_t& GetHeight() const {
      return (_height);
    };

    bool      PosIsValid(const uint8_t& x, const uint8_t& y)
    {
      if (x >= _width || y >= _height)
        return (false);
      return (true);
    }

    void      Set(bool value)
    {
      for (uint8_t h = 0; h < _height; h += 1) {
        for (uint8_t w = 0; w < _width; w += 1) {
          Set(w, h, value);
        }
      }
    }

  private:

    void  Allocate() {
      uint16_t  size = (uint16_t)((uint16_t)_width * (uint16_t)_height);
      float   fsize = ceilf((float)size / 8.0f); // To be replaced on Arduino (std::ceilf)
      size = (uint16_t)fsize;
      _array = new uint8_t[size];
    }

    void  Init(const bool& valueInit) {
      Set(valueInit);
    }

    uint8_t* _array;
    uint8_t   _width;
    uint8_t   _height;
};

template <class A>
bool IsEqual(A a, A b, A diff) {
  return (abs(a - b) < diff);
}

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
// UInt8
template class Vec2T<uint8_t>;
typedef Vec2T<uint8_t> Vec2ui8;

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
    const unsigned int& GetDistance() const {
      return (_distance);
    };
    const unsigned int& GetDistancePrev() const {
      return (_distancePrev);
    };

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
      _speedLinear = 50.0f;
      _speedAngular = 240.0f; // 240° per second
      // Movement is in 2D (The Ground)
      // No management of highness (For the robot everything is Flat !)
      _initFunction[STOP] = &MovementController::Stop;
      _initFunction[FORWARD] = &MovementController::Forward;
      _initFunction[BACKWARD] = &MovementController::Backward;
      _initFunction[ROTATE_LEFT] = &MovementController::RotateLeft;
      _initFunction[ROTATE_RIGHT] = &MovementController::RotateRight;
    }

    void  SetSpeedLinear(const float& speedLinear) {
      _speedLinear = speedLinear;
    };
    void  SetSpeedAngular(const float& speedAngular) {
      _speedAngular = speedAngular;
    };


    // Unit could be an Angle (in degree) or a Distance (in cm)
    void        Move(MovementType movementType, uint16_t unit, bool force = false)
    {
      Serial << "Movement Controller Move ("<< (int)movementType<<") Of (" << (int)unit << ")\n";
      if (_moveIsFinished || force)
      {
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

      unsigned long timeMoveStart;
      unsigned long timeMoveElapsed;
      unsigned long timeMoveCurrent;

      timeMoveStart = millis();

      while (!_moveIsFinished)
      {
        timeMoveCurrent = millis();
        timeMoveElapsed = timeMoveCurrent - timeMoveStart;
        timeMoveStart = timeMoveCurrent;
        Update(timeMoveElapsed);
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


    void        Update(float dtMs)
    {
      _timeMoved += dtMs;
      if (_timeMoved >= _timeToMove)
      {
        if (_movementType != STOP)
        {
          Stop(0); // Unlimited Stop
          Serial << "Movement Controller Update (" << (int)_timeMoved << ") On (" << (int)_timeToMove << ") STOP\n";
          _timeToMove = 0;
          _timeMoved = 0;
          _movementType = STOP;
          _moveIsFinished = true;
        }
      }
    }

    const MovementType&   GetMovementType() const {
      return (_movementType);
    };
    const float&          GetSpeedLinear() const {
      return (_speedLinear);
    };
    const float&          GetSpeedAngular() const {
      return (_speedAngular);
    };
    const bool&           IsMoveFinished() const {
      return (_moveIsFinished);
    };

  private:

    void        Forward(uint16_t unit)
    {
      _timeToMove = ((float)unit / _speedLinear) * 1000.f; // to MS
      Serial << "Time to Move Forward of (" << (int)unit << ")cm : (" << _timeToMove <<")ms\n";
      _motorLeft.Forward();
      _motorRight.Forward();
    }
    void        Backward(uint16_t unit)
    {
      _timeToMove = ((float)unit / _speedLinear) * 1000.f; // to MS
      Serial << "Time to Move Backward of (" << (int)unit << ")cm : (" << _timeToMove <<")ms\n";
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
      Serial << "Set Rotate Unit Time (" << (int)unit << ") angle\n";
      float modSpeedAngular = _speedAngular; // More unit is low -> Slower it is
      Serial << "Mode Speed Angular (" << modSpeedAngular << ")\n";
      if (unit < 45) {
        //modSpeedAngular = (modSpeedAngular / (70.0f / (25.0f + (float)unit))); // From 3Times slower speed to -> 1 (from 0 to 10degree Rotation)
      }
      Serial << "MODIFIED Mode Speed Angular (" << modSpeedAngular << ")\n";
      _timeToMove = ((float)unit / modSpeedAngular) * 1000.f; // to MS

      _timeToMove += 60; // time to start rotating (50 ms approx)
      Serial << "Time (" << (int)_timeToMove  << ")to rotate for n (" << (int)unit << ") angle\n";
      //Serial << "Time (" << (int)_timeToMove  << ")to rotate for n (" << (int)unit << ") angle\n";
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


    class FindPathInfo
    {
      public:
        FindPathInfo()
        {
          _pathFov = 120;
          //					//  // This Number ! is the PRECISION
          _angleRotate = (_pathFov / 8); // Must Have Value with no decimals
          _stepsMax = (360 / _angleRotate); // Must Have Value with no decimals
          _distances = new uint16_t[_stepsMax];
          _maxDist = 40000; // 40 meters
          Reset();
        }

        ~FindPathInfo()
        {
        }

        void	Reset()
        {
          _pathFov = 120;
          _pathFound = false;
          _step = 0;
          _stepMiddle = 0;
          _pathAngle = 0;
        }

        void	Set(uint16_t minDist)
        {
          Reset();
          _minDist = minDist;
        }

        // Return false if have checked all distance and possibilities and no path can be found
        bool	PushDistance(uint16_t distance)
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

        bool	SearchDesperatePath()
        {
          _minDist = 30;
          _pathFov = 80;
          if (SearchPath())
            return (true);
          _minDist = 10;
          _pathFov = 100;
          if (SearchPath())
            return (true);
          return (false);
        }

        bool		SearchPath()
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

        bool		IsPathFound() const
        {
          return (_pathFound);
        }

        // Return Angle from start in good path direction
        uint16_t			GetPathAngleFromStart() const {
          return ((_step * _angleRotate) - _pathAngle);
        }


        uint16_t			GetShortestDistanceInFov(const uint8_t& radius)
        {
          uint8_t	stepsToValidHalf = (uint8_t)(((float)(_pathFov / 2) / _angleRotate) / 2.0f);
          uint8_t	stepsEnd = (_stepMiddle + stepsToValidHalf);
          uint16_t distanceMin = UINT16_MAX;

          for (uint8_t s = (_stepMiddle - stepsToValidHalf); s < stepsEnd; s += 1)
          {
            //std::cout << "_distances[" << (int)s << "]=" << _distances[s] << std::endl;
            if (_distances[s] < distanceMin)
            {
              distanceMin = _distances[s];
            }
          }
          //std::cout << "Distance MIn: (" << distanceMin << ")" <<std::endl;
          return (distanceMin);
        }

        const uint16_t* GetDistances() const {
          return (_distances);
        };
        const uint16_t& GetMinDist() const {
          return (_minDist);
        };
        const float& GetAngleRotate() const {
          return (_angleRotate);
        };
        const uint8_t& GetStep() const {
          return (_step);
        };
        const uint8_t& GetStepMiddle() const {
          return (_stepMiddle);
        };
        const uint8_t& GetStepsMax() const {
          return (_stepsMax);
        };
        const uint8_t& GetPathFov() const {
          return (_pathFov);
        };
        const uint16_t& GetPathAngleFromEnd() const {
          return (_pathAngle);
        };

      private:

        bool		_pathFound;

        uint16_t* _distances;

        uint16_t	_minDist;
        uint16_t	_maxDist;

        uint8_t		_pathFov = 80;
        uint16_t	_pathAngle = 0;
        uint8_t		_step = 0;
        uint8_t		_stepMiddle = 0;

        uint8_t		_stepsMax;
        float		_angleRotate;
    };

    class GoPathInfo
    {
      public:

        GoPathInfo()
        {
          Reset();
        }

        void	Set(const uint16_t* distances, uint16_t distanceMin)
        {
          Reset();
          _distances = distances;
          _distanceMin = distanceMin;
        }

        void			Update(const uint16_t dtMs)
        {
          _timerSensor += dtMs;
        }

        void			ResetTimerSensor() {
          _timerSensor = 0;
        }

        const uint16_t& GetTimerSensor() const {
          return (_timerSensor);
        };
        const uint16_t& GetDistanceOld() const {
          return (_distanceOld);
        };


        float GetSpeedEstimation(const uint16_t& dtMs, const uint16_t& distanceCurrent, bool goForward = true)
        {
          return (GetSpeedEstimation(dtMs, _distanceOld, distanceCurrent, goForward));
        }

        // Considering Robot is moving forward
        static float	GetSpeedEstimation(const uint16_t& dtMs, const uint16_t& distanceOld, const uint16_t& distanceCurrent, bool goForward = true) {
          float distanceTravelled;

          if (goForward) { // Forward
            distanceTravelled = ((float)distanceOld - (float)distanceCurrent);
          }
          else { // Backward
            distanceTravelled = ((float)distanceCurrent - (float)distanceOld);
          }
          return ((distanceTravelled / (float)dtMs) * 1000.0f); // To convert in unit/s
        }

        const void		PushDistance(uint16_t distance)
        {
          if (IsEqual<uint16_t>(distance, _distanceOld, 4))
          {
            _distanceSameCount += 1;
          }
          else {
            _distanceSameCount = 0;
          }
          _distanceOld = distance;
        }

        bool			IsBlocked()
        {
          return (_distanceSameCount > 2 || (_distanceSameCount >= 2 && _linearSpeedWeirdCount >= 1));
        }

        const uint8_t& GetSameDistanceCount() const {
          return (_distanceSameCount);
        }

        void	NotifyLinearSpeedWeird() {
          _linearSpeedWeirdCount += 1;
        }

        void	ResetLinearSpeedWeirdCount() {
          _linearSpeedWeirdCount = 0;
        }

        const uint8_t& GetLinearSpeedWeirdCount() {
          return (_linearSpeedWeirdCount);
        }

        const uint16_t& GetDistanceMin() {
          return (_distanceMin);
        }

        void            SetDistanceMin(uint16_t distanceMin)
        {
          _distanceMin = distanceMin;
        }

      private:

        void	Reset()
        {
          _timerSensor = 0;
          _distanceOld = 0;
          _distanceSameCount = 0;
          _linearSpeedWeirdCount = 0;
          _distanceMin = 10;
        }

        uint8_t			_linearSpeedWeirdCount;
        uint8_t			_distanceSameCount;
        uint16_t		_distanceOld;
        uint16_t		_timerSensor;
        const uint16_t* _distances; // Only a pointer - Depend of another Object - Not in charge to free it
        uint16_t		_distanceMin;
    };


    typedef enum RobovacState : uint8_t
    {
      REST = 0,
      CALIBRATE = 1,
      FINDPATH,
      GOPATH,
      BLOCKED,
    } RobovacState;

    Robovac() : RobovacSC(RF24_PIN_CE, RF24_PIN_CS), _map(30, 30, false), _mapClean(30, 30, false)
    {
      Serial.begin(9600);
      Serial << "Robovac Init\n";
      _movementController.SetSpeedAngular(240.0f); // Value Is Known from precedent Calibrate Call
      _movementController.SetSpeedLinear(90.0f); // Value Is Known from precedent Calibrate Call


      _frontDistanceSensor = new DistanceSensorUltrasonic(ULTRASONIC_FRONT_PIN_P, ULTRASONIC_FRONT_PIN_M);
#ifndef WIN

#define RELAY_ALIM_PIN 4
      pinMode(RELAY_ALIM_PIN, OUTPUT);
      digitalWrite(RELAY_ALIM_PIN, HIGH);
#endif

      _movementController.Move(MovementType::STOP, 1000, true);
      _state = REST;
      _diameter = 30; // In Cm - Must be Set Manually for the moment - Could be self calibrate
      _radius = (_diameter / 2.0);
      _direction = Vec2(0.0f, 1.0f);

      _rotateDir = ((int)(this) % 3);
      _dtMs = 0;
      _rotateAngle = 0;
      //ResetMap();
      //_movementController.MoveNow(MovementType::ROTATE_LEFT, 4);
      Serial << "---------------\n";
      Serial << "Robovac Init OK\n";
      Serial << "---------------\n";
      Start();
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
      Serial << "\nOperate Hoover (" << dtMs << ")ms\n";
      //_dtMs = (float)dtMs;
      //_movementController.Update(_dtMs); // Update Movement after decision has been taked
       UpdateCommunication(); // Update Client
      // UpdateFromState();
      static int i = 0;
      
       SendMsg("Coucou" + String(i));
      i+= 1;
    }


    void		Start()
    {
      Serial << "Start!!\n";
      FindPath(40);
      //ResetMap();
    }

  private:

#define DISTANCE_TOO_CLOSE 10

    void UpdateFromState()
    {
      if (_state == CALIBRATE) {
        _state = REST; // At End OF Calibrate -> Robot Is Resting
        _dtMs = 0; // Was calibrating _dtMs is reset
        // Demo();
      } else if (_state == REST) {
       // SendMsg("I'm Resting");
      } else {
        UpdateWork(_dtMs);
      }
    }

    void		UpdateWork(const float& dt)
    {
      //std::cout << "Update Robovac (" << body->GetID() << ")" << std::endl;
      if (_state == FINDPATH) {
        UpdateFindPath(dt);
      }
      else if (_state == GOPATH) {
        UpdateGoPath(dt);
      }
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

    void		SetRadius(uint8_t radius)
    {
      _radius = radius;
    }

    void		Stop()
    {
      _movementController.Move(MovementType::STOP, 0);
      //_body->SetVelocityLinear(Vec2(0, 0));
    }

    void		GoPath()
    {
      //std::cout << "GoPath ";
      _state = GOPATH;
      _goPathInfo.Set(_findPathInfo.GetDistances(), _goPathInfo.GetDistanceMin());
    }

    void		ReactToPbGoPath() {
      _movementController.Move(MovementType::STOP, true);
      //StopBody();
      FindPath(50); // Search a valid Raisonable path
    }

    void		SetPosHooverInMap()
    {
      _posMap.x = ((_posHoover.x + _radius) / (float)_diameter);
      _posMap.y = ((_posHoover.y + _radius) / (float)_diameter);
    }

    void		SetCleanFromPosHoover()
    {
      //std::cout << "Map Clean Set: X(" << (int)_posMap.x << ") Y(" << (int)_posMap.y << ")" << std::endl;
      _mapClean.Set(_posMap.x, _posMap.y, true);
      _map.Set(_posMap.x, _posMap.y, true); // If part map is clean -> Path is available also
    }

    void		SetPosHooverForward(float unit)
    {
      _posHoover.x = (_posHoover.x + (unit * _direction.x));
      _posHoover.y = (_posHoover.y + (unit * _direction.y));
      SetPosHooverInMap();
    }

    void		UpdateGoPath(const float& dt)
    {
      Serial << "Go Path Update: (" << dt << ")\n";
      Vec2	correctMove;
      float	dtS = (dt / 1000.0f);
      _goPathInfo.Update((uint16_t)dt);

      if (_movementController.IsMoveFinished())
      {
        Serial << "Move Finished GO Forward Distance Min(" << (int)_goPathInfo.GetDistanceMin() << ")\n";

        _movementController.Move(MovementType::FORWARD, _goPathInfo.GetDistanceMin());
        SetCleanFromPosHoover();
        //MoveBody();
      } else {
        //MoveBody();

        //SetPosHooverForward(dtS * _movementController.GetSpeedLinear());
        //SetPosHooverInMap();
        //std::cout << "Pos Hoover: " << _posHoover.ToString() << std::endl;
      }
      if (_goPathInfo.GetTimerSensor() > 200) // In Ms
      {
        float distance = _frontDistanceSensor->GetDistance();
        float estimatedSpeed = _goPathInfo.GetSpeedEstimation(_goPathInfo.GetTimerSensor(), (uint16_t)distance); // Forward
        Serial << "Speed Estimation (" << estimatedSpeed << ") Expected: (" << _movementController.GetSpeedLinear() << ")\n" ;

        if (estimatedSpeed < (float)_speedLinearWeird) {
          _goPathInfo.NotifyLinearSpeedWeird();
          if (_goPathInfo.GetLinearSpeedWeirdCount() >= 2) {
            // SetPosHooverForward(-10); // Has Not Moved
            ReactToPbGoPath();
            return; // Stop And FindPath
          }
        }
        else {
          _goPathInfo.ResetLinearSpeedWeirdCount();
        }
//        _goPathInfo.PushDistance((uint16_t)distance);

        if (distance < _radius || _goPathInfo.IsBlocked()) {
          // Should remap if blocked
          //if (_goPathInfo.IsBlocked())
          //SetPosHooverForward(-10); // Has Not Moved
          ReactToPbGoPath();
          return; // Stop And FindPath
        }
        _goPathInfo.ResetTimerSensor();
      }

    }



    void		Blocked()
    {
      _state = BLOCKED;
    }

    void		FindPath(uint16_t minDist)
    {
      //std::cout << "FindPath (" << minDist << ")";
      _state = FINDPATH;
      _findPathInfo.Set(minDist);
      _movementController.Move(MovementType::STOP, 0);
    }

    virtual void	OnReadCommandCalibrate(RobovacCommand* command) override
    {
      if (_state != CALIBRATE) // Already Calibrating
      {
        Start();
        //_state = CALIBRATE;
        //Calibrate();
        //Demo();

      }
    }

    virtual void	OnReadCommandMovement(RobovacCommandMovement* command) override
    {
      if (_state != CALIBRATE) // Already Calibrating
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


    void		RotateFromDir(const float& angle) {
      if (_rotateDir) {
        _movementController.Move(MovementType::ROTATE_LEFT, angle);
      }
      else {
        _movementController.Move(MovementType::ROTATE_RIGHT, angle);
      }
    }

    void		PlaceFindPath()
    {
      // Rotate Left for FindPath
      if (_rotateDir) {
        _movementController.Move(MovementType::ROTATE_RIGHT, (_findPathInfo.GetPathAngleFromEnd()));
        //VectorTransformer::Rotate(_direction, -(float)(_findPathInfo.GetPathAngleFromEnd())); // Right
      }
      else {
        _movementController.Move(MovementType::ROTATE_LEFT, (_findPathInfo.GetPathAngleFromEnd()));
        //VectorTransformer::Rotate(_direction, (float)(_findPathInfo.GetPathAngleFromEnd())); // Left
      }
      //VectorTransformer::Normalize(_direction);
      // std::cout << "Angle Rotated from origin (" << _findPathInfo.GetAngleRotated() << ")";
      _rotateAngle += _findPathInfo.GetPathAngleFromStart();
      if (_rotateAngle > 495) {
        _rotateAngle = 0;
        _rotateDir = !_rotateDir;
      }
    }

    void		SetFreePathInMap(int x, int y, int xE, int yE)
    {
      int		deltaX, deltaY;
      int		incX, incY;
      int		i;
      int		sum;
      int		xi, yi;
      uint8_t xiU;
      uint8_t yiU;

      i = 1;

      xi = x;
      yi = y;

      deltaX = (xE - xi);
      deltaY = (yE - yi);

      if (deltaX < 0)
        incX = -1;
      else
        incX = 1;

      if (deltaY < 0)
        incY = -1;
      else
        incY = 1;

      deltaX = abs(deltaX);
      deltaY = abs(deltaY);

      if (deltaX > deltaY)
      {
        // Little Endian Optimization
        sum = (deltaX >> 1);
        while (i <= deltaX)
        {
          xi += incX;
          sum += deltaY;
          if (sum >= deltaX)
          {
            sum -= deltaX;
            yi += incY;
          }

          xiU = (uint8_t)xi;
          yiU = (uint8_t)yi;
          _map.Set(xiU, yiU, true);
          i += 1;
        }
      }
      else
      {
        // Little Endian Optimization
        sum = (deltaY >> 1);
        while (i <= deltaY)
        {
          yi += incY;
          sum += deltaX;
          if (sum >= deltaY)
          {
            sum -= deltaY;
            xi += incX;
          }

          xiU = (uint8_t)xi;
          yiU = (uint8_t)yi;
          _map.Set(xiU, yiU, true);
          i += 1;
        }
      }
    }

    void		SetFreePathFromDistance(const float& distance)
    {
      int x, y;
      int xE, yE;
      float distanceScaled = (distance / (float)_diameter);

      x = (int)_posMap.x;
      y = (int)_posMap.y;
      xE = (int)((float)_posMap.x + (_direction.x * distanceScaled));
      yE = (int)((float)_posMap.y + (_direction.y * distanceScaled));
      SetFreePathInMap(x, y, xE, yE);

      //::cout << "Set Free Path in Map from: ("<< x <<") ("<< y <<") to ("<< xE <<") ("<< yE <<")" << std::endl;
    }

    void		UpdateFindPath(const float& dt)
    {
      Serial << "Find Path Update: (" << dt << ")\n";
      if (!_findPathInfo.IsPathFound())
      {
        if (_movementController.IsMoveFinished())
        {
          _frontDistanceSensor->SetDistance();
          float distance = (float)_frontDistanceSensor->GetDistance();
          SetFreePathFromDistance(distance);
          Serial << "Find Path Update - Move Finished Distance (" << distance << ")\n";
          if (_findPathInfo.PushDistance((uint16_t)distance))
          {
            if (_findPathInfo.IsPathFound()) {
              Serial << "Find Path Update - Path FOUND (Min Dist: (" << (int)_findPathInfo.GetShortestDistanceInFov(_radius) << ")\n";
              _goPathInfo.SetDistanceMin((uint16_t)_findPathInfo.GetShortestDistanceInFov(_radius));
              PlaceFindPath();
            }
            else {
              // Constant Rotate
              //
              //Serial << "ANGLE ROTATE: (" <<  _findPathInfo.GetAngleRotate() << ")\n";
              if (_rotateDir) { // Rotate Left
                _movementController.Move(MovementType::ROTATE_LEFT, _findPathInfo.GetAngleRotate(), true);
                //VectorTransformer::Rotate(_direction, (float)_findPathInfo.GetAngleRotate());
              }
              else { // Rotate Right
                _movementController.Move(MovementType::ROTATE_RIGHT, _findPathInfo.GetAngleRotate(), true);
                //VectorTransformer::Rotate(_direction, -(float)_findPathInfo.GetAngleRotate());
              }
              //VectorTransformer::Normalize(_direction);
            }

          }
          else {
            Blocked(); // BLOCKED
          }
        }
      }
      else {
        if (_movementController.IsMoveFinished())
        {
          GoPath();
        }
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

      for (uint8_t s = 0; s < maxSteps; s += 1)
      {
        delay(50);
        _frontDistanceSensor->SetDistance();
        distance = _frontDistanceSensor->GetDistance(); // Get Distance from Sensor
       // SendMsg("FreeDist:" + String(distance));
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
          //SendMsg("I GO");
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

    bool MarkersAreEqual(uint16_t* markersA, uint16_t* markersB, uint8_t nbMarkers, float diffAccept = 0.05, float percentAccept = 0.80f)
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
       // UpdateCommunication();
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
          //SendMsg("Mark:" + String(distance) + "CNT:" + String(measureCount));
        }
        else
        {
          if (measureCount > markerPosEnd) // Start to compare Markers and Current Distance
          {
            if (measureCount > (markerPosEnd + 10) && MarkersAreEqual(markers, &dist[measureCount - markersCount], markersCount, diffAccept, percentAccept))
            {
              delete(markers);
              delete(dist);
             // SendMsg("OK CALIB:" + String(360.0f / timePassed));
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
      //SendMsg("BAD CALIB:" + String((((float)angularVelocityMax + (float)angularVelocityMin) / 2.0f)));
      if (_movementController.GetSpeedAngular() > 0)
      {
        return (_movementController.GetSpeedAngular());
      } // Else return Average of supposed AngularSpeed
      return (((float)angularVelocityMax + (float)angularVelocityMin) / 4.0f);
    }

    void          CalibrateAngularSpeed()
    {
      float angularSpeed = CalibrateAngularRotate(90, 360, 40, 4.0f, 0.34, 0.72);
     // SendMsg("CALIB:" + String(angularSpeed));
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
    }

    void		ResetMap()
    {
      Serial << "Reset Map\n";
      _posMap.x = _map.GetWidth() / 2.0;
      _posMap.y = _map.GetHeight() / 2.0;
      _posHoover.x = (_posMap.x * _diameter) - _radius;
      _posHoover.y = (_posMap.y * _diameter) - _radius;
      _map.Set(false);
      _mapClean.Set(false);
      Serial << "Reset Map OK\n";
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


    Vec2				_centerMap; // For DEBUG Draw ONLY
    Vec2				_posOffset; // For DEBUG Draw ONLY

    Vec2				_posHoover; // Estimated
    Vec2ui8				_posMap; // Estimated from pos Hoover

    float         _dtMs;
    BoolArray			_map;
    BoolArray			_mapClean;
    uint8_t				_speedLinearWeird = 60.0f;
    uint16_t			_rotateAngle;
    bool				_rotateDir;  // Rotate Left if True - else Rotate Right

    uint8_t				_diameter = 30;
    uint8_t				_radius = 15;
    Vec2				_direction;

    FindPathInfo		_findPathInfo;
    GoPathInfo			_goPathInfo;

    RobovacState		_state;
    MovementController	_movementController;
};




//////////////////////////////////////////////
//////////////// END ROBOVAC //////////////////////////////
///////////////////////////////////////////////////

Robovac*       robovac = NULL;

unsigned long timeStart;
unsigned long timeElapsed;
unsigned long timeCurrent;

#include <printf.h>

void setup() {
  Serial.begin(9600);
  while(!Serial);

  printf_begin();
  // put your setup code here, to run once:
  timeStart = millis();
  //robovac.Calibrate();
  if (robovac == NULL)
  {
    robovac = new Robovac();
    robovac->Initialize();
  }
  Serial << "Robovac Initialized\n";
}


void loop()
{
  //delay(10); // Avoid Useless CPU Consumption
  while (true)
  {
    Serial.print("freeMemory()=");
    Serial.println(freeMemory());
    timeCurrent = millis();

    timeElapsed = timeCurrent - timeStart;
    timeStart = timeCurrent;
    robovac->Operate(timeElapsed);
  }
  // put your main code here, to run repeatedly:

}

//
//// Load in the libraries
//#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>
//
//// Set the CE & CSN pins
//#define CE_PIN   7
//#define CSN_PIN 8
//
//// This is the address used to send/receive
//const byte rxAddr[6] = "00001";
//
//// Create a Radio
//RF24 radio(CE_PIN, CSN_PIN); 
//
//void setup() {
//  
//  // Start up the Serial connection
//  while (!Serial);
//  Serial.begin(9600);
//  
//  // Start the Radio!
//  radio.begin();
//  
//  // Power setting. Due to likelihood of close proximity of the devices, set as RF24_PA_MIN (RF24_PA_MAX is default)
//  radio.setPALevel(RF24_PA_MIN); // RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
//  
//  // Slower data rate for better range
//  radio.setDataRate( RF24_250KBPS ); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
//  
//  // Number of retries and set tx/rx address
//  radio.setRetries(15, 15);
//  radio.openWritingPipe(rxAddr);
//
//  // Stop listening, so we can send!
//  radio.stopListening();
//}
//
//void loop() {
//
//  // Set up a message and a timestamp to it using millis()
//  String str = "Hello world! "; 
//  str += String( millis() );
//  
//  // http://stackoverflow.com/questions/7383606/converting-an-int-or-string-to-a-char-array-on-arduino
//  // Length (with one extra character for the null terminator)
//  int str_len = str.length() + 1; 
//  
//  // Prepare the character array (the buffer) 
//  char char_array[str_len];
//  
//  // Copy it over 
//  str.toCharArray(char_array, str_len);
//
//  // Ace, let's now send the message
//  radio.write(&char_array, sizeof(char_array));
//  
//  // Let the ourside world know..
//  Serial.print("Sent Message: ");
//  Serial.print( char_array );
//  Serial.println("");
//  
//  // Wait a short while before sending the other one
//  delay(80);
//}
