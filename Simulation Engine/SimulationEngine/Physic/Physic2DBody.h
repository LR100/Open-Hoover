#pragma once


#include <vector>

//#include "Physic2DShapes.h"
#include "../Maths/Vector.h"
#include "../Maths/AABB.h"

//#include "../Tools/DList.h" // For Colliders List

#include "Physic2DFixture.h"


class Physic2DWorld;

struct Physic2DBodyProperties
{
	Physic2DBodyProperties()
	{
		type = STATIC;
		velocityLinear.x = 0;
		velocityLinear.y = 0;
		position.x = 0;
		position.y = 0;
		angle = 0;
		allowSleep = true;
		fixedRotation = true;
		linearDamping = 0;
	}

	enum TYPE : uint8_t
	{
		STATIC = 0,
		DYNAMIC = 1
	};

	// Physic2DBodyProperties();

	TYPE		type;
	Vec2		position;
	Vec2		velocityLinear;
	float		angle;
	bool		allowSleep;
	float		linearDamping;
	bool		fixedRotation;
	//Physic2DShape*	shape;
};
// By default bodies have no shapes
class Physic2DBody
{
public:
	Physic2DBody(Physic2DWorld* world, Physic2DBodyProperties& properties) {};

private:
	Physic2DBody();

public:
	virtual ~Physic2DBody() {};

	virtual Physic2DWorld*	GetWorld() const = 0;
	virtual const size_t&	GetID() const = 0;

	//void				SetShape(Physic2DShape* shape);
	//const Physic2DShape*		GetShape() const;
	virtual Physic2DFixture*	CreateFixture(const Physic2DFixtureProperties& properties) = 0;

	//virtual void				ResetPosition(const Vec2& position) = 0;
	virtual void				SetPosition(const Vec2& position) = 0;
	virtual Vec2				GetPosition() const = 0;
	//virtual const Vec2&		GetPositionOld() const;

	//virtual const Vec2&			GetDir() const;
	//virtual const Vec2&			GetDirNorm() const;
	//virtual const Vec2&			GetDirParallelNorm() const;


	//virtual void				ApplyForce(const Vec2& force) = 0;
	//virtual const Vec2&			GetForce() const;

	//virtual void				ApplyImpulseLinear(const Vec2& impulse) = 0;

	
	virtual void					SetVelocityLinear(const Vec2& velocity) = 0;
	virtual Vec2					GetVelocityLinear() const = 0;
	////const Vec2&		GetVelocityLinearOld() const;


	//virtual void				Move(const float& dt) = 0;
	//virtual void				Move(const float& dt, const Vec2& gravity) = 0;

	//virtual void				SetSleeping(bool state) = 0;
	//virtual void				SetColliding(bool state) = 0;

	//virtual void						SetSweptVolume(const AABB2& sweptVolume) = 0;
	//virtual const AABB2&		GetSweptVolume() const;

	//virtual const AABB2&		GetAABB() const;
	virtual const Physic2DBodyProperties::TYPE&			GetType() const = 0;
	virtual const Physic2DBodyProperties&				GetProperties() const = 0;
	//virtual const size_t&		GetID() const;

	//virtual void				SetRestitution(const float& restitution) = 0;
	//virtual const float&		GetRestitution() const;

	//virtual void				SetMass(const float& mass) = 0;
	//virtual const float&		GetMass() const;
	//virtual const float&		GetInvMass() const;

	//virtual void				SetFriction(const float& friction) = 0; // By Default 0 Friction
	//virtual const float&		GetFriction() const;

	//virtual const bool&			IsHighVelocity() const;
	//virtual const bool&			IsHighVelocityInWorld() const;
	//virtual const bool&			IsSleeping() const;
	//virtual const bool&			IsColliding() const;
	//virtual const bool&			WasSleeping() const;

	//virtual std::string			ToString() const;

	virtual const std::vector<Physic2DFixture*>&	GetFixtures() const = 0;

private:

	/*void			SetTypeVelocity() = 0;
	void			SetDir() = 0;*/

	//size_t			_id;
	////Physic2DShape*		_shape;
	//Physic2DWorld*		_world;

protected:


};

