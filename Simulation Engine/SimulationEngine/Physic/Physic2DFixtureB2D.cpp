#include "Physic2DFixtureB2D.h"

Physic2DShape* Physic2DFixtureB2D::GetShape() const
{
	return (_properties.shape);
}

const Physic2DFixtureProperties Physic2DFixtureB2D::GetProperties() const
{
	return (_properties);
}

void Physic2DFixtureB2D::SetFixtureDefB2D(const b2FixtureDef& fixtureDef)
{
	_b2FixtureDef = fixtureDef;
}

const b2FixtureDef& Physic2DFixtureB2D::GetFixtureDefB2D() const
{
	return (_b2FixtureDef);
}

void Physic2DFixtureB2D::SetFixtureB2D(b2Fixture* fixture)
{
	_b2Fixture = fixture;
}
