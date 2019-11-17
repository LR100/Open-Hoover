#include "PX2Manifold.h"

PX2Manifold::PX2Manifold(PX2Body * a, PX2Body * b)
{
	_a = a;
	_b = b;
	dist = FLT_MAX;
	penetration = 1.0f;
	resolved = false;
	SetType();
}

PX2Manifold::PX2Manifold()
{
	_a = NULL;
	_b = NULL;
	penetration = 1.0f;
	resolved = false;
	// Normal is automaticaly initialized to 0,0
}

PX2Manifold::~PX2Manifold()
{
}

void PX2Manifold::Set(PX2Body * a, PX2Body* b)
{
	_a = a;
	_b = b;
	dist = FLT_MAX;
	resolved = false;
	SetType();
}

void PX2Manifold::SwapAB(PX2Body * buffer)
{
	buffer = _a;
	_a = _b;
	_b = buffer;
}

void PX2Manifold::SetType()
{
	_type = _a->GetType() + _b->GetType();
}
