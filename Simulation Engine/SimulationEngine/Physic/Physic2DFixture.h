#pragma once

#include "Physic2DShape.h"
struct Physic2DFixtureProperties
{
	Physic2DFixtureProperties()
	{
		userdata = NULL;
		friction = 0;
		restitution = 0;
		density = 1.0f;
		isSensor = false;
		shape = NULL;
	}
	void*			userdata;
	float			friction;
	float			restitution;
	float			density;
	bool			isSensor;
	Physic2DShape*	shape;
};

class Physic2DBody;

class Physic2DFixture
{
public:
	Physic2DFixture(Physic2DBody* body, Physic2DFixtureProperties properties) {};

	virtual const Physic2DShape* GetShape() const = 0;

private:
	Physic2DFixture();
};