#pragma once

#include "Vector.h"

#pragma pack(1)
template <typename T>
struct Mat22T
{
	Mat22T();

	virtual ~Mat22T() {};

	void		Zero();

	Vec2T<T>	cA, cB;
};
#pragma pack()

