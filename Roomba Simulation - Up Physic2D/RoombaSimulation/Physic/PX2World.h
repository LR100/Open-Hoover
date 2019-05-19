#pragma once


#include <vector>
#include "PX2Body.h"

class PX2BodyManager;


class PX2World
{
public:
	PX2World(Vec2 gravity = Vec2(0, 9.8f));
	~PX2World();

	/// BASICS
	void					Simulate(float timePassed);

	/// GET
	PX2BodyManager *		GetBodyManager() const;

	/// SET
	PX2Body*				CreateBody(PX2Body::Properties& properties);

	PX2Body*				GetBody(const size_t& id) const;
	std::vector<PX2Body*>	GetBodies() const;
	std::vector<PX2Body*>	GetBodiesInAABB(const AABB2&aabb) const;
	std::vector<PX2Body*>	GetBodiesAtPosition(const Vec2& pos) const;

	void					DestroyBody(PX2Body* body);
	void					DestroyAllBodies();

	std::vector<PX2Shape*>	GetShapes() const;

	void					SetGravity(const Vec2 &gravity);
	const Vec2&				GetGravity() const;

	// Remove Everything in the World and Free Bodies (DO NOT free Shapes)
	void					Clear();



private:

	Vec2			_gravity;
	PX2BodyManager * _bodyManager;
};
