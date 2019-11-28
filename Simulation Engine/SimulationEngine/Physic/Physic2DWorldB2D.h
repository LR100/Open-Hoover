#pragma once

#include "Physic2DBody.h"
#include "Physic2DWorld.h"


#include <unordered_map>
#include <deque>
#include <set>

///
// Box2D
//
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2World.h"


#include <iostream> // TMP debug

class Physic2DWorldB2D : public Physic2DWorld
{
public:
	friend class Physic2DBodyB2D;

	Physic2DWorldB2D(Vec2 gravity = Vec2(0, 9.8f));
	
	~Physic2DWorldB2D()
	{
		if (_b2World != NULL) {
			delete _b2World;
			_b2World = NULL;
		}
	}


	/// BASICS
	virtual void					Simulate(float timePassed) override;

	/// GET
	//PX2BodyManager *		GetBodyManager() const;

	/// SET
	virtual Physic2DBody*				CreateBody(Physic2DBodyProperties& properties) override;
	virtual std::vector<Physic2DBody*>	GetBodies() const override;
	virtual Physic2DBody*				GetBody(const size_t& id) const override;
	virtual void						GetBodiesInAABB(Physic2DQueryBodyCb* querybody, const AABB2& aabb) const override;
	virtual void						GetBodiesAtPosition(Physic2DQueryBodyCb* querybody, const Vec2& pos) const override;

	virtual void						DestroyBody(Physic2DBody* body) override;
	virtual void						DestroyAllBodies() override;


	virtual void						SetGravity(const Vec2& gravity) override;
	virtual Vec2						GetGravity() const override;

	virtual Physic2DShapeCircle*		CreateShapeCircle() override;
	virtual Physic2DShapeBox*			CreateShapeBox() override;
	virtual Physic2DShapePolygon*		CreateShapePolygon() override;


	virtual RaytraceOutput			Raytrace(const Vec2& origin, const Vec2& dir, const float& maxDist) override;

	// Remove Everything in the World and Free Bodies (DO NOT free Shapes)
	virtual void					Clear() override;

protected:


private:

	class RaytraceQuery : public b2QueryCallback
	{
	public:
		RaytraceQuery(b2World* world, const Vec2& origin, const Vec2& dir, const float& maxDist);

		bool				ReportFixture(b2Fixture* fixture);
		RaytraceOutput		Raytrace();
		const size_t&		GetBodyID() const;

	private:

		// Results
		bool				_found;
		b2RayCastOutput		_b2output;
		RaytraceOutput		_output;
		size_t				_bodyID;
		// Compute
		Vec2				_move;
		size_t				_step;
		b2RayCastInput		_b2input;
		float				_distStep;
		float				_maxDist;
		b2World*			_world;
		Vec2				_origin;
		Vec2				_dir;
	};

	class AABBQuery : public b2QueryCallback
	{
	public:
		AABBQuery(Physic2DQueryBodyCb* querybody, Physic2DWorld* world, b2World* b2world, const AABB2& aabb);
		bool								ReportFixture(b2Fixture* fixture);
		void								Query();

	private:

		// Results
		std::set<size_t>			_bodiesID;
		b2AABB						_b2aabb;
		b2World*					_b2World;
		Physic2DWorld*				_world;
		// Compute
		Physic2DFixture::UserData*  _userData;
		size_t						_bodyID;
		Physic2DQueryBodyCb*		_querybody;
	};

	
	size_t						GetBodyAvailableID();

	b2World*					_b2World;
	
	int32						_velocityIterations;  //how strongly to correct velocity
	int32						_positionIterations;

	std::unordered_map<size_t, Physic2DBody*>	_bodiesMAP;
	std::deque<size_t>							_bodiesAvailableIDs;
	size_t										_bodyCountMax;
};
