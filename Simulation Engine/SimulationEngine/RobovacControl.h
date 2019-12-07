#pragma once

#include "RobovacBoolArray.h"
#include "RobovacMovementController.h"

#ifdef _WIN32
#include "Graphic/IDrawer2D.h"
#include "Graphic/IWindow.h"
#include "Physic/Physic2DBody.h"
#include "Physic/Physic2DWorld.h"
#include "Maths/VectorTransformer.h"
#include <iostream>
#endif // _WIN32


template <class A>
bool IsEqual(A a, A b, A diff) {
	return (abs(a - b) < diff);
}

static IDrawer2D* ROBOVACDRAWER = NULL;
static IWindow* ROBOVACWINDOW = NULL;

static IDrawer2D* ROBOVACMAPDRAWER = NULL;
static IWindow* ROBOVACMAPWINDOW = NULL;

class RobovacSimulation; // TMP


struct Robovac
{
public:

	class FindPathInfo
	{
	public:
		FindPathInfo();
		~FindPathInfo() {};

		void	Reset();
		void	Set(uint16_t minDist);
		// Return false if have checked all distance and possibilities and no path can be found
		bool	PushDistance(uint16_t distance);
		// Search Functions
		bool	SearchDesperatePath();
		bool	SearchPath();

		bool	IsPathFound() const { return (_pathFound); }

		// Return Angle from start in good path direction
		uint16_t GetPathAngleFromStart() const;
		uint16_t GetShortestDistanceInFov(const uint8_t& radius);

		// GETTERS //
		const uint16_t* GetDistances() const { return (_distances); };
		const uint16_t& GetMinDist() const { return (_minDist); };
		const float&	GetAngleRotate() const { return (_angleRotate); };
		const uint8_t&	GetStep() const { return (_step); };
		const uint8_t&	GetStepMiddle() const { return (_stepMiddle); };
		const uint8_t&	GetStepsMax() const { return (_stepsMax); };
		const uint8_t&	GetPathFov() const { return (_pathFov); };
		const uint16_t& GetPathAngleFromEnd() const { return (_pathAngle); };

	private:

		bool		_pathFound;

		uint16_t*	_distances;

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
		if (drawMap) { // Map under everything
			DrawMap(drawer);
		}
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

	void		Stop()
	{
		_movementController.Move(MovementType::STOP, 0);
		_body->SetVelocityLinear(Vec2(0, 0));
	}

	void		DrawBox(const uint8_t boxX, const uint8_t boxY, IDrawer2D* drawer) {

		Color	color;
		Color	coolGrey(25, 35, 30);
		Color	coolClean(5, 50, 150);

		color = coolGrey;

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
			drawer->DrawRect(posScaled.x, posScaled.y, w, h, coolGrey);
			//drawer->DrawRectFill(posScaled.x, posScaled.y, w, h, 0.2f, color);
		}
		// CLEAN Map
		if (_mapClean.Get(boxX, boxY)) {
			color = coolClean;
		}
		else {

		}
			

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
		std::cout << "Radius ? (" << (int)_radius << ")" << std::endl;
		uint8_t fov = 110; // Minimum
		if (fov < _findPathInfo.GetPathFov()) {
			fov = _findPathInfo.GetPathFov();
		}
		_goPathInfo.Set(_findPathInfo.GetDistances(), _findPathInfo.GetShortestDistanceInFov(fov));
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
		static  Vec2 posMiddle;


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
			//ROBOVACDRAWER->DrawCircleFill(x, y, 6, Color::BLUE());
			//ROBOVACDRAWER->DrawCircleFill(xE, yE, 6, Color::YELLOW());
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
		Physic2DWorld::RaytraceOutput raytraceOutput;
		float distance = 0;

		raytraceOutput = _body->GetWorld()->Raytrace(position, _direction, 4000.0f);
		inter = raytraceOutput.intersection;

		distance = VectorTransformer::Distance(inter, position);
		// distance = 200; /// TMPPPPPPPPPP FOR TEST BOX2D MOVEMENT
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