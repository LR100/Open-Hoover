#include "PX2Body.h"

#include "PX2World.h"
#include "PX2BodyManager.h"
#include "../Maths/VectorTransformer.h"


#include <algorithm>
#include <iostream> // Tmp 
#include <sstream> // For ToString() function

#define PX2BODY_MIN_RESTITUTION 0.00000000001f
#define PX2BODY_MIN_MASS 0.0f

PX2Body::PX2Body(PX2World * world, Properties & properties)
{
	_world = world;
	_type = properties.type;
	_id = _world->GetBodyManager()->GetBodyAvailableID();
	if (_id == SIZE_MAX)
	{
		std::cerr << "ERROR PX2Body Have INVALID ID (world is probably Full)" << std::endl;
	}
	_isHighVelocity = false;
	_isHighVelocityInWorld = false;
	_velocityAngular = 0.0f;
	_isSleeping = false;
	_wasSleeping = false;

	_aabb.center = properties.position;
	_positionOld = properties.position;
	_shape = properties.shape;

	if (_shape != NULL)
	{
		_aabb.halfSize = _shape->GetAABB().halfSize;
		_aabb.ComputeMinMax();
	}

	_velocityLinear = Vec2(0, 0);
	SetVelocityLinear(properties.velocityLinear);
	SetMass(properties.mass);
	SetRestitution(properties.restitution);
	SetFriction(properties.friction);

	//std::cout << "Create new body with id: (" << _id << ")\n";

	/// EVERYTHING BEFORE INIT IN BODY MANAGER
	_world->GetBodyManager()->AddBody(this);
	if (_type == PX2Body::TYPE::STATIC)
	{
		_velocityLinear = Vec2(0, 0);
	}
}

PX2Body::~PX2Body()
{
	//_world->GetBodyManager()->DestroyBody(_id);
}

void PX2Body::SetShape(PX2Shape * shape)
{
	_shape = shape;
	_aabb.halfSize = shape->GetAABB().halfSize;
	_aabb.ComputeMinMax();
}

const PX2Shape * PX2Body::GetShape() const
{
	return (_shape);
}

void PX2Body::ResetPosition(const Vec2 & position)
{
	_aabb.center = position;
	_positionOld = position;
	_aabb.ComputeMinMax();
}

void PX2Body::SetPosition(const Vec2 & position)
{
	// Notify BodyManager that position Changed;
	_aabb.center = position;
}

const Vec2 & PX2Body::GetPosition() const
{
	return (_aabb.center);
}

const Vec2 & PX2Body::GetPositionOld() const
{
	return (_positionOld);
}

const Vec2 & PX2Body::GetDir() const
{
	return (_dir);
}

const Vec2 & PX2Body::GetDirNorm() const
{
	return (_dirNorm);
}

const Vec2 & PX2Body::GetDirParallelNorm() const
{
	return (_dirParallelNorm);
}

void PX2Body::SetVelocityLinear(const Vec2 & velocity)
{
	
	// Notify BodyManager that Velocity Changed;
	_velocityLinear = velocity;
	SetTypeVelocity();
	//_force = Vec2(0, 0);
}

const Vec2 & PX2Body::GetVelocityLinear() const
{
	return (_velocityLinear);
}

const bool& PX2Body::IsHighVelocity() const
{
	return (_isHighVelocity);
}

const bool & PX2Body::IsHighVelocityInWorld() const
{
	return (_isHighVelocityInWorld);
}

const bool & PX2Body::IsSleeping() const
{
	return (_isSleeping);
}

const bool & PX2Body::WasSleeping() const
{
	return (_wasSleeping);
}

std::string PX2Body::ToString() const
{
	std::stringstream ss;
	ss << "Position " << _aabb.center.ToString() << std::endl;
	ss << "Position Old " << _positionOld.ToString() << std::endl;
	ss << "Dir " << _dir.ToString() << std::endl;
	ss << "Dir Norm" << _dirNorm.ToString() << std::endl;
	
	return (ss.str());
}

void PX2Body::SetSleeping(bool state)
{
	_wasSleeping = _isSleeping;
	_isSleeping = state;
}

const AABB2& PX2Body::GetAABB() const
{
	return (_aabb);
}

const PX2Body::TYPE & PX2Body::GetType() const
{
	return (_type);
}

void PX2Body::SetMass(const float & mass)
{
	_mass = mass;

	if (_mass < PX2BODY_MIN_MASS)
		_mass = PX2BODY_MIN_MASS;

	if (_mass == 0)
		_invMass = 0;
	else
		_invMass = 1.0f / _mass;
}

const float & PX2Body::GetMass() const
{
	return (_mass);
}

const float & PX2Body::GetInvMass() const
{
	return (_invMass);
}

void PX2Body::SetFriction(const float & friction)
{
	_friction = friction;
}

const float & PX2Body::GetFriction() const
{
	return (_friction);
}

void PX2Body::SetRestitution(const float & restitution)
{
	_restitution = restitution;
	if (_restitution > 1.0f)
		_restitution = 1.0f;
	else if (_restitution < PX2BODY_MIN_RESTITUTION)
		_restitution = PX2BODY_MIN_RESTITUTION;
}

const float & PX2Body::GetRestitution() const
{
	return (_restitution);
}

void PX2Body::ApplyForce(const Vec2 & force)
{
	_forceAccumulator += force;
}

const Vec2 & PX2Body::GetForce() const
{
	return (_forceAccumulator);
}

void PX2Body::ApplyImpulseLinear(const Vec2 & impulse)
{
	_forceAccumulator += (impulse * _invMass);
}

const size_t & PX2Body::GetID() const
{
	return (_id);
}

void PX2Body::Move(const float & dt)
{
	//_acceleration =

	//_velocityLinearOld = _velocityLinear;

	// Must Recompute if is HighVelocity
	//SetVelocityLinear((_velocityLinear + (_acceleration * dt)));

	_velocityLinear += _forceAccumulator * dt;

	_positionOld = _aabb.center;
	_aabb.center += _velocityLinear * dt;

/*
	_aabb.center.x = _aabb.center.x + ((_velocityLinear.x * 0.75f) + (_velocityLinearOld.x * 0.25f)) * dt;
	_aabb.center.y = _aabb.center.y + ((_velocityLinear.y * 0.75f) + (_velocityLinearOld.y * 0.25f))* dt;*/


	
	// Reset Forces
	_forceAccumulator.x = 0;
	_forceAccumulator.y = 0;

	

	SetDir();
	//SetTypeVelocity();
}

void PX2Body::Move(const float & dt, const Vec2 & gravity)
{
	ApplyForce(gravity);
	Move(dt);
}

void PX2Body::SetSweptVolume(const AABB2& sweptVolume)
{
	_sweptVolume = sweptVolume;
}

const AABB2& PX2Body::GetSweptVolume() const
{
	return (_sweptVolume);
}

void PX2Body::SetTypeVelocity()
{
	// Type Velocity is reffered on Dir (Move Step) (SHOULD BE !!!)
	_lengthVelocity = std::fabs(_velocityLinear.x ) + std::fabs(_velocityLinear.y );
//	_isHighVelocity = false; // TMPPP
	if (_shape && _lengthVelocity >= _shape->GetHighVelocityValue())
	{
		_isHighVelocity = true;
		SetSleeping(false);
	}
	else
	{
		_isHighVelocity = false;
	}
	if (_lengthVelocity >= _world->GetBodyManager()->GetHighVelocityLength())
	{
		_isHighVelocityInWorld = true;
	}
	else
	{
		_isHighVelocityInWorld = false;
	}
}

void PX2Body::SetDir()
{
	_dir = (_aabb.center - _positionOld);
	VectorTransformer::Normalize(_dir, _dirNorm);
	_dirParallelNorm = _dirNorm;
	VectorTransformer::Rotate(_dirParallelNorm, 90.0f);
}

PX2Body::Properties::Properties()
{
	type = DYNAMIC;
	shape = NULL;
	restitution = 0.4f;
	mass = 1.0f;
	friction = 0.5f;
	velocityLinear = Vec2(0, 0);
}
