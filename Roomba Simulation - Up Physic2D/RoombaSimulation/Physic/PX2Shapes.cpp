#include "PX2Shapes.h"

#include <algorithm>
#include "../Maths/VectorTransformer.h"
#include "../Maths/ValueTools.h"

#include "PX2CollisionSolver.h"

#define COEF_HIGH 2.0f

const float & PX2Shape::GetRadius() const
{
	return (_radius);
}

const float & PX2Shape::GetHighVelocityValue()
{
	return (_highVelocityValue);
}


PX2ShapeCircle::PX2ShapeCircle(float radius)
{
	_type = PX2Shape::TYPE::CIRCLE;
	_aabb.halfSize.x = radius;
	_aabb.halfSize.y = radius;
	_radius = radius;
	_highVelocityValue = _radius * COEF_HIGH;
}

PX2ShapeCircle::~PX2ShapeCircle()
{

}

bool PX2ShapeCircle::Intersect(const PX2Shape * shapeB, const Vec2& posA, const Vec2& posB, PX2Manifold& manifold) const
{
	if (shapeB->GetType() == PX2Shape::TYPE::CIRCLE)
	{
		const PX2ShapeCircle*	other = static_cast<const PX2ShapeCircle*>(shapeB);
		return (IntersectWithCircle(other, posA, posB, manifold));
	}
	else if (shapeB->GetType() == PX2Shape::TYPE::BOX)
	{
		const PX2ShapeBox*		other = static_cast<const PX2ShapeBox*>(shapeB);
		return (IntersectWithBox(other, posA, posB, manifold));
	}
	return (false);
}

bool PX2ShapeCircle::IntersectWithCircle(const PX2ShapeCircle * shape, const Vec2 & posA, const Vec2 & posB, PX2Manifold & manifold) const
{
	Vec2					n = (posB - posA);
	float					r = (_radius + shape->GetRadius());
	float					rr = r * r;

	if (VectorTransformer::LengthSquared(n) > rr)
		return (false);

	float d = VectorTransformer::Length(n);

	if (d != 0)
	{
		// Distance is difference between radius and distance
		manifold.penetration = r - d;

		// Utilize our d since we performed sqrt on it already within Length( )
		// Points from A to B, and is a unit vector
		VectorTransformer::Normalize(n, manifold.normal);
		return (true);
	}
	// Circles are on same position
	else
	{
		// Choose random (but consistent) values
		manifold.penetration = r;


		VectorTransformer::Normalize(-n, manifold.normal);
		return (true);
	}
}

bool PX2ShapeCircle::IntersectWithBox(const PX2ShapeBox * shapeB, const Vec2 & posA, const Vec2 & posB, PX2Manifold & manifold) const
{
	// This Become B -> Switch Positions
	return (shapeB->IntersectWithCircle(this, posB, posA, manifold));
}

PX2ShapeBox::PX2ShapeBox(float width, float height)
{
	_type = PX2Shape::TYPE::BOX;
	_width = width;
	_height = height;
	_aabb.halfSize.x = (_width * 0.5f);
	_aabb.halfSize.y = (_height * 0.5f);
	_radius = std::max(_aabb.halfSize.x, _aabb.halfSize.y);
	_highVelocityValue = _radius * COEF_HIGH;
}

PX2ShapeBox::~PX2ShapeBox()
{
}

#include <iostream> // TMP

bool PX2ShapeBox::Intersect(const PX2Shape * shape, const Vec2& posA, const Vec2& posB, PX2Manifold& manifold) const
{
	if (shape->GetType() == PX2Shape::TYPE::BOX)
	{
		const PX2ShapeBox*		other = static_cast<const PX2ShapeBox*>(shape);
		return (IntersectWithBox(other, posA, posB, manifold));
	}
	else if (shape->GetType() == PX2Shape::TYPE::CIRCLE)
	{
		const PX2ShapeCircle*	other = static_cast<const PX2ShapeCircle*>(shape);
		return (IntersectWithCircle(other, posA, posB, manifold));
	}
	return (false);
}

bool PX2ShapeBox::IntersectWithBox(const PX2ShapeBox * shape, const Vec2 & posA, const Vec2 & posB, PX2Manifold & manifold) const
{
	// Vector from A to B
	Vec2		n = (posA - posB);

	AABB2 abox = _aabb;
	abox.center = posA;
	abox.ComputeMinMax();

	AABB2 bbox = shape->GetAABB();
	bbox.center = posB;
	bbox.ComputeMinMax();

	// Calculate half extents along x axis for each object
	float aex = (abox.max.x - abox.min.x) * 0.5f;
	float bex = (bbox.max.x - bbox.min.x) * 0.5f;

	// Calculate overlap on x axis
	float x_overlap = aex + bex - abs(n.x);

	// SAT test on x axis
	if (x_overlap > 0)
	{
		// Calculate half extents along x axis for each object
		float aey = (abox.max.y - abox.min.y) * 0.5f;
		float bey = (bbox.max.y - bbox.min.y) * 0.5f;

		// Calculate overlap on y axis
		float y_overlap = aey + bey - abs(n.y);

		// SAT test on y axis
		if (y_overlap > 0)
		{
			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap)
			{
				// Point towards B knowing that n points from A to B
				if (n.x < 0)
				{
					manifold.normal = Vec2(1, 0);
				}
				else
				{
					manifold.normal = Vec2(-1, 0);
				}
				manifold.penetration = x_overlap;
				return (true);
			}
			else
			{
				// Point toward B knowing that n points from A to B
				if (n.y < 0)
				{
					manifold.normal = Vec2(0, 1);
				}
				else
				{
					manifold.normal = Vec2(0, -1);
				}
				manifold.penetration = y_overlap;
				return (true);
			}
		}
	}
	return (false);
}

float clamp(float n, float lower, float upper)
{
	return std::max(lower, std::min(n, upper));
}

bool PX2ShapeBox::IntersectWithCircle(const PX2ShapeCircle * shape, const Vec2 & posA, const Vec2 & posB, PX2Manifold & manifold) const
{
	// Setup a couple pointers to each object

	// Vector from A to B
	

	// Closest point on A to center of B
	Vec2 nearest = posB;

	// Calculate half extents along each axis
	AABB2 abox = _aabb;
	abox.center = posA;
	abox.ComputeMinMax();

	// Clamp point to edges of the AABB2 
	
	nearest.x = clamp(nearest.x, abox.min.x, abox.max.x);
	nearest.y = clamp(nearest.y, abox.min.y, abox.max.y);

	bool inside = false;

	// Circle is inside the AABB2 so we need to clamp the circle's center
	// to the closest edge

	Vec2 normal = (posB - nearest);

	float d = VectorTransformer::LengthSquared(normal);

	float r = shape->GetRadius();

	// Early out of the radius is shorter than distance to closest point and
	// Circle not inside the AABB2 
	

	// Avoided sqrt until we needed
	d = ValueTools::FastSqrt(d);
	VectorTransformer::Normalize(normal);

	// Collision normal needs to be flipped to point outside if circle was
	// inside the AABB2 

	manifold.normal = -normal;
	manifold.penetration = (r - d);


	

	const float DIFF_EQUAL = 0.5f;

	if (d == 0)
	{
   		if (ValueTools::EqualEpsilon(nearest.y, abox.min.y, DIFF_EQUAL))
		{
			//std::cout << "Hit Top" << std::endl;
			manifold.penetration += fabs(nearest.y - abox.min.y);

 			manifold.normal = Vec2(0, 1);
		}
		else if (ValueTools::EqualEpsilon(nearest.y, abox.max.y, DIFF_EQUAL))
		{
			//std::cout << "Hit Bot" << std::endl;
			manifold.penetration += ((abox.max.y - nearest.y) + r);
			manifold.normal = Vec2(0, -1);
		}
		else if (ValueTools::EqualEpsilon(nearest.x, abox.min.x, DIFF_EQUAL))
		{
			//std::cout << "Hit Left" << std::endl;
			manifold.penetration += ((nearest.x - abox.min.x) + r);
			manifold.normal = Vec2(1, 0);
		}
		else if (ValueTools::EqualEpsilon(nearest.x, abox.max.x, DIFF_EQUAL))
		{
			//std::cout << "Hit Right" << std::endl;
			manifold.penetration += ((abox.max.x - nearest.x) + r);
			manifold.normal = Vec2(-1, 0);
		}
	}
	

	//std::cout << "NORMAL X(" << manifold.normal.x << ") Y(" << manifold.normal.y << ")" << std::endl;
	//std::cout << "Penetration (" << manifold.penetration << ") R(" << r << ") D(" << d << ")\n";


	return (true);
}

//// Vector from A to B
//Vec2 n = posB - posA;
//
//// Closest point on A to center of B
//Vec2 closest = n;
//
//// Calculate half extents along each axis
//float x_extent = (_aabb.max.x - _aabb.min.x) * .5f;
//float y_extent = (_aabb.max.y - _aabb.min.y) * .5f;
//
//// Clamp point to edges of the AABB2 
//closest.x = clamp(closest.x, -x_extent, x_extent);
//closest.y = clamp(closest.y, -y_extent, y_extent);
//
//bool inside = false;
//
//// Circle is inside the AABB2 so we need to clamp the circle's center
//// to the closest edge
//if (VectorTransformer::AreEqual(n, closest, 1.0f))
//{
//	inside = true;
//
//	// Find closest axis
//	if (abs(n.x) > abs(n.y))
//	{
//		// Clamp to closest extent
//		if (closest.x > 0)
//			closest.x = x_extent;
//		else
//			closest.x = -x_extent;
//	}
//
//	// y axis is shorter
//	else
//	{
//		// Clamp to closest extent
//		if (closest.y > 0)
//			closest.y = y_extent;
//		else
//			closest.y = -y_extent;
//	}
//}
//
//Vec2 normal = n - closest;
//
//float d = VectorTransformer::LengthSquared(normal);
//float r = shape->GetRadius();
//
//// Early out of the radius is shorter than distance to closest point and
//// Circle not inside the AABB2 
//if (d > r * r && !inside)
//return false;
//
//// Avoided sqrt until we needed
//d = ValueTools::FastSqrt(d);
//
//
//// Collision normal needs to be flipped to point outside if circle was
//// inside the AABB2 
//if (inside)
//{
//	manifold.normal = -n;
//	manifold.penetration = r - d;
//}
//else
//{
//	manifold.normal = n;
//	manifold.penetration = r - d;
//}
//
//return true;



const float & PX2ShapeBox::GetWidth() const
{
	return (_width);
}

const float & PX2ShapeBox::GetHeight() const
{
	return (_height);
}
