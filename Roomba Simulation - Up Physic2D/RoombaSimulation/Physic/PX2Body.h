#pragma once

#include "PX2Shapes.h"
#include "../Maths/Vector.h"


//#include "../Tools/DList.h" // For Colliders List


class PX2World;

// By default bodies have no shapes
class PX2Body
{
public:
	friend class PX2BodyManager;

	enum TYPE : uint8_t
	{
		STATIC = 0,
		DYNAMIC = 1
	};

	enum VELOCITY : uint8_t
	{
		SLEEPING = 0,
		LOW = 1,
		HIGH = 2
		// BULLET
	};

	struct Properties
	{
		Properties();

		TYPE		type;
		Vec2		position;
		Vec2		velocityLinear;
		float		mass;
		float		restitution;
		float		friction;
		PX2Shape*	shape;
	};

	PX2Body(PX2World* world, Properties & properties);
	~PX2Body();

	void				SetShape(PX2Shape* shape);
	const PX2Shape*		GetShape() const;

	void				ResetPosition(const Vec2& position);
	void				SetPosition(const Vec2& position);
	const Vec2&			GetPosition() const;
	const Vec2&			GetPositionOld() const;

	const Vec2&			GetDir() const;
	const Vec2&			GetDirNorm() const;
	const Vec2&			GetDirParallelNorm() const;


	void				ApplyForce(const Vec2& force);
	const Vec2&			GetForce() const;

	void				ApplyImpulseLinear(const Vec2& impulse);


	void				SetVelocityLinear(const Vec2& velocity);
	const Vec2&			GetVelocityLinear() const;
	//const Vec2&		GetVelocityLinearOld() const;


	void				Move(const float& dt);
	void				Move(const float& dt, const Vec2& gravity);

	void				SetSleeping(bool state);
	void				SetColliding(bool state);

	void				SetSweptVolume(const AABB2& sweptVolume);
	const AABB2&		GetSweptVolume() const;

	const AABB2&		GetAABB() const;
	const TYPE&			GetType() const;
	const size_t&		GetID() const;

	void				SetRestitution(const float& restitution);
	const float&		GetRestitution() const;

	void				SetMass(const float& mass);
	const float&		GetMass() const;
	const float&		GetInvMass() const;

	void				SetFriction(const float& friction); // By Default 0 Friction
	const float&		GetFriction() const;

	const bool&			IsHighVelocity() const;
	const bool&			IsHighVelocityInWorld() const;
	const bool&			IsSleeping() const;
	const bool&			IsColliding() const;
	const bool&			WasSleeping() const;

	std::string			ToString() const;

private:

	void			SetTypeVelocity();
	void			SetDir();

	size_t			_id;
	PX2Shape*		_shape;
	PX2World*		_world;

protected:

	// Properties
	TYPE			_type;
	float			_invMass;
	float			_mass;
	float			_restitution;
	float			_friction;
	
	// Dynamic Infos
	

	AABB2 			_aabb;
	Vec2			_positionOld;

	AABB2 			_sweptVolume;
	
	Vec2			_centroidLocal;
	Vec2			_centroidWorld;

	Vec2			_dir;
	Vec2			_dirParallelNorm;
	Vec2			_dirNorm;

	Vec2			_forceAccumulator;
	
	Vec2			_velocityLinear;
	float			_velocityAngular;

	float			_torque;
	float			_angle;

	float			_lengthVelocity;
	bool			_isHighVelocity;
	bool			_isHighVelocityInWorld;

	bool			_isSleeping;
	bool			_wasSleeping;
};