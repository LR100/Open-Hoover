#include "AABB.h"

#include <sstream>
#include <algorithm>

template <typename T>
AABB2T<T>::AABB2T(const Vec2T<T>& _center, const Vec2T<T>& _halfSize)
{
	center = _center;
	halfSize = _halfSize;
	ComputeMinMax();
}

template<typename T>
void AABB2T<T>::ComputeMinMax()
{
	min = center - halfSize;
	max = center + halfSize;
}

template<typename T>
bool AABB2T<T>::ContainsPoint(const Vec2T<T> & point) const
{
	if (point.x < min.x || point.x > max.x || point.y < min.y || point.y > max.y)
		return (false);
	return (true);
}

template<typename T>
bool AABB2T<T>::ContainsAABB(const AABB2T<T>& other) const
{
	if (other.min.x <= min.x || other.max.x >= max.x || other.min.y <= min.y || other.max.y >= max.y)
		return (false);
	return (true);
}

template<typename T>
bool AABB2T<T>::IntersectAABB(const AABB2T<T>& other) const
{
	if (max.x < other.min.x || min.x > other.max.x)
		return (false);
	if (max.y < other.min.y || min.y > other.max.y)
		return (false);
	return (true);
}

template<typename T>
bool AABB2T<T>::IntersectRay(const Vec2T<T>& p, const Vec2T<T>& d, T & tmin, Vec2T<T>& q) const
{
	tmin = 0.0f;
	// set to -FLT_MAX to get first hit online
	float tmax = FLT_MAX;
	// For all three slabs

	if (std::abs(d.x) < FLT_EPSILON)
	{
		// Ray is parallel to slab. No hit if origin not within slab
		if (p.x < min.x || p.x > max.x)
			return (false);
	}
	else
	{
		// Compute intersection t value of ray with near and far plane of slab
		float ood = 1.0f / d.x;
		float t1 = (min.x - p.x) * ood;
		float t2 = (max.x - p.x) * ood;
		// Make t1 be intersection with near plane, t2 with far plane
		if (t1 > t2) std::swap(t1, t2);
		// Compute the intersection of slab intersection intervals
		if (t1 > tmin) tmin = t1;
		if (t2 > tmax) tmax = t2;
		// Exit with no collision as soon as slab intersection becomes empty
		if (tmin > tmax)
			return (false);
	}

	if (std::abs(d.y) < FLT_EPSILON)
	{
		// Ray is parallel to slab. No hit if origin not within slab
		if (p.y < min.y || p.y > max.y)
			return (false);
	}
	else
	{
		// Compute intersection t value of ray with near and far plane of slab
		float ood = (1.0f / d.y);
		float t1 = (min.y - p.y) * ood;
		float t2 = (max.y - p.y) * ood;
		// Make t1 be intersection with near plane, t2 with far plane
		if (t1 > t2) std::swap(t1, t2);
		// Compute the intersection of slab intersection intervals
		if (t1 > tmin) tmin = t1;
		if (t2 > tmax) tmax = t2;
		// Exit with no collision as soon as slab intersection becomes empty
		if (tmin > tmax)
			return (false);
	}

	// Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
	q = p + d * tmin;
	return (true);
}

template<typename T>
std::string AABB2T<T>::ToString() const
{
	std::stringstream ss;
	ss << "C( " << center.ToString() << ") ";
	ss << "S( " << halfSize.ToString() << ") ";
	return (ss.str());
}

template<typename T>
AABB2T<T> AABB2T<T>::FindSurrounding(const AABB2T<T>& other) const
{
	AABB2T<T>	surrounding;

	surrounding.min.x = std::min(min.x, other.min.x);
	surrounding.max.x = std::max(max.x, other.max.x);

	surrounding.min.y = std::min(min.y, other.min.y);
	surrounding.max.y = std::max(max.y, other.max.y);

	surrounding.halfSize.x = (surrounding.max.x - surrounding.min.x) * 0.5f;
	surrounding.halfSize.y = (surrounding.max.y - surrounding.min.y) * 0.5f;

	surrounding.center.x = (surrounding.min.x + surrounding.halfSize.x);
	surrounding.center.y = (surrounding.min.y + surrounding.halfSize.y);

	return (surrounding);
}

template<typename T>
AABB2T<T> AABB2T<T>::FindSurrounding(const AABB2T<T>& a, const AABB2T<T>& other)
{
	AABB2T<T>	surrounding;

	surrounding.min.x = std::min(a.min.x, other.min.x);
	surrounding.max.x = std::max(a.max.x, other.max.x);

	surrounding.min.y = std::min(a.min.y, other.min.y);
	surrounding.max.y = std::max(a.max.y, other.max.y);

	surrounding.halfSize.x = (surrounding.max.x - surrounding.min.x) * 0.5f;
	surrounding.halfSize.y = (surrounding.max.y - surrounding.min.y) * 0.5f;

	surrounding.center.x = (surrounding.min.x + surrounding.halfSize.x);
	surrounding.center.y = (surrounding.min.y + surrounding.halfSize.y);

	return (surrounding);
}

template<typename T>
AABB2T<T> AABB2T<T>::FindSurrounding(const Vec2T<T>& a, const Vec2T<T>& other)
{
	AABB2T<T>	surrounding;

	if (a.x > other.x)
	{
		surrounding.min.x = other.x;
		surrounding.max.x = a.x;
	}
	else
	{
		surrounding.min.x = a.x;
		surrounding.max.x = other.x;
	}

	if (a.y > other.y)
	{
		surrounding.min.y = other.y;
		surrounding.max.y = a.y;
	}
	else
	{
		surrounding.min.y = a.y;
		surrounding.max.y = other.y;
	}


	surrounding.halfSize.x = (surrounding.max.x - surrounding.min.x) * 0.5f;
	surrounding.halfSize.y = (surrounding.max.y - surrounding.min.y) * 0.5f;

	surrounding.center.x = (surrounding.min.x + surrounding.halfSize.x);
	surrounding.center.y = (surrounding.min.y + surrounding.halfSize.y);

	return (surrounding);
}
