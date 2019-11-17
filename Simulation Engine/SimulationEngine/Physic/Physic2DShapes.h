#pragma once

#include "../Maths/AABB.h"

struct PX2Manifold;


#define PX2SHAPE_TYPE_COUNT 2 // OBB Not yet implemented

class PX2Shape
{
public:

	enum TYPE : uint8_t
	{
		CIRCLE = 0,
		BOX = 1,
		OBB = 2,
		NONE
	};

	virtual ~PX2Shape() {};
	const AABB2& GetAABB() const { return (_aabb); };
	const TYPE&	 GetType() const { return (_type); };

	virtual bool Intersect(const PX2Shape* shape, const Vec2& posA, const Vec2& posB, PX2Manifold& manifold) const = 0;

	const float& GetRadius() const; // Radius Of Bounding Circle
	const float& GetHighVelocityValue(); // Proportional to Radius

protected:

	AABB2	_aabb;
	TYPE	_type = NONE;
	float	_radius;
	float	_highVelocityValue;

private:

};

class PX2ShapeCircle;

class PX2ShapeBox : public PX2Shape
{
public:
	PX2ShapeBox(float width, float height);
	~PX2ShapeBox();

	virtual bool Intersect(const PX2Shape* shape, const Vec2& posA, const Vec2& posB, PX2Manifold& manifold) const override;

	bool IntersectWithBox(const PX2ShapeBox* shape, const Vec2& posA, const Vec2& posB, PX2Manifold& manifold) const;
	bool IntersectWithCircle(const PX2ShapeCircle* shape, const Vec2& posA, const Vec2& posB, PX2Manifold& manifold) const;


	const float& GetWidth() const;
	const float& GetHeight() const;

private:

	float	_width;
	float	_height;
};

class PX2ShapeCircle : public PX2Shape
{
public:
	PX2ShapeCircle(float radius);
	~PX2ShapeCircle();

	virtual bool Intersect(const PX2Shape* shape, const Vec2& posA, const Vec2& posB, PX2Manifold& manifold) const override;
	bool IntersectWithCircle(const PX2ShapeCircle* shape, const Vec2& posA, const Vec2& posB, PX2Manifold& manifold) const;
	bool IntersectWithBox(const PX2ShapeBox* shape, const Vec2& posA, const Vec2& posB, PX2Manifold& manifold) const;

private:

};

