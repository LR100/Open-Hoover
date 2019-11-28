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

#include "RobovacControl.h"

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
	const Robovac*	AddRobovac(const Vec2& pos);

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

	class DrawQueryCb : public Physic2DQueryBodyCb
	{
	public:
		DrawQueryCb(RobovacWorld* _world, IDrawer2D* _drawer);
		void			Reset();
		const size_t&	GetObjectsCount() const;

	private:
		// Inherited via Physic2DQueryBody
		virtual bool	ReportBody(Physic2DBody* body) override;

		IDrawer2D*		drawer;
		RobovacWorld*	world;
		size_t			objectsCount;
	};

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
	Physic2DWorld*		_p2dWorld;
	AABB2				_screen;


	std::vector<size_t>						_objectsSelected;
	std::vector<size_t>						_objectsSelectedToDraw;
	std::unordered_map<size_t, Robovac*>	_robovacs;

	size_t					_objectsCount;

	size_t					_idLastRobovac;
	Vec2					_released;

	RobovacBasics*			_roombaBasics;

};

