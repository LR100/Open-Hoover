#ifndef _VALUETOOLS_H_
#define _VALUETOOLS_H_

#include <cmath>
#include <algorithm>

#define PI (atanf(1.0f) * 4.0f)
#define RADIANS(deg)  ((deg) * PI / 180.0f)
#define DEGREES(rad)  ((rad) * 180.0f / PI)

#define MMC_FINF 3402823000000000000000.0f
#define MMC_PI 3.14159265359
#define MMC_D_TO_R(n) (n * (MMC_PI / 180.0f))

#define MMC_LOW_VALUE 0.0000000001f
#define MMC_SQRT_MAGIC_F 0x5f3759df 


class ValueTools
{
public:

	virtual ~ValueTools() {};

	template <typename T>
	static void Swap(T& a, T& b);

	template <typename T>
	static T Min(const T& a, const T& b);

	template <typename T>
	static T Max(const T& a, const T& b);

	template <typename T>
	static T Abs(T v);

	template <typename T>
	static T Floor(T v);

	template <typename T>
	static T Round(T v);


	static inline float ValueTools::Clamp(float n, float lower, float upper);
	
	
	static bool		EqualEpsilon(const float& a, const float& b, const float& epsilon);

	static float	PRound(float v, int precision);

	static float	FloatingPart(const float& f);
	static float	RFloatingPart(const float& f);
	static int		IntegerPart(const float& f);

	static float	FastSqrt(const float& value);
	static float	FastInvSqrt(const float& value);

private:

};


template<typename T>
inline void ValueTools::Swap(T & a, T & b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

template<typename T>
inline T ValueTools::Min(const T & a, const T & b)
{
	if (a < b)
		return (a);
	return (b);
}

template<typename T>
inline T ValueTools::Max(const T & a, const T & b)
{
	if (a > b)
		return (a);
	return (b);
}

template<typename T>
inline T ValueTools::Abs(T  v)
{
	if (v < 0)
		return (-v);
	return (v);
}

template<typename T>
inline T ValueTools::Floor(T v)
{
	return T(std::floor(v));
}

template<typename T>
inline T ValueTools::Round(T v)
{
	return T(std::round(v));
}

inline float ValueTools::Clamp(float n, float lower, float upper)
{
	return std::max(lower, std::min(n, upper));
}

inline float ValueTools::FastSqrt(const float & value)
{
	const float xhalf = 0.5f * value;
	union // get bits for floating value
	{
		float x;
		int i;
	} u;
	u.x = value;
	u.i = MMC_SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0
	return (value * u.x * (1.5f - xhalf * u.x * u.x));// Newton step, repeating increases accuracy 
}

inline float ValueTools::FastInvSqrt(const float & value)
{
	long	i;
	float	x = (value * 0.5F);
	float	y = value;
	const	float f = 1.5F;

	i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (f - (x * y * y));
	return (y);
}

#endif /* !_VALUETOOL_H_ */
