#pragma once

#include <string>
#include "Tools\XMLParser.h"
#include "Physic\PX2World.h"

#include "Graphic\IDrawer2D.h"
#include "Events\IEventHandler.h"


class RobovacSimulation; // TMP

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

	void	AddWall(const AABB2&wall);
	void	AddRobovac(const Vec2& pos);
	
	void	SelectObjectsAtPosition(const Vec2& pos);
	void	RemoveObjectsAtPosition(const Vec2& pos);

	void	Update(const float& dt);

	void	Import(const std::string& pathMap);
	void	Clear();

private:

	//// Draw Parts ////
	// Body
	void	DrawBody(PX2Body* body, IDrawer2D* drawer);
	// Last Robovac Created Info
	void	DrawInfoRobovac(IDrawer2D* drawer);

	void	MouseLeftReleased(const Mouse* mouse);
	void	MouseMotion(const Mouse* mouse);


	void	MoveLastRobovac(Vec2 dir);

	// For FUN

	void	Crazy();



	struct RobovacBasics
	{
		RobovacBasics();

		float		radius;
		Sprite*		sprite;
		PX2Shape*	shape;
	};

	// For Physics Managment
	PX2World*				_pxWorld;
	AABB2					_screen;


	std::vector<size_t>		_objectsSelected;
	std::vector<size_t>		_objectsSelectedToDraw;

	size_t					_objectsCount;

	size_t					_idLastRobovac;
	Vec2					_released;

	RobovacBasics			_roombaBasics;
};
