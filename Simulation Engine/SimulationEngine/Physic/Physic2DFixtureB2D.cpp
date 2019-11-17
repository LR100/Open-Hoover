#include "Physic2DFixtureB2D.h"

const Physic2DShape* Physic2DFixtureB2D::GetShape() const
{
	return (_shape);
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
