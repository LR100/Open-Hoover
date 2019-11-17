#include "Physic2DWorldB2D.h"
#include "Physic2DBodyB2D.h"
#include "Physic2DShapeB2D.h"

#define MAX_BODIES 10000

Physic2DWorldB2D::Physic2DWorldB2D(Vec2 gravity) : Physic2DWorld(gravity)
{
	_bodyCountMax = MAX_BODIES;
	_b2World = new b2World(b2Vec2(gravity.x, gravity.y));
	_velocityIterations = 8;  //how strongly to correct velocity
	_positionIterations = 3;
	std::cout << "Box2DWorld is INIT" << std::endl;
	// Init IDS For Bodies
	for (size_t i = 0; i < _bodyCountMax; i += 1)
	{
		_bodiesAvailableIDs.push_back(_bodyCountMax - i);
	}
}

void Physic2DWorldB2D::Simulate(float timePassed)
{
	std::cout << "Physic2DWorldB2D::Simulate" << std::endl;
	_b2World->Step(timePassed, _velocityIterations, _positionIterations);
}


Physic2DBody* Physic2DWorldB2D::CreateBody(Physic2DBodyProperties& properties)
{
	Physic2DBodyB2D* body = new Physic2DBodyB2D(this, properties);
	body->SetBodyB2D(_b2World->CreateBody(&body->GetBodyDefB2D()));
	body->SetIDB2D(GetBodyAvailableID());
	_bodies.push_back(body);
	return (body);
}

Physic2DBody* Physic2DWorldB2D::GetBody(const size_t& id) const
{
	return nullptr;
}

std::vector<Physic2DBody*> Physic2DWorldB2D::GetBodies() const
{
	return (_bodies);
}

void Physic2DWorldB2D::DestroyBody(Physic2DBody* body)
{
}

void Physic2DWorldB2D::DestroyAllBodies()
{
}

void Physic2DWorldB2D::SetGravity(const Vec2& gravity)
{
	_b2World->SetGravity(b2Vec2(gravity.x, gravity.y));
}

Vec2 Physic2DWorldB2D::GetGravity() const
{
	return Vec2(_b2World->GetGravity().x, _b2World->GetGravity().y);
}

Physic2DShapeCircle* Physic2DWorldB2D::CreateShapeCircle()
{
	return (new Physic2DShapeCircleB2D(this));
}

Physic2DShapeBox* Physic2DWorldB2D::CreateShapeBox()
{
	return (new Physic2DShapeBoxB2D(this));
}

Physic2DShapePolygon* Physic2DWorldB2D::CreateShapePolygon()
{
	return (new Physic2DShapePolygonB2D(this));
}

void Physic2DWorldB2D::Clear()
{
}

size_t Physic2DWorldB2D::GetBodyAvailableID()
{
	size_t	id;

	if (_bodiesAvailableIDs.size())
	{
		id = _bodiesAvailableIDs.back();
		_bodiesAvailableIDs.pop_back();
	}
	else
	{
		id = SIZE_MAX;
	}
	return (id);
}
