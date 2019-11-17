#pragma once

#include "../Maths/Vector.h"

class Physic2DWorld;

class Physic2DShape
{
public:

	enum TYPE : uint8_t
	{
		CIRCLE = 0,
		BOX = 1,
		OBB = 2,
		POLYGON,
		NONE
	};

	Physic2DShape(Physic2DWorld* world, TYPE type) { _type = type; };
	virtual ~Physic2DShape() {};
	virtual void* GetShape() = 0;
	const TYPE& GetType() const { return (_type); };

private:

	TYPE	_type;
};


class Physic2DShapeCircle : public Physic2DShape
{
public:
	Physic2DShapeCircle(Physic2DWorld* world) : Physic2DShape(world, TYPE::CIRCLE) {};
	virtual ~Physic2DShapeCircle() {};

	virtual void			SetPositionRelativeToBody(const Vec2& position) = 0;
	virtual void			SetRadius(const float& radius) = 0;
	virtual const float&	GetRadius() const = 0;
	
private:

};

class Physic2DShapeBox : public Physic2DShape
{
public:
	Physic2DShapeBox(Physic2DWorld* world) : Physic2DShape(world, TYPE::BOX) {};
	virtual ~Physic2DShapeBox() {};

	virtual const float&	GetWidth() const = 0;
	virtual const float&	GetHeight() const = 0;
	virtual void			SetSize(const float& width, const float& height) = 0;
private:

};

class Physic2DShapePolygon : public Physic2DShape
{
public:
	Physic2DShapePolygon(Physic2DWorld* world) : Physic2DShape(world, TYPE::POLYGON) {};
	virtual ~Physic2DShapePolygon() {};
private:

};

