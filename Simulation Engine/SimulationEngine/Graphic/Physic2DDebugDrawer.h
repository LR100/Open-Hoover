#pragma once

#include "IDrawer2D.h"
#include "Sprite.h"
#include "../Physic/Physic2DWorld.h"
#include <unordered_map>

class Physic2DDebugDrawer : public Physic2DQueryBodyCb
{
public:
	Physic2DDebugDrawer(IDrawer2D* drawer);
	~Physic2DDebugDrawer();

	void			RegisterBody(Physic2DBody* body);
	void			UnregisterBody(const size_t& bodyID);
	void			ResetObjectsCount();
	const size_t&   GetObjectsCount() const;

private:
	
	void	UnregisterBodyNoCheck(const size_t& bodyID);

	// Id Body - Sprites of body from Fixtures
	
	IDrawer2D*											_drawer;

	struct SpriteCentered
	{
		Sprite*		sprite;
		Vec2		offset;
	};

	std::unordered_map<size_t, std::vector<SpriteCentered>>	_bodiesSprites;
	// Inherited via Physic2DQueryBodyCb
	virtual bool ReportBody(Physic2DBody* body) override;

	Vec2		_posDraw;
	size_t		_objectsCount;
	Color		_colorCenter;
};





