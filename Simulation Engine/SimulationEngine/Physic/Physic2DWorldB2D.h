#pragma once

#include "Physic2DBody.h"
#include "Physic2DWorld.h"


#include <deque>

///
// Box2D
//
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2World.h"


#include <iostream> // TMP debug

class Physic2DWorldB2D : public Physic2DWorld
{
public:
	friend class Physic2DBodyB2D;

	Physic2DWorldB2D(Vec2 gravity = Vec2(0, 9.8f));
	
	~Physic2DWorldB2D()
	{
		if (_b2World != NULL) {
			delete _b2World;
			_b2World = NULL;
		}
	}

	/// BASICS
	virtual void					Simulate(float timePassed) override;

	/// GET
	//PX2BodyManager *		GetBodyManager() const;

	/// SET
	virtual Physic2DBody* CreateBody(Physic2DBodyProperties& properties) override;
	virtual Physic2DBody* GetBody(const size_t& id) const override;
	virtual std::vector<Physic2DBody*>		GetBodies() const override;
	//std::vector<Physic2DBody*>	GetBodiesInAABB(const AABB2&aabb) const;
	//std::vector<Physic2DBody*>	GetBodiesAtPosition(const Vec2& pos) const;

	virtual void							DestroyBody(Physic2DBody* body) override;
	virtual void							DestroyAllBodies() override;


	virtual void							SetGravity(const Vec2& gravity) override;
	virtual Vec2							GetGravity() const override;

	virtual Physic2DShapeCircle*	CreateShapeCircle() override;
	virtual Physic2DShapeBox*		CreateShapeBox() override;
	virtual Physic2DShapePolygon*	CreateShapePolygon() override;

	// Remove Everything in the World and Free Bodies (DO NOT free Shapes)
	virtual void					Clear() override;

private:

	size_t							GetBodyAvailableID();

	std::vector<Physic2DBody*>	_bodies;
	b2World*					_b2World;
	int32						_velocityIterations;  //how strongly to correct velocity
	int32						_positionIterations;


	std::deque<size_t>			_bodiesAvailableIDs;
	size_t						_bodyCountMax;
};
