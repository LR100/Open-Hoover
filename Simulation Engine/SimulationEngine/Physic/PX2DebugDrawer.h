#pragma once

#include "../Graphic/IDrawer2D.h" // TMP PURE DEBUG GRAPHIC
#include "../Graphic/IWindow.h" // TMP PURE DEBUG GRAPHIC
extern IDrawer2D*	PX2Drawer;
extern IWindow*		PX2Window;

#include "../Maths/AABB.h"



class PX2DebugDrawer
{
public:
	PX2DebugDrawer() {};
	virtual ~PX2DebugDrawer() {};

	static void DrawAABB(const AABB2&aabb, const Color& color);
	static void DrawVec2Vec2(const Vec2& a, const Vec2& b, const Color& color);

private:
};
