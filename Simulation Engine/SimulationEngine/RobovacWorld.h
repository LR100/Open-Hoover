#pragma once

#include <string>
#include "Tools\XMLParser.h"
#include "Physic/Physic2DWorld.h"
#include "Physic/XML/Physic2DWorldXML.h"
//#include "Physic\Physic2DBody.h"
//#include "Physic\Physic2DBodyManager.h" // TMP - For RayTracing
#include "Maths/VectorTransformer.h"

#include "Graphic\IDrawer2D.h"
#include "Events\IEventHandler.h"
#include "Graphic\IWindow.h"


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

	std::string	ToString() {
		std::stringstream	ss;
		bool				state;
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

	void	Set(const uint8_t& x, const uint8_t& y, bool state) {
		if (!PosIsValid(x, y))
			return;
		float		posByteF = std::roundf(((float)((float)y * (float)_width) + (float)x));
		uint16_t	posByte = (uint16_t)posByteF;
		uint8_t		posBit = (uint8_t)(posByte % 8);
		posByte = (uint16_t)(posByteF / 8.0f);
		if (state)
			BIT_SET(_array[posByte], posBit);
		else
			BIT_CLEAR(_array[posByte], posBit);
	}

	bool	Get(const uint8_t& x, const uint8_t& y) {
		if (!PosIsValid(x, y))
			return (false);
		float		posByteF = std::roundf(((float)((float)y * (float)_width) + (float)x));
		uint16_t	posByte = (uint16_t)posByteF;
		uint8_t		posBit = (uint8_t)(posByte % 8);
		posByte = (uint16_t)(posByteF / 8.0f);
		return (BIT_CHECK(_array[posByte], posBit));
	}

	const uint8_t& GetWidth() const { return (_width); };
	const uint8_t& GetHeight() const { return (_height); };

	bool			PosIsValid(const uint8_t& x, const uint8_t& y)
	{
		if (x >= _width || y >= _height)
			return (false);
		return (true);
	}

	void			Set(bool value)
	{
		for (uint8_t h = 0; h < _height; h += 1) {
			for (uint8_t w = 0; w < _width; w += 1) {
				Set(w, h, value);
			}
		}
	}

private:

	void	Allocate() {
		uint16_t	size = (uint16_t)((uint16_t)_width * (uint16_t)_height);
		float		fsize = std::ceilf((float)size / 8.0f); // To be replaced on Arduino (std::ceilf)
		size = (uint16_t)fsize;
		_array = new uint8_t[size];
	}

	void	Init(const bool& valueInit) {
		Set(valueInit);
	}

	uint8_t* _array;
	uint8_t		_width;
	uint8_t		_height;
};


template <class A>
bool IsEqual(A a, A b, A diff) {
	return (abs(a - b) < diff);
}

static IDrawer2D* ROBOVACDRAWER = NULL;
static IWindow* ROBOVACWINDOW = NULL;

static IDrawer2D* ROBOVACMAPDRAWER = NULL;
static IWindow* ROBOVACMAPWINDOW = NULL;

class RobovacSimulation; // TMP

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

struct Robovac
{
public:

	class FindPathInfo
	{
	public:
		FindPathInfo()
		{
			_pathFov = 80;
			//					//  // This Number ! is the PRECISION
			_angleRotate = (_pathFov / 20); // Must Have Value with no decimals
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
			_pathFov = 50;
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
				std::cout << "_distances[" << (int)s << "]=" << _distances[s] << std::endl;
				if (_distances[s] < distanceMin)
				{
					distanceMin = _distances[s];
				}
			}
			std::cout << "Distance MIn: (" << distanceMin << ")" <<std::endl;
			return (distanceMin);
		}

		const uint16_t* GetDistances() const { return (_distances); };
		const uint16_t& GetMinDist() const { return (_minDist); };
		const float& GetAngleRotate() const { return (_angleRotate); };
		const uint8_t& GetStep() const { return (_step); };
		const uint8_t& GetStepMiddle() const { return (_stepMiddle); };
		const uint8_t& GetStepsMax() const { return (_stepsMax); };
		const uint8_t& GetPathFov() const { return (_pathFov); };
		const uint16_t& GetPathAngleFromEnd() const { return (_pathAngle); };

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
			_distances = distances;
			_distanceMin = distanceMin;
			Reset();
		}

		void			Update(const uint16_t dtMs)
		{
			_timerSensor += dtMs;
		}

		void			ResetTimerSensor() {
			_timerSensor = 0;
		}

		const uint16_t& GetTimerSensor() const { return (_timerSensor); };
		const uint16_t& GetDistanceOld() const { return (_distanceOld); };


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

	enum RobovacState
	{
		REST = 0,
		CALIBRATE = 1,
		FINDPATH = 2,
		GOPATH = 3,
		BLOCKED,
	};


	Robovac() : _map(70, 70, false), _mapClean(70, 70, false) // 30 * (ex diameter: 30) = 900 -> 9m dist of area
	{
		_diameter = 30; // MUST be Set manually by user
		_radius = (_diameter / 2.0);
		_direction = Vec2(0.0f, 1.0f);
		std::cout << "Rotate Dir Val:" << (int)(this) << std::endl;
		_rotateDir = ((int)(this) % 3);
		std::cout << "Rotate Dir:" << _rotateDir << std::endl;
		_rotateAngle = 0;
		ResetMap();
		std::cout << "Approximate Size of a Robovac: (" << sizeof(Robovac) << ")" << std::endl;
	}

	void		Start()
	{
		FindPath(100);
	}

	void		DrawDir(IDrawer2D* drawer)
	{
		Vec2 pos = _body->GetPosition();
		Vec2i	posDraw;
		Vec2i	posDirDraw;

		posDraw.x = (int)pos.x;
		posDraw.y = (int)pos.y;
		posDirDraw.x = (int)(pos.x + (_direction.x * (float)_radius));
		posDirDraw.y = (int)(pos.y + (_direction.y * (float)_radius));
		// std::cout << "Draw From: " << posDraw.ToString() << " To:" << posDirDraw.ToString() << std::endl;
		drawer->DrawLine(posDraw.x, posDraw.y, posDirDraw.x, posDirDraw.y, Color::RED());
	}

	void		Draw(IDrawer2D* drawer, bool drawMap = true)
	{
		//std::cout << "Draw Robovac" << std::endl;
		if (_state == FINDPATH) {
			DrawFindPath(drawer);
		}
		else if (_state == GOPATH) {
			// DrawGoPath(drawer);
		}
		else if (_state == BLOCKED) {
			DrawBlocked(drawer);
		}
		DrawDir(drawer);
		if (drawMap) {
			DrawMap(drawer);
		}
	}

	void		Update(const float& dt)
	{
		//std::cout << "Update Robovac (" << body->GetID() << ")" << std::endl;
		if (_state == FINDPATH) {
			UpdateFindPath(dt);
		}
		else if (_state == GOPATH) {
			UpdateGoPath(dt);
		}
	}

	void		SetRadius(uint8_t radius)
	{
		_radius = radius;
	}

	void		Stop()
	{
		_movementController.Move(MovementType::STOP, 0);
		_body->SetVelocityLinear(Vec2(0, 0));
	}

	void		DrawBox(const uint8_t boxX, const uint8_t boxY, IDrawer2D* drawer) {

		Color	color;

		color = Color::WHITE();

		Vec2i	posScaled;
		unsigned int w = _diameter;
		unsigned int h = _diameter;

		int xToMid = (int)(((float)_map.GetWidth() / 2.0f) - (float)boxX);
		int yToMid = (int)(((float)_map.GetHeight() / 2.0f) - (float)boxY);

		// Path MAP
		posScaled.x = _centerMap.x - (xToMid * (int)_diameter);
		posScaled.y = _centerMap.y - (yToMid * (int)_diameter);; // Middle Map
		
		if (!_map.Get(boxX, boxY))
		{
			drawer->DrawRect(posScaled.x, posScaled.y, w, h, Color::GREY());
			//drawer->DrawRectFill(posScaled.x, posScaled.y, w, h, 0.2f, color);
		}
		// CLEAN Map
		if (_mapClean.Get(boxX, boxY))
			color = Color::GREEN();
		else
			color = Color::RED();

		posScaled.x = (_centerMap.x - (xToMid * (int)_diameter)) + 1;
		posScaled.y = (_centerMap.y - (yToMid * (int)_diameter)) + 1; // Middle Map

		w -= 1;
		h -= 1;
		drawer->DrawBorder(posScaled.x, posScaled.y, w, h, color, color);
	}

	void		DrawMap(IDrawer2D* drawer)
	{
		for (uint8_t y = 0; y < _map.GetHeight(); y += 1)
		{
			for (uint8_t x = 0; x < _map.GetWidth(); x += 1)
			{
				DrawBox(x, y, drawer);
			}
		}
		// Draw POs Hoover
		Vec2i posHS;

		posHS.x = (_posHoover.x + _posOffset.x);
		posHS.y = (_posHoover.y + _posOffset.y);
		drawer->DrawCircleFill(posHS.x, posHS.y, 6, Color::ORANGE());
	}

	void		GoPath()
	{
		std::cout << "GoPath ";
		_state = GOPATH;
		_goPathInfo.Set(_findPathInfo.GetDistances(), _findPathInfo.GetShortestDistanceInFov(_radius));
	}

	void		ReactToPbGoPath() {
		_movementController.Move(MovementType::STOP, true);
		StopBody();
		FindPath(40); // Search a valid Raisonable path 
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
		Vec2	correctMove;
		float	dtS = (dt / 1000.0f);
		_goPathInfo.Update((uint16_t)dt);

		if (_movementController.IsMoveFinished())
		{
			std::cout << "Move Finished GO Forward" << std::endl;
			
			_movementController.Move(MovementType::FORWARD, _goPathInfo.GetDistanceMin());
			SetCleanFromPosHoover();
			MoveBody();
		}
		else {
			_movementController.Update(dt);
			MoveBody();
			_posHoover = (_body->GetPosition() - _posOffset);
			SetPosHooverInMap();
			//SetPosHooverForward(dtS * _movementController.GetSpeedLinear());
			//std::cout << "Pos Hoover: " << _posHoover.ToString() << std::endl;
		}
		if (_goPathInfo.GetTimerSensor() > 200) // In Ms
		{
			float distance = GetFrontDistanceSensor();
			float estimatedSpeed = _goPathInfo.GetSpeedEstimation(_goPathInfo.GetTimerSensor(), (uint16_t)distance); // Forward
			//std::cout << "Speed Estimation (" << estimatedSpeed << ")" << std::endl;

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
			_goPathInfo.PushDistance((uint16_t)distance);

			if (distance < _diameter || _goPathInfo.IsBlocked()) {
				// Should remap if blocked
				//if (_goPathInfo.IsBlocked())
				//SetPosHooverForward(-10); // Has Not Moved
				ReactToPbGoPath();
				return; // Stop And FindPath
			}
			_goPathInfo.ResetTimerSensor();
		}

	}
#ifdef _WIN32
	void		DrawBlocked(IDrawer2D* drawer)
	{
		Vec2 pos = _body->GetPosition();
		Vec2i	posDraw;

		posDraw.x = (int)pos.x;
		posDraw.y = (int)pos.y;
		// std::cout << "Draw From: " << posDraw.ToString() << " To:" << posDirDraw.ToString() << std::endl;
		drawer->DrawCircleFill(posDraw.x, posDraw.y, _radius, Color::ORANGE());
		drawer->DrawCircleFill(posDraw.x, posDraw.y, (_radius - 3), Color::RED());
	}
#endif // _WIN32

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


	void		RotateFromDir(const float& angle) {
		if (_rotateDir) {
			_movementController.Move(MovementType::ROTATE_LEFT, angle);
		}
		else {
			_movementController.Move(MovementType::ROTATE_RIGHT, angle);
		}
	}

	void		RotateFromDir(Vec2& path, const float& angle) {
		if (_rotateDir) {
			VectorTransformer::Rotate(path, angle); // Rotate Right
		}
		else {
			VectorTransformer::Rotate(path, -angle); // Rotate Left
		}
		VectorTransformer::Normalize(path);
	}

#ifdef _WIN32
	void		DrawFindPath(IDrawer2D* drawer)
	{
		// Draw Good Distances
		float	angleOff = 0;
		Vec2	directionPath = _direction;

		Vec2	pos = _body->GetPosition();
		Vec2	posFront;
		Vec2i	posFrontDraw;
		Vec2i	posFrontDirDraw;

		Vec2	test = Vec2(1.0, 0);
		Color	color;


		if (_findPathInfo.IsPathFound()) {
			// Position at end of step
			RotateFromDir(directionPath, (float)_findPathInfo.GetPathAngleFromEnd()); // Rotate Right
		}
		else {
			// One step before
			RotateFromDir(directionPath, -(float)_findPathInfo.GetAngleRotate()); // Else Rotate Left
		}

		uint8_t j = (_findPathInfo.GetStep() - 1);
		// Overflow is controlled by loop
		for (uint8_t i = 0; i < (_findPathInfo.GetStep() - 1); i += 1)
		{
			//std::cout << "Direction Path: " << directionPath.ToString() << std::endl;
			RotateFromDir(directionPath, -(float)_findPathInfo.GetAngleRotate());

			// Draw Direction - and Maybe Distance
			posFront = pos + (directionPath * (float)_radius);
			posFrontDraw.x = (int)posFront.x;
			posFrontDraw.y = (int)posFront.y;
			//std::cout << "Distance (" << _findPathInfo.GetDistances()[i] << ")" << std::endl;

			posFrontDirDraw.x = (int)posFront.x + (directionPath.x * (float)_findPathInfo.GetDistances()[j]);
			posFrontDirDraw.y = (int)posFront.y + (directionPath.y * (float)_findPathInfo.GetDistances()[j]);
			if (_findPathInfo.GetDistances()[j] > _findPathInfo.GetMinDist()) {
				color = Color::GREEN();
			}
			else {
				color = Color::RED();
			}
			drawer->DrawLine(posFrontDraw.x, posFrontDraw.y, posFrontDirDraw.x, posFrontDirDraw.y, color);
			j -= 1;
		}
	}
#endif


	void		PlaceFindPath()
	{
		// Rotate Left for FindPath
		if (_rotateDir) {
			_movementController.Move(MovementType::ROTATE_RIGHT, (_findPathInfo.GetPathAngleFromEnd()));
			VectorTransformer::Rotate(_direction, -(float)(_findPathInfo.GetPathAngleFromEnd())); // Right
		}
		else {
			_movementController.Move(MovementType::ROTATE_LEFT, (_findPathInfo.GetPathAngleFromEnd()));
			VectorTransformer::Rotate(_direction, (float)(_findPathInfo.GetPathAngleFromEnd())); // Left
		}
		VectorTransformer::Normalize(_direction);
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
		if (ROBOVACDRAWER && ROBOVACWINDOW)
		{
			ROBOVACDRAWER->DrawCircleFill(x, y, 6, Color::BLUE());
			ROBOVACDRAWER->DrawCircleFill(xE, yE, 6, Color::YELLOW());
			ROBOVACWINDOW->Refresh();
		}
		//::cout << "Set Free Path in Map from: ("<< x <<") ("<< y <<") to ("<< xE <<") ("<< yE <<")" << std::endl;
	}

	void		UpdateFindPath(const float& dt)
	{
		// std::cout << "Find Path Update" << std::endl;
		if (!_findPathInfo.IsPathFound())
		{
			if (_movementController.IsMoveFinished())
			{
				// std::cout << "Find Path Update - Move Finished" << std::endl;
				float distance = GetFrontDistanceSensor();
				SetFreePathFromDistance(distance);
				if (_findPathInfo.PushDistance((uint16_t)distance))
				{
					if (_findPathInfo.IsPathFound()) {
						PlaceFindPath();
					}
					else {
						// Constant Rotate
						// 
						if (_rotateDir) { // Rotate Left
							_movementController.Move(MovementType::ROTATE_LEFT, _findPathInfo.GetAngleRotate());
							VectorTransformer::Rotate(_direction, (float)_findPathInfo.GetAngleRotate());
						}
						else { // Rotate Right
							_movementController.Move(MovementType::ROTATE_RIGHT, _findPathInfo.GetAngleRotate());
							VectorTransformer::Rotate(_direction, -(float)_findPathInfo.GetAngleRotate());
						}
						VectorTransformer::Normalize(_direction);
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
		_movementController.Update(dt);
	}

	float		GetFrontDistanceSensor()
	{
		const Vec2& position = (_body->GetPosition() + (_direction * (float)_radius));
		Vec2		inter;
		//_body->GetWorld()->GetBodyManager()->GetTreeCollision()->GetFirstIndexInRay(position, _direction, inter);
		float distance = 0;
		distance = VectorTransformer::Distance(inter, position);
		distance = 200; /// TMPPPPPPPPPP FOR TEST BOX2D MOVEMENT
		//std::cout << "Get Distance: (" << distance << ")" << std::endl;
		return (distance);
	}

	void				StopBody()
	{
		std::cout << "Stop Body " << std::endl;
		_body->SetVelocityLinear(Vec2(0.0f, 0.0f));
	}

	void				MoveBody(bool forward = true)
	{
		//																	
		Vec2 linearSpeed = (_direction * (_movementController.GetSpeedLinear()));
		if (forward) {
			_body->SetVelocityLinear(linearSpeed);
		}
		else {
			_body->SetVelocityLinear(-linearSpeed);
		}
	}

	void		SetBody(Physic2DBody* body)
	{
		_body = body;
		SetInfoBodyMap();
	}

	
private:

	void		SetInfoBodyMap()
	{
		if (_body)
		{
			_centerMap = _body->GetPosition();
			_posOffset.x = _centerMap.x - _posHoover.x;
			_posOffset.y = _centerMap.y - _posHoover.y;
		}
	}

	void		ResetMap()
	{
		_posMap.x = _map.GetWidth() / 2.0;
		_posMap.y = _map.GetHeight() / 2.0;
		_posHoover.x = (_posMap.x * _diameter) - _radius;
		_posHoover.y = (_posMap.y * _diameter) - _radius;
		_map.Set(false);
		_mapClean.Set(false);
	}


	Physic2DBody* _body;


	Vec2				_centerMap; // For DEBUG Draw ONLY
	Vec2				_posOffset; // For DEBUG Draw ONLY

	Vec2				_posHoover; // Estimated
	Vec2ui8				_posMap; // Estimated from pos Hoover

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

class RobovacWorld
{
public:

	friend class RobovacSimulation; // TMP TOO

	RobovacWorld(const std::string& pathMap);
	RobovacWorld();

	~RobovacWorld() {};


	void	SetEvents(IEventHandler* eventHandler);

	void	InitDrawer(IDrawer2D* drawer);
	void	Draw(IDrawer2D* drawer);

	void			AddWall(const AABB2& wall);
	const Robovac* AddRobovac(const Vec2& pos);

	void	SelectObjectsAtPosition(const Vec2& pos);
	void	RemoveObjectsAtPosition(const Vec2& pos);

	void	Update(const float& dt);

	void	Import(const std::string& pathMap);
	void	Clear();

private:

	//// Draw Part ////
	// Body
	void	DrawBody(Physic2DBody* body, IDrawer2D* drawer);
	void	DrawRobovacs(IDrawer2D* drawer);
	void	DrawRobovac(IDrawer2D* drawer, Robovac* robovac);
	// Last Robovac Created Info
	void	DrawInfoRobovac(IDrawer2D* drawer);

	void	UpdateRobovacs(const float& dtMs);

	//// END - Draw Part ////

	//// Events Part ////

	void	MouseLeftReleased(const Mouse* mouse);
	void	MouseMotion(const Mouse* mouse);

	void	MoveLastRobovac(Vec2 dir);
	//// END - Events Part ////

	// For FUN

	void	Crazy();


	struct RobovacBasics
	{
		RobovacBasics(Physic2DWorld* world);

		float					radius;
		Sprite*					sprite;
		Physic2DShapeCircle*	shape;
	};

	// For Physics Managment
	//Physic2DWorld* _pxWorld;
	Physic2DWorld*  _p2dWorld;
	AABB2			_screen;


	std::vector<size_t>						_objectsSelected;
	std::vector<size_t>						_objectsSelectedToDraw;
	std::unordered_map<size_t, Robovac*>	_robovacs;

	size_t					_objectsCount;

	size_t					_idLastRobovac;
	Vec2					_released;

	RobovacBasics*			_roombaBasics;
};

