#include "ValueTools.h"

bool ValueTools::EqualEpsilon(const float & a, const float & b, const float & epsilon)
{
	return (fabs(b - a) < epsilon);
}

float ValueTools::PRound(float v, int precision)
{
	float tmp = std::pow(10, precision);
	float t = std::roundf(v * tmp) / tmp;
	if (t == 0 && std::signbit(t))
		t = 0.0f;
	return (t);
}

float ValueTools::FloatingPart(const float & f)
{
	return ((f - (long)f));
}

float ValueTools::RFloatingPart(const float & f)
{
	return (1.0f - FloatingPart(f));
}

int ValueTools::IntegerPart(const float & f)
{
	return ((f - FloatingPart(f)));
}


