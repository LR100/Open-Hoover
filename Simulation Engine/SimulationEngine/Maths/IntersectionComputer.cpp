#include "IntersectionComputer.h"


#include "../Physic/PX2DebugDrawer.h" // TMP ULTRAAA

IntersectionComputer::IntersectionComputer()
{
}

IntersectionComputer::~IntersectionComputer()
{
}

bool IntersectionComputer::SegmentSegment(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d, Vec2 & res)
{
	double			s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;

	s10_x = b.x - a.x;
	s10_y = b.y - a.y;
	s32_x = d.x - c.x;
	s32_y = d.y - c.y;

	denom = s10_x * s32_y - s32_x * s10_y;
	if (denom == 0)
	{
		return (false);
	}
	bool denomPositive = denom > 0;

	s02_x = a.x - c.x;
	s02_y = a.y - c.y;
	s_numer = s10_x * s02_y - s10_y * s02_x;
	if ((s_numer < 0) == denomPositive)
	{
		//std::cout << "((s_numer < 0) == denomPositive)" << std::endl;
		return (false); // No collision
	}

	t_numer = s32_x * s02_y - s32_y * s02_x;
	if ((t_numer < 0) == denomPositive)
	{
		//std::cout << "((t_numer < 0) == denomPositive)" << std::endl;
		return (false); // No collision
	}

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
	{
		//std::cout << "((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive)" << std::endl;
		return (false); // No collision
	}
	// Collision detected
	t = t_numer / denom;

	res.x = (float)a.x + (t * s10_x);
	res.y = (float)a.y + (t * s10_y);

	return (true);
}



bool IntersectionComputer::SegmentSegmentC(const Vec2& p, const Vec2& p2, const Vec2& q, const Vec2& q2, Vec2& res)
{
	Vec2 r = p2 - p;
	Vec2 s = q2 - q;

	float rxs = r.Cross(s);
	float qpxr = (q - p).Cross(r);

	// If r x s = 0 and (q - p) x r = 0, then the two lines are collinear.
	if (rxs == 0 && qpxr == 0)
	{
		// 1. If either  0 <= (q - p) * r <= r * r or 0 <= (p - q) * s <= * s
		// then the two lines are overlapping,

		if ((0 <= (q - p).Dot(r) && (q - p).Dot(r) <= r.Dot(r) || (0 <= (p - q).Dot(s) && (p - q).Dot(s) <= s.Dot(s))))
		{
			res = p2; // Set P2
			return true;
		}

		// 2. If neither 0 <= (q - p) * r = r * r nor 0 <= (p - q) * s <= s * s
		// then the two lines are collinear but disjoint.
		// No need to implement this expression, as it follows from the expression above.
		return false;
	}

	// 3. If r x s = 0 and (q - p) x r != 0, then the two lines are parallel and non-intersecting.
	if (rxs == 0 && !(qpxr == 0))
		return false;

	// t = (q - p) x s / (r x s)
	float t = (q - p).Cross(s) / rxs;

	// u = (q - p) x r / (r x s)

	float u = (q - p).Cross(r) / rxs;

	// 4. If r x s != 0 and 0 <= t <= 1 and 0 <= u <= 1
	// the two line segments meet at the point p + t r = q + u s.
	if (!rxs == 0 && (0 <= t && t <= 1) && (0 <= u && u <= 1))
	{
		// We can calculate the intersection point using either t or u.
		res = p + (r * t);

		// An intersection was found.
		return (true);
	}

	// 5. Otherwise, the two line segments are not parallel but do not intersect.
	return (false);
}

bool IntersectionComputer::LineLine(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d, Vec2 & res)
{
	static float			s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;

	s10_x = b.x - a.x;
	s10_y = b.y - a.y;
	s32_x = d.x - c.x;
	s32_y = d.y - c.y;

	denom = s10_x * s32_y - s32_x * s10_y;
	if (denom == 0)
		return (false); // Collinear


	s02_x = a.x - c.x;
	s02_y = a.y - c.y;

	s_numer = s10_x * s02_y - s10_y * s02_x;
	t_numer = s32_x * s02_y - s32_y * s02_x;

	// Collision detected
	t = t_numer / denom;

	res.x = a.x + (t * s10_x);
	res.y = a.y + (t * s10_y);

	return (true);
}

bool IntersectionComputer::LineCircle(const Vec2 & O, const Vec2 & D, const Vec2 & C, float radius, float t[2], Vec2 point[2], Vec2 normal[2])
{
	Vec2 d = O - C;
	float a = D.Dot(D);
	float b = d.Dot(D);
	float c = d.Dot(d) - radius * radius;

	t[0] = FLT_MAX;
	t[1] = FLT_MAX;
	/*PX2DebugDrawer::DrawVec2Vec2(O, O + D, Color(55, 55, 255));
	PX2Drawer->DrawCircle(C.x, C.y, radius * 2, Color(20, 200, 120));
	PX2Window->Refresh();*/

	//system("pause");

	float disc = b * b - a * c;
	if (disc < 0.0f) {
		return false;
	}

	float sqrtDisc = ValueTools::FastSqrt(disc);
	float invA = 1.0f / a;
	t[0] = (-b - sqrtDisc) * invA;
	t[1] = (-b + sqrtDisc) * invA;

	if (t[1] < t[0] && t[1] > 0)
	{
		static float tmp = t[1];
		t[1] = t[0];
		t[0] = tmp;
	}

	if (t[0] < 0.0f)
		return (false);

	//std::cout << "T[0] (" << t[0] << ")" << std::endl;

	float invRadius = 1.0f / radius;
	for (int i = 0; i < 2; ++i)
	{
		point[i] = O + D * t[i];
		normal[i] = (point[i] - C) * invRadius;
	}

	/*PX2Drawer->DrawCircle(point[0].x, point[0].y, 4, Color(200, 200, 20));
	PX2Window->Refresh();*/

	//system("pause");
	return (true);
}

bool IntersectionComputer::ClosestPointOnLine(const Vec2 & origin, const Vec2 & lA, const Vec2 & lB, Vec2 & point)
{
	float A1 = lB.y - lA.y;
	float B1 = lA.x - lB.x;
	double C1 = (lB.y - lA.y) * lA.x + (lA.x - lB.x) * lA.y;
	double C2 = -B1 * origin.x + A1 * origin.y;
	double det = A1 * A1 - -B1 * B1;

	if (det != 0)
	{
		point.x = (float)((A1*C1 - B1 * C2) / det);
		point.y = (float)((A1*C2 - -B1 * C1) / det);
	}
	else
	{
		point.x = origin.x;
		point.x = origin.y;
	}
	return (true);
}

void	CheckRayAABBResult(const Vec2& o, const Vec2 & dir, const Vec2& res, float t[2], Vec2 point[2], Vec2 normal[2], unsigned int side)
{
	static const Vec2	normals[4] = { Vec2(0, -1), Vec2(0, 1), Vec2(-1, 0), Vec2(1, 0) };

	float	tTmp = 0;

	//std::cout << "Check Ray AABB Result For Side (" << side << ")" << std::endl;

	if (dir.x > 0)
		tTmp = (res.x - o.x) / dir.x;
	else
		tTmp = (res.y - o.y) / dir.y;

	if (tTmp > 0)
	{
		if (tTmp < t[0])
		{
			t[0] = tTmp;
			point[0] = res;
			normal[0] = normals[side];
			// No Normal For The Moment
		}
		else if (tTmp < t[1])
		{
			t[1] = tTmp;
			point[1] = res;
			normal[1] = normals[side];
		}
	}
}

bool IntersectionComputer::RayAABB(const Vec2 & o, const Vec2 & dir, const AABB2 & aabb, float t[2], Vec2 point[2], Vec2 normal[2], const float & maxDist)
{
	//std::cout << "RAY AABB" << std::endl;
	Vec2	res;

	t[0] = FLT_MAX;
	t[1] = FLT_MAX;

	Vec2 oFar = (o + (dir * maxDist));

	if (SegmentSegment(o, oFar, aabb.min, Vec2(aabb.max.x, aabb.min.y), res)) // TOP
		CheckRayAABBResult(o, dir, res, t, point, normal, 0);
	if (SegmentSegment(o, oFar, Vec2(aabb.min.x, aabb.max.y), aabb.max, res)) // BOT
		CheckRayAABBResult(o, dir, res, t, point, normal, 1);
	if (SegmentSegment(o, oFar, aabb.min, Vec2(aabb.min.x, aabb.max.y), res)) // LEFT
		CheckRayAABBResult(o, dir, res, t, point, normal, 2);
	if (SegmentSegment(o, oFar, Vec2(aabb.max.x, aabb.min.y), aabb.max, res)) // RIGHT
		CheckRayAABBResult(o, dir, res, t, point, normal, 3);

	return (t[0] != FLT_MAX);
}

bool IntersectionComputer::RayInsideAABB(const Vec2 & o, const Vec2 & dir, const AABB2 & aabb, Vec2& res, const float& maxDist)
{
	static const Vec2	top(0, 1);
	static const Vec2	bot(0, -1);
	static const Vec2	left(1, 0);
	static const Vec2	right(-1, 0);

	Vec2 oFar = (o + (dir * maxDist));

	/*std::cout << "Angle Top and dir (" << top.Dot(dir) << ")" << std::endl;
	std::cout << "Angle Bot and dir (" << bot.Dot(dir) << ")" << std::endl;
	std::cout << "Angle Left and dir (" << left.Dot(dir) << ")" << std::endl;
	std::cout << "Angle Right and dir (" << right.Dot(dir) << ")" << std::endl;*/

	if (top.Dot(dir) < 0.0f) // TOP
	{
		if (SegmentSegment(o, oFar, aabb.min, Vec2(aabb.max.x, aabb.min.y), res))
			return (true);
	}
	else // BOT
	{
		if (SegmentSegment(o, oFar, Vec2(aabb.min.x, aabb.max.y), aabb.max, res))
			return (true);
	}
	if (left.Dot(dir) < 0.0f)
	{
		if (SegmentSegment(o, oFar, aabb.min, Vec2(aabb.min.x, aabb.max.y), res))
			return (true);
	}
	else
	{
		if (SegmentSegment(o, oFar, Vec2(aabb.max.x, aabb.min.y), aabb.max, res))
			return (true);
		
	}
	return (false);
}

float IntersectionComputer::Det(const float & a, const float & b, const float & c, const float & d)
{
	return ((a * d) - (b * c));
}

const Vec2 LineAABBNearestIntersectionComputer::_topDir(0, 1);
const Vec2 LineAABBNearestIntersectionComputer::_rightDir(1, 0);

void LineAABBNearestIntersectionComputer::Reset()
{
	intersectionDistSquared = FLT_MAX;
	intersectionCount = 0;
	intersectionSide = 0;
}

bool LineAABBNearestIntersectionComputer::Intersect(const AABB2& aabb, const Vec2 & lA, const Vec2 & lB)
{
	Reset();

	IntersectLeft(aabb, lA, lB);
	IntersectRight(aabb, lA, lB);
	IntersectTop(aabb, lA, lB);
	IntersectBot(aabb, lA, lB);

	return (intersectionCount != 0);
}


#include <iostream> // Tmp

bool LineAABBNearestIntersectionComputer::IntersectSideFromDir(const AABB2& aabb, const Vec2 & lA, const Vec2& lB, const Vec2 & dir)
{
	Reset();
	_angle = dir.Dot(_topDir);

	//PX2DebugDrawer::DrawAABB(aabb, Color(0, 150, 0));
	// //std::cout << "DIR: (" << dir.ToString() << ")" << std::endl;
	// //std::cout << "Angle: (" << _angle << ")" << std::endl;

	// //std::cout << "Coming From ";
	if (_angle > 0)
	{

		IntersectTop(aabb, lA, lB);
	}
	else if (_angle == 0)
	{
		IntersectTop(aabb, lA, lB);
		IntersectBot(aabb, lA, lB);
	}
	else
	{
		IntersectBot(aabb, lA, lB);
	}
	_angle = dir.Dot(_rightDir);

	if (_angle < 0)
	{
		IntersectRight(aabb, lA, lB);
	}
	else if (_angle == 0)
	{
		IntersectRight(aabb, lA, lB);
		IntersectLeft(aabb, lA, lB);
	}
	else
	{
		IntersectLeft(aabb, lA, lB);
	}

	//std::cout << std::endl;

	return (intersectionCount != 0);
}

bool LineAABBNearestIntersectionComputer::IntersectTop(const AABB2& aabb, const Vec2 & lA, const Vec2 & lB)
{
	//std::cout << " -Top ";
	_testPoints[0].x = aabb.min.x;
	_testPoints[0].y = aabb.min.y;
	_testPoints[1].x = aabb.max.x;
	_testPoints[1].y = aabb.min.y;
	return (Intersect(0, lA, lB));
}

bool LineAABBNearestIntersectionComputer::IntersectBot(const AABB2& aabb, const Vec2 & lA, const Vec2 & lB)
{
	//std::cout << " -Bot ";
	_testPoints[0].x = aabb.min.x;
	_testPoints[0].y = aabb.max.y;
	_testPoints[1].x = aabb.max.x;
	_testPoints[1].y = aabb.max.y;
	return (Intersect(1, lA, lB));
}

bool LineAABBNearestIntersectionComputer::IntersectLeft(const AABB2& aabb, const Vec2 & lA, const Vec2 & lB)
{
	//std::cout << " -Left ";
	_testPoints[0].x = aabb.min.x;
	_testPoints[0].y = aabb.min.y;
	_testPoints[1].x = aabb.min.x;
	_testPoints[1].y = aabb.max.y;
	return (Intersect(2, lA, lB));
}

bool LineAABBNearestIntersectionComputer::IntersectRight(const AABB2& aabb, const Vec2 & lA, const Vec2 & lB)
{
	//std::cout << " -Right ";
	_testPoints[0].x = aabb.max.x;
	_testPoints[0].y = aabb.min.y;
	_testPoints[1].x = aabb.max.x;
	_testPoints[1].y = aabb.max.y;
	return (Intersect(3, lA, lB));
}

bool LineAABBNearestIntersectionComputer::Intersect(const uint8_t& side, const Vec2& lA, const Vec2& lB)
{
	//std::cout << "Test Points A (" << _testPoints[0].ToString() << ") B " << _testPoints[1].ToString() << std::endl;
	//PX2DebugDrawer::DrawVec2Vec2(_testPoints[0], _testPoints[1], Color(25 * (side + 1), (side + 1) * (side + 1), (side << 2)));
	if (IntersectionComputer::SegmentSegment(_testPoints[0], _testPoints[1], lA, lB, _resPoint))
	{
		//std::cout << " (Intersect ";
		_lAToRes = (lA - _resPoint);
		intersectionCount += 1;
		_dist = VectorTransformer::LengthSquared(_lAToRes);
		if (_dist < intersectionDistSquared)
		{
			intersections[side] = _resPoint;
			intersectionDistSquared = _dist;
			intersectionSide = side;

			//std::cout << " " << _resPoint.ToString() << ")"; 

			return (true);
		}
	}
	return (false);
}
