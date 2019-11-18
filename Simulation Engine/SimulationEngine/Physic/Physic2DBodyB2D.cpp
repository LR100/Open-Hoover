#include "Physic2DBodyB2D.h"

#include "Physic2DFixtureB2D.h"

const std::vector<Physic2DFixture*>& Physic2DBodyB2D::GetFixtures() const
{
	return (_fixtures);
}

Physic2DFixture* Physic2DBodyB2D::CreateFixture(const Physic2DFixtureProperties& properties)
{
	Physic2DFixtureB2D* fixture = new Physic2DFixtureB2D(this, properties);

	fixture->SetFixtureB2D(_b2Body->CreateFixture(&fixture->GetFixtureDefB2D()));
	_fixtures.push_back(fixture);
	return (fixture);
}

#include <iostream> // DEBUG TMP

void Physic2DBodyB2D::SetIDB2D(const size_t& id)
{
	_id = id;
}

void Physic2DBodyB2D::SetBodyB2D(b2Body* body)
{
	std::cout << "Address of body position (" << &body->GetPosition() << ")" << std::endl;
	_b2Body = body;
}

const b2BodyDef& Physic2DBodyB2D::GetBodyDefB2D() const
{
	return (_b2BodyDef);
}

Physic2DWorld* Physic2DBodyB2D::GetWorld() const
{
	return ((Physic2DWorld*)_world);
}

const size_t& Physic2DBodyB2D::GetID() const
{
	return (_id);
}

void Physic2DBodyB2D::SetPosition(const Vec2& position)
{
	b2Vec2 v(position.x, position.y);
	_b2Body->SetTransform(v, _b2Body->GetAngle());
}

Vec2 Physic2DBodyB2D::GetPosition() const
{
	return (Vec2(_b2Body->GetPosition().x, _b2Body->GetPosition().y));
}

void Physic2DBodyB2D::SetVelocityLinear(const Vec2& velocity)
{
	std::cout << "Set VL: " << velocity.ToString() << std::endl;
	_b2Body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
}

Vec2 Physic2DBodyB2D::GetVelocityLinear() const
{
	return (Vec2(_b2Body->GetLinearVelocity().x, _b2Body->GetLinearVelocity().y));
}

const Physic2DBodyProperties::TYPE& Physic2DBodyB2D::GetType() const
{
	return (_properties.type);
}

const Physic2DBodyProperties& Physic2DBodyB2D::GetProperties() const
{
	return (_properties);
}

