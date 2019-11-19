#pragma once

#include "Physic2DFixture.h"

///
// Box2D
//

#include "Box2D/Dynamics/b2Fixture.h"

class Physic2DBody;

class Physic2DFixtureB2D : public Physic2DFixture
{
public:
	Physic2DFixtureB2D(Physic2DBody* body, Physic2DFixtureProperties properties) : Physic2DFixture(body, properties) 
	{
		_b2Fixture = NULL;
		_b2FixtureDef.density = properties.density;
		_b2FixtureDef.restitution = properties.restitution;
		_b2FixtureDef.friction = properties.friction;
		_b2FixtureDef.isSensor = properties.isSensor;
		_b2FixtureDef.userData = _data;
		_b2FixtureDef.shape = (const b2Shape*)properties.shape->GetShape();
		_properties = properties;
	};

	virtual Physic2DShape*					GetShape() const override;
	virtual const Physic2DFixtureProperties GetProperties() const override;
	void									SetFixtureDefB2D(const b2FixtureDef& fixtureDef);
	const b2FixtureDef&						GetFixtureDefB2D() const;
	void									SetFixtureB2D(b2Fixture* fixture);

private:

	Physic2DFixtureProperties	_properties;
	b2FixtureDef				_b2FixtureDef;
	b2Fixture*					_b2Fixture;
};