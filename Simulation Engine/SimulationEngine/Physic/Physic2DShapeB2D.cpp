#include "Physic2DShapeB2D.h"

void Physic2DShapeCircleB2D::SetPositionRelativeToBody(const Vec2& position)
{
	_circleShape.m_p.Set(position.x, position.y);
}

void Physic2DShapeCircleB2D::SetRadius(const float& radius)
{
	_circleShape.m_radius = radius;
}

const float& Physic2DShapeCircleB2D::GetRadius() const
{
	return (_circleShape.m_radius);
}

void* Physic2DShapeCircleB2D::GetShape()
{
	return (&_circleShape);
}

void* Physic2DShapePolygonB2D::GetShape()
{
	return (&_polygonShape);
}

const float& Physic2DShapeBoxB2D::GetWidth() const
{
	return (_width);
}

const float& Physic2DShapeBoxB2D::GetHeight() const
{
	return (_height);
}

void Physic2DShapeBoxB2D::SetSize(const float& width, const float& height)
{
	_width = width;
	_height = height;
	_polygonShape.SetAsBox(width * 0.5f, height * 0.5f);
}

void* Physic2DShapeBoxB2D::GetShape()
{
	return (&_polygonShape);
}
