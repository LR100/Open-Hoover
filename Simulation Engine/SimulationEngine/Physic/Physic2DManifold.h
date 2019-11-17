#pragma once

#include "PX2Body.h"
#include "../Tools/DObject.h"

#define PX2_UPDATE_PER_SECOND 50.0f // 50 By Default
#define PX2_DT_STEP (1.0f / PX2_UPDATE_PER_SECOND)

struct PX2Manifold : public DObject<PX2Manifold>
{
	PX2Manifold(PX2Body* a, PX2Body* b);
	PX2Manifold();
	virtual ~PX2Manifold();

	float	dist;
	float	penetration;

	float	toi;

	Vec2	normal;

	Vec2	positionCorrectionA;
	Vec2	positionCorrectionB;

	bool	resolved;

	void	Set(PX2Body* a, PX2Body* b);
	void	SwapAB(PX2Body* buffer);

	inline const uint8_t&	Type() const { return (_type); };
	inline PX2Body*			A() const { return (_a); };
	inline PX2Body*			B() const { return (_b); };


private:

	
	void		SetType();

	uint8_t					_type;
	PX2Body*				_a;
	PX2Body*				_b;
};
