#include "PX2CollisionListener.h"

PX2CollisionListener::PX2CollisionListener(const size_t & bodyCountMax)
{
	_bodyCountMax = bodyCountMax;
	_bodies.resize(_bodyCountMax);
	Reset();
}

void PX2CollisionListener::NotifyCollisionA(PX2Manifold * manifold)
{
	NotifyBodyCollision(manifold, manifold->A());
	//std::cout << "Notify Collision A" << std::endl;
	_collisionCount += 1;
}

void PX2CollisionListener::NotifyCollisionB(PX2Manifold * manifold)
{
	NotifyBodyCollision(manifold, manifold->B());
	_collisionCount += 1;
}

void PX2CollisionListener::NotifyCollisionAB(PX2Manifold * manifold)
{
	NotifyBodyCollision(manifold, manifold->A());
	NotifyBodyCollision(manifold, manifold->B());
	_collisionCount += 1;
}


#include <iostream> // TMP
#include "PX2DebugDrawer.h" // TMP


void PX2CollisionListener::Reset()
{
	for (size_t i = 0; i < _bodyCountMax; i += 1)
	{
		_bodies.at(i).list.Clear();
	}
	_collisionCount = 0;
}

void PX2CollisionListener::NotifyBodyCollision(PX2Manifold * manifold, PX2Body * body)
{
	if (_bodies.at(body->GetID()).list.GetFront())
	{
		if (_bodies.at(body->GetID()).list.GetFront()->data->toi > manifold->toi)
		{
			_bodies.at(body->GetID()).list.PushFrontManifold(manifold);
		}
		else
			_bodies.at(body->GetID()).list.PushBackManifold(manifold);
	}
	else
		_bodies.at(body->GetID()).list.PushFrontManifold(manifold);
}
