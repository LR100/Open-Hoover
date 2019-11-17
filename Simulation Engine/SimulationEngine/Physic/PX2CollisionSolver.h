#pragma once

#include "../Maths/IntersectionComputer.h"

#include "PX2ManifoldList.h"
#include "PX2CollisionListener.h"

class PX2CollisionSolver
{
public:

	PX2CollisionSolver(PX2CollisionListener* collisionListener);
	~PX2CollisionSolver();

	
	void Solve(const PX2ManifoldList& manifoldList, const float& dt);
	
	
private:

	void	ResolveCollisions(const float& dt);

	//void Solve(PX2Manifold* manifold, const float& dt);

	// Dynamic vs Dynamic //
	void TestDD(PX2Manifold* manifold, const float& dt);
	void ResolveDD(PX2Manifold* manifold, const float& dt);
	/// Fast
	void TestFastCDCD(PX2Manifold* manifold, const float& dt);
	void ResolveFastCDCD(PX2Manifold* manifold, const float& dt);
	void TestFastCDBD(PX2Manifold* manifold, const float& dt);
	void TestFastBDBD(PX2Manifold* manifold, const float& dt);
	/// Slow
	void TestSlowCDCD(PX2Manifold* manifold, const float& dt);
	void ResolveSlowCDCD(PX2Manifold* manifold, const float& dt);
	void TestSlowCDBD(PX2Manifold* manifold, const float& dt);
	void TestSlowBDBD(PX2Manifold* manifold, const float& dt);


	// Dynamic vs Static //
	void TestDS(PX2Manifold* manifold, const float& dt);
	void ResolveDS(PX2Manifold* manifold, const float& dt);
	/// Fast
	void TestFastCDCS(PX2Manifold* manifold, const float& dt);
	void ResolveFastCDCS(PX2Manifold* manifold, const float& dt);
	void TestFastCDBS(PX2Manifold* manifold, const float& dt);
	void ResolveFastCDBS(PX2Manifold* manifold, const float& dt);
	void TestFastBDCS(PX2Manifold* manifold, const float& dt);
	void TestFastBDBS(PX2Manifold* manifold, const float& dt);

	/// Slow
	void TestSlowCDCS(PX2Manifold* manifold, const float& dt);
	void TestSlowCDBS(PX2Manifold* manifold, const float& dt);
	int TestSlowCDBSEdge(PX2Manifold * manifold, const float & dt, const Vec2& a, const Vec2& b, const Vec2& edgeA, const Vec2& edgeB, const Vec2& edgeNormal, Vec2& inter);
	void ResolveSlowCDBS(PX2Manifold* manifold, const float& dt);
	void TestSlowBDCS(PX2Manifold* manifold, const float& dt);
	void TestSlowBDBS(PX2Manifold* manifold, const float& dt);


	// Swap Bodies to have A Dynamic
	void SwapDynamicToA(PX2Manifold* manifold);
	// Swap Bodies to have A Sleeping
	void SwapSleepingToA(PX2Manifold* manifold);

	// Static vs Static // -> IGNORE
	void TestSS(PX2Manifold* manifold, const float& dt);
	

	void ClassicDDResolveCollision(PX2Manifold* manifold, const float& dt);
	void ClassicDSResolveCollision(PX2Manifold* manifold, const float& dt);

	// Others Tools Methods

	// Collision Handling



	// Look for nearest point in line Segment intersecting with aabb from start point 
	
	

	/// Some Compute Buffer Variable (to avoid creating them every Time
	// For LineAABB2Testing
	LineAABBNearestIntersectionComputer	_lanic;

	// Others
	PX2Body*			_swapBuffer;

	// For Resolution
	Vec2			_impulse;
	Vec2			_rv;
	Vec2			_correction;
	float			_velAlongNormal;
	float			_e;
	float			_j;
	float			_ratio;
	float			_sumMass;

	// For Collision Handling
	//uint8_t		_typeMove;

	Vec2			_collision;

	Vec2			_normal;
	Vec2			_nVelA;
	Vec2			_nVelB;

	// TMP Buffers
	float			_bFlt[10];
	

	Vec2 _normalsBox[4] = { Vec2(0, -1), Vec2(0, 1), Vec2(-1, 0), Vec2(1, 0) };

	
	// Test Function

	static const size_t TYPE_MOVE_COUNT = 3;
	

	typedef void (PX2CollisionSolver::*TestFunction)(PX2Manifold* manifold, const float& dt);
	typedef void (PX2CollisionSolver::*ResolveFunction)(PX2Manifold* manifold, const float& dt);
	

	PX2CollisionListener*		_collisionListener;

	TestFunction				_testTypeMoveFunctions[TYPE_MOVE_COUNT];
	ResolveFunction				_resolveTypeMoveFunctions[TYPE_MOVE_COUNT];
	//TestFunctionCollision		_testTypeMoveFunctionsCollision[TYPE_MOVE_COUNT];


	/// High Velocity Functions

	// Dynamic VS Dynamic
	TestFunction				_testFastDD[PX2SHAPE_TYPE_COUNT][PX2SHAPE_TYPE_COUNT];
	ResolveFunction				_resolveFastDD[PX2SHAPE_TYPE_COUNT][PX2SHAPE_TYPE_COUNT];
	
	// Dynamic VS Static							  // Every Type VS Every Type (ex: BD vs CD, BS vs BD, ...)
	TestFunction				_testFastDS[PX2SHAPE_TYPE_COUNT][PX2SHAPE_TYPE_COUNT];
	ResolveFunction				_resolveFastDS[PX2SHAPE_TYPE_COUNT][PX2SHAPE_TYPE_COUNT];

	/// Low Velocity Functions
	// Dynamic VS Dynamic
	TestFunction				_testSlowDD[PX2SHAPE_TYPE_COUNT][PX2SHAPE_TYPE_COUNT];
	ResolveFunction				_resolveSlowDD[PX2SHAPE_TYPE_COUNT][PX2SHAPE_TYPE_COUNT];
	// Dynamic VS Static
	TestFunction				_testSlowDS[PX2SHAPE_TYPE_COUNT][PX2SHAPE_TYPE_COUNT];
	ResolveFunction				_resolveSlowDS[PX2SHAPE_TYPE_COUNT][PX2SHAPE_TYPE_COUNT];
};
