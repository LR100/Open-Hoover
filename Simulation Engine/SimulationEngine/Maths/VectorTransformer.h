#pragma once

#include "ValueTable.h"
#include "ValueTools.h"
#include "Vector.h"

// ALL Angles Are in DEGREE
class VectorTransformer
{
public:
	// Rotation
	virtual ~VectorTransformer() {};

	static void Rotate(Vec2& vec, const float& angle);
	static void RotateInv(Vec2& vec, const float& angle);

	static void RotateXYZ(Vec3& vec, const float& ax, const float& ay, const float& az);
	static void RotateX(Vec3& vec, const float& angle);
	static void RotateY(Vec3& vec, const float& angle);
	static void RotateZ(Vec3& vec, const float& angle);

	static void RotateInvXYZ(Vec3& vec, const float& ax, const float& ay, const float& az);
	static void RotateInvX(Vec3& vec, const float& angle);
	static void RotateInvY(Vec3& vec, const float& angle);
	static void RotateInvZ(Vec3& vec, const float& angle);


	static float	Distance(const Vec2& vecA, const Vec2& vecB);
	static float	Length(const Vec2& vec);
	static float	LengthSquared(const Vec2& vec);


	static void Normalize(Vec2& vec);
	static void Normalize(Vec3& vec);

	static void Normalize(const Vec2& vec, Vec2& res);
	static void Normalize(const Vec3& vec, Vec3& res);

	static bool AreEqual(const Vec2& a, const Vec2& b, const float& maxEpsilon);

	static float GetCos(const float& angle);
	static float GetSin(const float& angle);

protected:

	static CosTable		_cosTable;
	static SinTable		_sinTable;

	static unsigned int		GetScaledAngle(const float& angle);
};