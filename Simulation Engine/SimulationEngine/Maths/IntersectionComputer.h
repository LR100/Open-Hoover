#pragma once

#include "Vector.h"
#include "VectorTransformer.h"
#include "AABB.h"
#include "Line.h"

// Should probably be deprecated (DO an intersection function -> Like Intersection Computer Tool)
struct LineAABBNearestIntersectionComputer
{


	uint8_t		intersectionSide; // 0 Top - 1 Bot - 2 Left - 3 Right
	Vec2		intersections[4]; // 4 For All Possible Solutions
	uint8_t		intersectionCount;
	float		intersectionDistSquared;


	void		Reset();

	bool		Intersect(const AABB2& aabb, const Vec2& lA, const Vec2& lB);

	bool		IntersectSideFromDir(const AABB2& aabb, const Vec2& lA, const Vec2& lB, const Vec2& dir);

	bool		IntersectTop(const AABB2& aabb, const Vec2& lA, const Vec2& lB);
	bool		IntersectBot(const AABB2& aabb, const Vec2& lA, const Vec2& lB);
	bool		IntersectLeft(const AABB2& aabb, const Vec2& lA, const Vec2& lB);
	bool		IntersectRight(const AABB2& aabb, const Vec2& lA, const Vec2& lB);


private:

	bool		Intersect(const uint8_t& side, const Vec2& lA, const Vec2& lB);

	Vec2		_testPoints[2];
	Vec2		_resPoint;
	Vec2		_lAToRes;


	static const Vec2 _topDir;
	static const Vec2 _rightDir;

	float		_angle;
	float		_dist;
};

class IntersectionComputer
{
public:
	IntersectionComputer();
	virtual ~IntersectionComputer();

	static bool		SegmentSegment(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, Vec2& res);
	// Segment vs Segment with Colllinear Managment;
	static bool		SegmentSegmentC(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, Vec2& res);

	static bool		LineLine(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, Vec2& res);
	static bool		LineCircle(const Vec2& O, const Vec2& D, const Vec2& C, float radius, float t[2], Vec2 point[2], Vec2 normal[2]);
	static bool		ClosestPointOnLine(const Vec2& origin, const Vec2& lA, const Vec2& lB, Vec2& point);
	
	static bool		RayAABB(const Vec2& o, const Vec2& dir, const AABB2& aabb, float t[2], Vec2 point[2], Vec2 normal[2], const float& maxDist = 1000000.0f);
	static bool		RayInsideAABB(const Vec2& o, const Vec2& dir, const AABB2& aabb, Vec2& res, const float& maxDist = 1000000.0f);

private:

	static float	Det(const float& a, const float& b, const float& c, const float& d);


	static float			_bFlt[10];
	static double			_bDbl[10];
	// Segment Intersection
	
};
