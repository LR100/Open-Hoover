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
	Physic2DFixture(Physic2DBody* body, Physic2DFixtureProperties properties);

	class UserData
	{
		friend class Physic2DFixture;
	public:
		UserData() {
			data = NULL;
			bodyID = SIZE_MAX;
		}
		void*			data;

		const size_t&	GetBodyID() const {
			return (bodyID);
		}
		Physic2DFixture* GetFixture() const {
			return (self);
		}
	private:
		Physic2DFixture*	self;
		size_t				bodyID; // Keep track of parent Body (something that Box2D is not helpful with)
	};

	virtual void*									GetUserData() const { return (_data->data); };
	virtual Physic2DBody*							GetBody() const = 0;
	virtual Physic2DShape*							GetShape() const = 0;
	virtual const Physic2DFixtureProperties			GetProperties() const = 0;

protected:
	UserData*		_data; 
private:
	Physic2DFixture();
	
};