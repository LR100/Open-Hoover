#pragma once

#include "Physic2DShape.h"

///
// Box2D
//

#include "Box2D/Collision/Shapes/b2Shape.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"

class Physic2DShapeCircleB2D : public Physic2DShapeCircle
{
public:
	Physic2DShapeCircleB2D(Physic2DWorld* world) : Physic2DShapeCircle(world)
	{

	};
	virtual ~Physic2DShapeCircleB2D() {};

	virtual void			SetPositionRelativeToBody(const Vec2& position);
	virtual void			SetRadius(const float& radius);
	virtual const float&	GetRadius() const;
	virtual void*			GetShape() override;




private:
	b2CircleShape	_circleShape;
};

class Physic2DShapeBoxB2D : public Physic2DShapeBox
{
public:
	Physic2DShapeBoxB2D(Physic2DWorld* world) : Physic2DShapeBox(world)
	{
		_width = 0;
		_height = 0;
	};
	virtual ~Physic2DShapeBoxB2D() {};

	virtual const float&	GetWidth() const override;
	virtual const float&	GetHeight() const override;
	virtual void			SetSize(const float& width, const float& height) override;
	virtual void*			GetShape() override;
private:
	b2PolygonShape	_polygonShape;
	float			_width;
	float			_height;
};

class Physic2DShapePolygonB2D : public Physic2DShapePolygon
{
public:
	Physic2DShapePolygonB2D(Physic2DWorld* world) : Physic2DShapePolygon(world)
	{

	};
	virtual ~Physic2DShapePolygonB2D() {};

	virtual void* GetShape() override;
private:
	b2PolygonShape	_polygonShape;
};