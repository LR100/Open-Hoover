#pragma once

#include "PX2Body.h"
#include "PX2ManifoldList.h"

class PX2CollisionListener
{
public:
	PX2CollisionListener(const size_t& bodyCountMax);
	virtual ~PX2CollisionListener() {};

	struct CollisionInfo
	{
		// PX2Body*			body; // No Need To Store Body -> Id is implicit (pos in vector)
		PX2ManifoldList		list; // All Collision in Swept Volume (first have smallest TOI)
		PX2Manifold*			manifold;
	};


	// SET
	void	NotifyCollisionA(PX2Manifold* manifold);
	void	NotifyCollisionB(PX2Manifold* manifold);
	void	NotifyCollisionAB(PX2Manifold* manifold);
	void	Reset();


	// GET
	const size_t&							GetCollisionCount() const { return (_collisionCount); };
	const size_t&							GetBodyCountMax() const { return (_bodyCountMax); };
	const std::vector<CollisionInfo>&		GetCollisions() const { return (_bodies); };


private:

	void	NotifyBodyCollision(PX2Manifold* manifold, PX2Body* body);
	
	size_t						_collisionCount;
	std::vector<CollisionInfo>	_bodies;
	size_t						_bodyCountMax;
};
