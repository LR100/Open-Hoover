#pragma once

#include "Vector.h"

template <typename T>
class AABB2T
{
public:
	AABB2T() {}; // Vec Initialize by themself -> nothing to do
	AABB2T(const Vec2T<T>& _center, const Vec2T<T>& _halfSize);

	void		ComputeMinMax();

	/// Contains ///
	// Return (true) if the given Point is Completely inside this AABB2 	
	bool		ContainsPoint(const Vec2T<T>& point) const;
	// Return (true) if the given AABB2is Completely inside this AABB2 	
	bool		ContainsAABB(const AABB2T<T>& other) const;

	/// Intersect ///
	// Return (true) if the given AABB2 is intersecting this AABB2 	
	bool		IntersectAABB(const AABB2T<T>& other) const;
	// Return (true) if the given AABB2 is intersecting this AABB2 	
	bool		IntersectRay(const Vec2T<T> & p, const Vec2T<T> & d, T & tmin, Vec2T<T> & q) const;


	std::string	ToString() const;

	// Find The AABB2 Surrounding Both AABB
	AABB2T<T>			FindSurrounding(const AABB2T<T>& other) const;
	// Find The AABB2 Surrounding Both AABB
	static AABB2T<T>	FindSurrounding(const AABB2T<T>& a, const AABB2T<T>& other);
	// Find The AABB2 Surrounding Both Points
	static AABB2T<T>	FindSurrounding(const Vec2T<T>& a, const Vec2T<T>& other);

	Vec2T<T>	center;
	Vec2T<T>	halfSize;

	Vec2T<T>	min;
	Vec2T<T>	max;
};

// Float 
template class AABB2T<float>;
typedef AABB2T<float> AABB2;

// Int 
template class AABB2T<int>;
typedef AABB2T<int> AABB2i;

