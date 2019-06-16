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
			_pathFound = false;
			_pathFov = 90;
			_steps = 15;
			_distancesOkCount = 0;
			_distancesOkToMatch = 15; // MUST NOT Be Superior To Steps
			_stepsMax = (480 / _steps); // Maximum 1.2 round
			_angleRotate = (_pathFov / _steps);
			_distances = new uint16_t[_distancesOkToMatch];
			_step = 0;
		}

		~FindPathInfo()
		{
		}

		void	Set(uint16_t minDist, uint16_t maxDist)
		{
			_pathFound = false;
			_step = 0;
			_distancesOkCount = 0;
			_minDist = minDist;
			_maxDist = maxDist;
		}

		// Return true if have enough distance OK in FOV
		bool	PushDistance(uint16_t distance)
		{
			_step += 1;
			if (distance > _minDist && distance < _maxDist)
			{
				_distances[_distancesOkCount] = distance;
				_distancesOkCount += 1;
				// std::cout << "Push Distance OK (" << (int)_distancesOkCount << ")" << std::endl;
				if (_distancesOkCount == _distancesOkToMatch)
				{
					_pathFound = true;
				}
				return (true);
			}
			else {
				_distancesOkCount = 0;
				return (false);
			}
		}

		bool		IsPathFound() const
		{
			return (_pathFound);
		}

		const uint8_t		GetDistancesOkCount() const { return (_distancesOkCount); };
		const uint16_t*		GetDistances() const { return (_distances); };
		const uint8_t&		GetAngleRotate() const { return (_angleRotate); };
		const uint8_t&		GetStep() const { return (_step); };
		const uint8_t&		GetStepsMax() const { return (_stepsMax); };
		const uint8_t&		GetPathFov() const { return (_pathFov); };

	private:


		bool		_pathFound;

		uint16_t*   _distances;
		uint8_t		_distancesOkCount;
		uint8_t		_distancesOkToMatch;

		uint16_t	_minDist;
		uint16_t	_maxDist;

		uint8_t		_pathFov = 80;
		uint8_t		_step = 0;
		uint8_t		_steps = 10;

		uint8_t		_stepsMax;
		uint8_t		_angleRotate;
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
		FindPath(30, 400000000);
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
		if (_goPathInfo.GetTimerSensor() > 400) // In Ms
		{
			float distance = GetFrontDistanceSensor();
			_goPathInfo.PushDistance((uint16_t)distance);
			if (distance < _diameter || _goPathInfo.IsBlocked()) {
				_movementController.Move(MovementType::STOP, true);
				StopBody();
				FindPath(30, 40000);
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

	void		FindPath(uint16_t minDist, uint16_t maxDist)
	{
		std::cout << "FindPath (" << minDist << ") (" << maxDist << ")";
		_state = FINDPATH;
		_findPathInfo.Set(minDist, maxDist);
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

		Vec2 test = Vec2(1.0, 0);

		uint8_t j = (_findPathInfo.GetDistancesOkCount() - 1);

		for (uint8_t i = 0; i < _findPathInfo.GetDistancesOkCount(); i += 1)
		{
			//std::cout << "Direction Path: " << directionPath.ToString() << std::endl;
			VectorTransformer::Rotate(directionPath, -(float)_findPathInfo.GetAngleRotate());
			VectorTransformer::Normalize(directionPath);
			
			// Draw Direction - and Maybe Distance
			posFront = pos + (directionPath * (float)_radius);
			posFrontDraw.x = (int)posFront.x;
			posFrontDraw.y = (int)posFront.y;
			//std::cout << "Distance (" << _findPathInfo.GetDistances()[i] << ")" << std::endl;

			posFrontDirDraw.x = (int)posFront.x + (directionPath.x * (float)_findPathInfo.GetDistances()[j]);
			posFrontDirDraw.y = (int)posFront.y + (directionPath.y * (float)_findPathInfo.GetDistances()[j]);
			drawer->DrawLine(posFrontDraw.x, posFrontDraw.y, posFrontDirDraw.x, posFrontDirDraw.y, Color::GREEN());
			j -= 1;
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
				if (_findPathInfo.PushDistance((uint16_t)distance)) {
					_movementController.Move(MovementType::ROTATE_LEFT, _findPathInfo.GetAngleRotate());
					VectorTransformer::Rotate(_direction, (float)_findPathInfo.GetAngleRotate());
					VectorTransformer::Normalize(_direction);
				}
				else {
					_movementController.Move(MovementType::ROTATE_LEFT, _findPathInfo.GetAngleRotate() * 2);
					VectorTransformer::Rotate(_direction, (float)(_findPathInfo.GetAngleRotate() * 2));
					VectorTransformer::Normalize(_direction);
				}
				if (_findPathInfo.IsPathFound()) {
					// Rotate 
					_movementController.Move(MovementType::ROTATE_RIGHT, (_findPathInfo.GetPathFov() / 2));
					VectorTransformer::Rotate(_direction, -(float)(_findPathInfo.GetPathFov() / 2));
					VectorTransformer::Normalize(_direction);
				}
				if (_findPathInfo.GetStep() > _findPathInfo.GetStepsMax()) {
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


	std::vector<size_t>					_objectsSelected;
	std::vector<size_t>					_objectsSelectedToDraw;
	std::unordered_map<size_t, Robovac>	_robovacs;

	size_t					_objectsCount;

	size_t					_idLastRobovac;
	Vec2					_released;

	RobovacBasics			_roombaBasics;
};

