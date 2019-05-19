#include "PX2World.h"
#include "PX2BodyManager.h"
#include "PX2Body.h"

PX2World::PX2World(Vec2 gravity)
{
	_bodyManager = new PX2BodyManager(this);
	_gravity = gravity;
}

PX2World::~PX2World()
{
}

void PX2World::Simulate(float timePassed)
{
	_bodyManager->Simulate(timePassed, _gravity);
}

PX2BodyManager * PX2World::GetBodyManager() const
{
	return (_bodyManager);
}


#include <iostream> // Tmp
PX2Body* PX2World::CreateBody(PX2Body::Properties & properties)
{
	PX2Body*		body = new PX2Body(this, properties);
	//std::cout << "Create Body" << std::endl;
	return (body);
}

PX2Body * PX2World::GetBody(const size_t & id) const
{
	return (_bodyManager->GetBody(id));
}

std::vector<PX2Body*> PX2World::GetBodies() const
{
	return (_bodyManager->GetBodies());
}

std::vector<PX2Body*> PX2World::GetBodiesInAABB(const AABB2& aabb) const
{
	return (_bodyManager->GetBodiesInAABB(aabb));
}

std::vector<PX2Body*> PX2World::GetBodiesAtPosition(const Vec2 & pos) const
{
	return (_bodyManager->GetBodiesAtPosition(pos));
}

void PX2World::DestroyBody(PX2Body * body)
{
	if (body != NULL)
	{
		std::cout << "Destroy Body With ID (" << body->GetID() << ")" << std::endl;
		_bodyManager->DestroyBody(body->GetID());
	}
}

void PX2World::DestroyAllBodies()
{
	_bodyManager->DestroyAllBodies();
}

std::vector<PX2Shape*> PX2World::GetShapes() const
{
	std::vector<PX2Shape*>	shapes;
	std::unordered_map<size_t, PX2Shape*>::const_iterator it = _bodyManager->GetShapes().begin();
	std::unordered_map<size_t, PX2Shape*>::const_iterator itE = _bodyManager->GetShapes().end();

	shapes.resize(_bodyManager->GetShapes().size());

	for (size_t i = 0; it != itE; it++)
	{
		shapes.at(i) = it->second;
		i += 1;
	}
	return (shapes);
}

void PX2World::SetGravity(const Vec2 & gravity)
{
	_gravity = gravity;
}

const Vec2 & PX2World::GetGravity() const
{
	return (_gravity);
}

void PX2World::Clear()
{
	DestroyAllBodies();
	_bodyManager->ClearAllShapes();
}
