#pragma once

#include "Physic2DBody.h"
#include "Physic2DWorld.h"

///
// Box2D
//
#include "Box2D/Dynamics/b2Body.h"

#include <iostream> // Tmp Debug

class Physic2DWorldB2D;

class Physic2DBodyB2D : public Physic2DBody
{
public:

	Physic2DBodyB2D(Physic2DWorld* world, Physic2DBodyProperties& properties) : Physic2DBody(world, properties) 
	{
		_id = 0;
		_properties = properties;
		if (properties.type == properties.DYNAMIC) {
			std::cout << "Dynamic Object" << std::endl;
			_b2BodyDef.type = b2_dynamicBody; //this will be a dynamic body
			properties.allowSleep = false;
		}
		else {
			_b2BodyDef.type = b2_staticBody; //this will be a static body
		}
		_b2BodyDef.linearDamping = properties.linearDamping;
		_b2BodyDef.allowSleep = properties.allowSleep;
		_b2BodyDef.position.Set(properties.position.x, properties.position.y); //set the starting position
		_b2BodyDef.angle = properties.angle;
		_b2Body = NULL;
		_world = (Physic2DWorldB2D*)world;
	};
	virtual ~Physic2DBodyB2D() {}

	virtual Physic2DWorld*	GetWorld() const override;
	virtual const size_t&	GetID() const override;
	
	//void				SetShape(Physic2DShape* shape);
	//const Physic2DShape*		GetShape() const;

	

	//virtual void				ResetPosition(const Vec2& position) = 0;
	virtual void				SetPosition(const Vec2& position) override;
	virtual Vec2				GetPosition() const override;
	//virtual const Vec2&			GetPositionOld() const;

	//virtual const Vec2&			GetDir() const;
	//virtual const Vec2&			GetDirNorm() const;
	//virtual const Vec2&			GetDirParallelNorm() const;


	//virtual void				ApplyForce(const Vec2& force) = 0;
	//virtual const Vec2&			GetForce() const;

	//virtual void				ApplyImpulseLinear(const Vec2& impulse) = 0;

	// Inherited via Physic2DBody
	virtual void		SetVelocityLinear(const Vec2& velocity) override;
	virtual Vec2		GetVelocityLinear() const override;
	////const Vec2&		GetVelocityLinearOld() const;


	//virtual void				Move(const float& dt) = 0;
	//virtual void				Move(const float& dt, const Vec2& gravity) = 0;

	//virtual void				SetSleeping(bool state) = 0;
	//virtual void				SetColliding(bool state) = 0;

	//virtual void						SetSweptVolume(const AABB2& sweptVolume) = 0;
	//virtual const AABB2&		GetSweptVolume() const;

	//virtual const AABB2&		GetAABB() const;
	virtual const Physic2DBodyProperties::TYPE& GetType() const override;
	virtual const Physic2DBodyProperties&		GetProperties() const override;
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

	virtual const std::vector<Physic2DFixture*>&	GetFixtures() const override;
	
	virtual Physic2DFixture*						CreateFixture(const Physic2DFixtureProperties& properties) override;


	/*void			SetTypeVelocity() = 0;
	void			SetDir() = 0;*/

	//size_t			_id;
	////Physic2DShape*		_shape;
	
	void				SetIDB2D(const size_t& id);
	void				SetBodyB2D(b2Body* body);
	const b2BodyDef&	GetBodyDefB2D() const;
	b2Body*				GetBodyB2D() const;

protected:


private:
	
	Physic2DWorldB2D*				_world;
	std::vector<Physic2DFixture*>	_fixtures;

	Physic2DBodyProperties			_properties;
	b2Body*							_b2Body;
	b2BodyDef						_b2BodyDef;
	size_t							_id;

	

};


