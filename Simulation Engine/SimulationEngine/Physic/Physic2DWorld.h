#pragma once


#include <vector>
#include "Physic2DBody.h"
#include "Physic2DShape.h"

class Physic2DWorld
{
public:
	Physic2DWorld(Vec2 gravity = Vec2(0, 9.8f))
	{
		
	}
	virtual ~Physic2DWorld() {};

	/// BASICS
	virtual void					Simulate(float timePassed) = 0;

	/// GET
	//PX2BodyManager *		GetBodyManager() const;

	/// SET
	virtual Physic2DBody*					CreateBody(Physic2DBodyProperties& properties) = 0;
	virtual Physic2DBody*					GetBody(const size_t& id) const = 0;
	virtual std::vector<Physic2DBody*>		GetBodies() const = 0;
	//std::vector<Physic2DBody*>	GetBodiesInAABB(const AABB2&aabb) const;
	//std::vector<Physic2DBody*>	GetBodiesAtPosition(const Vec2& pos) const;

	virtual void							DestroyBody(Physic2DBody* body) = 0;
	virtual void							DestroyAllBodies() = 0;


	virtual void							SetGravity(const Vec2 &gravity) = 0;
	virtual Vec2							GetGravity() const = 0;


	virtual Physic2DShapeCircle*			CreateShapeCircle() = 0;
	virtual Physic2DShapeBox*				CreateShapeBox() = 0;
	virtual Physic2DShapePolygon*			CreateShapePolygon() = 0;

	// Remove Everything in the World and Free Bodies (DO NOT free Shapes)
	virtual void					Clear() = 0;

protected:

private:

	

};