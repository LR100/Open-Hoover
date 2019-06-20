#pragma once

#include <string>
#include "Tools\XMLParser.h"
#include "Physic\PX2World.h"
#include "Physic\PX2Body.h"
#include "Physic\PX2BodyManager.h" // TMP - For RayTracing

#include "Graphic\IDrawer2D.h"
#include "Events\IEventHandler.h"




template <class A> 
bool IsEqual(A a, A b, A diff) {
	return (abs(a - b) < diff);
}

static IDrawer2D*	ROBOVACDRAWER = NULL;
static IWindow*		ROBOVACWINDOW = NULL;

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
			_minDist = 50;
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
						std::cout << "OK Steps To Valid (" << (int)stepsToValid << ")" << std::endl;
						uint8_t stepsToComeBack = (_step - s) + (stepsToValid / 2);
						std::cout << "Steps TO ComeBack" << (int)stepsToComeBack << std::endl;
						_pathAngle = (uint16_t)((float)_angleRotate * (float)stepsToComeBack);
						std::cout << "Path Angle:" << (int)_pathAngle << std::endl;
						_pathFound = true;
						return (true);
					}
				} else {
					stepsValid = 0; // Unvalid Step - Restart from 0
				}
				
			}
		}

		bool		IsPathFound() const
		{
			return (_pathFound);
		}

		// Return Angle from start in good path direction
		uint16_t			GetAngleRotated() const {
			return ((_step * _angleRotate) - _pathAngle);
		}

		const uint16_t*		GetDistances() const { return (_distances); };
		const uint16_t&		GetMinDist() const { return (_minDist); };
		const float&		GetAngleRotate() const { return (_angleRotate); };
		const uint8_t&		GetStep() const { return (_step); };
		const uint8_t&		GetStepsMax() const { return (_stepsMax); };
		const uint8_t&		GetPathFov() const { return (_pathFov); };
		const uint16_t&		GetPathAngle() const { return (_pathAngle); };

	private:

		bool		_pathFound;

		uint16_t*   _distances;

		uint16_t	_minDist;
		uint16_t	_maxDist;

		uint8_t		_pathFov = 80;
		uint16_t	_pathAngle = 0;
		uint8_t		_step = 0;

		uint8_t		_stepsMax;
		float		_angleRotate;
	};

	enum RobovacState
	{
		REST = 0,
		CALIBRATE = 1,
		FINDPATH = 2,
		GOPATH = 3,
		BLOCKED,
	};
	


	Robovac()
	{
		_direction = Vec2(0.0f, 1.0f);
		_rotateDir = true;
		_rotateAngle = 0;
		FindPath(100);
	}

	void		DrawDir(IDrawer2D* drawer)
	{
		Vec2 pos = body->GetPosition();
		Vec2i	posDraw;
		Vec2i	posDirDraw;

		posDraw.x = (int)pos.x;
		posDraw.y = (int)pos.y;
		posDirDraw.x = (int)(pos.x + (_direction.x * (float)_radius));
		posDirDraw.y = (int)(pos.y + (_direction.y * (float)_radius));
		// std::cout << "Draw From: " << posDraw.ToString() << " To:" << posDirDraw.ToString() << std::endl;
		drawer->DrawLine(posDraw.x, posDraw.y, posDirDraw.x, posDirDraw.y, Color::RED());
	}

	void		Draw(IDrawer2D* drawer)
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

//private:

	void		Stop()
	{
		_movementController.Move(MovementType::STOP, 0);
		body->SetVelocityLinear(Vec2(0, 0));
	}

	class GoPathInfo
	{
	public:

		GoPathInfo()
		{
			Reset();
		}


		void	Set(const uint16_t* distances)
		{
			_distances = distances;
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
			return (_distanceSameCount > 2);
		}

	private:

		void	Reset()
		{
			_timerSensor = 0;
			_distanceOld = 0;
			_distanceSameCount = 0;
		}

		uint8_t			_distanceSameCount;
		uint16_t		_distanceOld;
		uint16_t		_timerSensor;
		const uint16_t* _distances; // Only a pointer - Depend of another Object - Not in charge to free it
	};

	void		GoPath()
	{
		std::cout << "GoPath ";
		_state = GOPATH;
		_goPathInfo.Set(_findPathInfo.GetDistances());	
	}

	void		UpdateGoPath(const float& dt)
	{
		_goPathInfo.Update((uint16_t)dt);
		if (_movementController.IsMoveFinished())
		{
			std::cout << "Move Forward" << std::endl;
			_movementController.Move(MovementType::FORWARD, 10);
			MoveBody();
		}
		else {
			_movementController.Update(dt);
		}
		if (_goPathInfo.GetTimerSensor() > 200) // In Ms
		{
			float distance = GetFrontDistanceSensor();
			_goPathInfo.PushDistance((uint16_t)distance);
			if (distance < _diameter || _goPathInfo.IsBlocked()) {
				_movementController.Move(MovementType::STOP, true);
				StopBody();
				FindPath(50); // Search a valid Raisonable path 
				return; // Stop And FindPath
			}
			_goPathInfo.ResetTimerSensor();
		}
	}

	void		DrawBlocked(IDrawer2D* drawer)
	{
		Vec2 pos = body->GetPosition();
		Vec2i	posDraw;

		posDraw.x = (int)pos.x;
		posDraw.y = (int)pos.y;
		// std::cout << "Draw From: " << posDraw.ToString() << " To:" << posDirDraw.ToString() << std::endl;
		drawer->DrawCircleFill(posDraw.x, posDraw.y, _radius, Color::ORANGE());
		drawer->DrawCircleFill(posDraw.x, posDraw.y, (_radius - 3), Color::RED());
	}

	void		Blocked()
	{
		_state = BLOCKED;
	}

	void		FindPath(uint16_t minDist)
	{
		std::cout << "FindPath (" << minDist << ")";
		_state = FINDPATH;
		_findPathInfo.Set(minDist);
		_movementController.Move(MovementType::STOP, 0);
	}

	void		DrawFindPath(IDrawer2D* drawer)
	{
		// Draw Good Distances
		float	angleOff = 0;
		Vec2	directionPath = _direction;

		Vec2	pos = body->GetPosition();
		Vec2	posFront;
		Vec2i	posFrontDraw;
		Vec2i	posFrontDirDraw;

		Vec2	test = Vec2(1.0, 0);
		Color	color;

		
		if (_findPathInfo.IsPathFound())
		{
			// Position at end of step
			if (_rotateDir) {
				VectorTransformer::Rotate(directionPath, (float)(_findPathInfo.GetPathAngle()));
			} else {
				VectorTransformer::Rotate(directionPath, -(float)(_findPathInfo.GetPathAngle()));
			}
		}
		else {
			// One step before
			if (_rotateDir) {
				VectorTransformer::Rotate(directionPath, -(float)_findPathInfo.GetAngleRotate());
			} else {
				VectorTransformer::Rotate(directionPath, (float)_findPathInfo.GetAngleRotate());
			}
			
		}

		VectorTransformer::Normalize(directionPath);

		uint8_t j = (_findPathInfo.GetStep() - 1);
		// Overflow is controlled by loop
		for (uint8_t i = 0; i < (_findPathInfo.GetStep() - 1); i += 1)
		{
			//std::cout << "Direction Path: " << directionPath.ToString() << std::endl;
			if (_rotateDir) {
				VectorTransformer::Rotate(directionPath, -(float)_findPathInfo.GetAngleRotate());
			} else {
				VectorTransformer::Rotate(directionPath, (float)_findPathInfo.GetAngleRotate());
			}
			VectorTransformer::Normalize(directionPath);
			
			// Draw Direction - and Maybe Distance
			posFront = pos + (directionPath * (float)_radius);
			posFrontDraw.x = (int)posFront.x;
			posFrontDraw.y = (int)posFront.y;
			//std::cout << "Distance (" << _findPathInfo.GetDistances()[i] << ")" << std::endl;

			posFrontDirDraw.x = (int)posFront.x + (directionPath.x * (float)_findPathInfo.GetDistances()[j]);
			posFrontDirDraw.y = (int)posFront.y + (directionPath.y * (float)_findPathInfo.GetDistances()[j]);
			if (_findPathInfo.GetDistances()[j] > _findPathInfo.GetMinDist()) {
				color = Color::GREEN();
			} else {
				color = Color::RED();
			}

			drawer->DrawLine(posFrontDraw.x, posFrontDraw.y, posFrontDirDraw.x, posFrontDirDraw.y, color);
			j -= 1;
		}
	}


	void		PlaceFindPath()
	{
		// Rotate Left for FindPath
		if (_rotateDir) {
			_movementController.Move(MovementType::ROTATE_RIGHT, (_findPathInfo.GetPathAngle()));
			VectorTransformer::Rotate(_direction, -(float)(_findPathInfo.GetPathAngle())); // Right
		} else {
			_movementController.Move(MovementType::ROTATE_LEFT, (_findPathInfo.GetPathAngle()));
			VectorTransformer::Rotate(_direction, (float)(_findPathInfo.GetPathAngle())); // Left
		}
		VectorTransformer::Normalize(_direction);
		// std::cout << "Angle Rotated from origin (" << _findPathInfo.GetAngleRotated() << ")";
		_rotateAngle += _findPathInfo.GetAngleRotated();
		if (_rotateAngle > 480) {
			_rotateAngle = 0;
			_rotateDir = !_rotateDir;
		}
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
				if (_findPathInfo.PushDistance((uint16_t)distance))
				{
					if (_findPathInfo.IsPathFound()) {
						PlaceFindPath();
					} else {
						// Constant Rotate
						// 
						if (_rotateDir) { // Rotate Left
							_movementController.Move(MovementType::ROTATE_LEFT, _findPathInfo.GetAngleRotate());  
							VectorTransformer::Rotate(_direction, (float)_findPathInfo.GetAngleRotate());
						} else { // Rotate Right
							_movementController.Move(MovementType::ROTATE_RIGHT, _findPathInfo.GetAngleRotate()); 
							VectorTransformer::Rotate(_direction, -(float)_findPathInfo.GetAngleRotate());
						}
						VectorTransformer::Normalize(_direction);
					}

				} else {
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
		const Vec2& position = (body->GetPosition() + (_direction * (float)_radius));
		Vec2		inter;
		body->GetWorld()->GetBodyManager()->GetTreeCollision()->GetFirstIndexInRay(position, _direction, inter);
		float distance = 0;
		distance = VectorTransformer::Distance(inter, position);
		//std::cout << "Get Distance: (" << distance << ")" << std::endl;
		return (distance);
	}

	void				StopBody()
	{
		body->SetVelocityLinear(Vec2(0.0f, 0.0f));
	}

	void				MoveBody(bool forward = true)
	{
		//																	
		Vec2 linearSpeed = (_direction * (_movementController.GetSpeedLinear()));
		if (forward) {
			body->SetVelocityLinear(linearSpeed);
		} else {
			body->SetVelocityLinear(-linearSpeed);
		}
	}
	
	PX2Body*			body;

private:

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
	void	DrawBody(PX2Body* body, IDrawer2D* drawer);
	void	DrawRobovac(PX2Body* body, IDrawer2D* drawer);
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
		RobovacBasics();

		float		radius;
		Sprite* sprite;
		PX2Shape* shape;
	};

	// For Physics Managment
	PX2World* _pxWorld;
	AABB2					_screen;


	std::vector<size_t>						_objectsSelected;
	std::vector<size_t>						_objectsSelectedToDraw;
	std::unordered_map<size_t, Robovac*>	_robovacs;

	size_t					_objectsCount;

	size_t					_idLastRobovac;
	Vec2					_released;

	RobovacBasics			_roombaBasics;
};

