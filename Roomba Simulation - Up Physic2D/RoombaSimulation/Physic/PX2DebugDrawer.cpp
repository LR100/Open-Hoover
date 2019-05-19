#include "PX2DebugDrawer.h"

void PX2DebugDrawer::DrawAABB(const AABB2& aabb, const Color & color)
{
	PX2Drawer->DrawBorder((int)aabb.min.x, (int)aabb.min.y, (int)(aabb.halfSize.x * 2.0f), (int)(aabb.halfSize.y * 2.0f), color, color);
}

void PX2DebugDrawer::DrawVec2Vec2(const Vec2 & a, const Vec2 & b, const Color & color)
{
	PX2Drawer->DrawLine((int)a.x, (int)a.y, (int)b.x, (int)b.y, color);
}

