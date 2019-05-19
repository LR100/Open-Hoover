#pragma once

#include <string>

#pragma pack(1)
template <typename T>
struct Vec2T
{
	Vec2T(T _x, T _y);
	Vec2T();

	virtual ~Vec2T() {};

	std::string	ToString() const;

	Vec2T<T>	operator-() const;
	Vec2T<T>&	operator-=(const Vec2T<T>& other);
	Vec2T<T>&	operator-=(const T& value);
	Vec2T<T>&	operator+=(const Vec2T<T>& other);
	Vec2T<T>&	operator+=(const T& value);

	Vec2T<T>&	operator*=(const T& value);
	Vec2T<T>&	operator/=(const T& value);

	// CONST Op
	bool		operator==(const Vec2T<T>& other) const;

	Vec2T<T>	operator-(const Vec2T& other) const;
	Vec2T<T>	operator+(const Vec2T<T>& other) const;

	Vec2T<T>	operator*(const T& value) const;
	Vec2T<T>	operator*(const Vec2T& other) const;

	Vec2T<T>	operator/(const T& value) const;

	T			Dot(const Vec2T<T>& other) const;
	T			Cross(const Vec2T<T>& other) const;
	void		Zero();


	T			x, y;
};
#pragma pack()

// Float 
template class Vec2T<float>;
typedef Vec2T<float> Vec2;
// Int
template class Vec2T<int>;
typedef Vec2T<int> Vec2i;


#pragma pack(1)
template <typename T>
struct Vec3T
{
	Vec3T(T _x, T _y, T _z);
	Vec3T();

	virtual ~Vec3T() {};

	std::string	ToString() const;

	Vec3T<T>	operator-() const;

	Vec3T<T>&	operator-=(const Vec3T<T>& other);
	Vec3T<T>&	operator+=(const Vec3T<T>& other);

	Vec3T<T>&	operator*=(const T& value);
	Vec3T<T>&	operator/=(const T& value);

	// CONST Op
	bool		operator==(const Vec3T<T>& other) const;

	Vec3T<T>	operator-(const Vec3T& other) const;
	Vec3T<T>	operator+(const Vec3T<T>& other) const;

	Vec3T<T>	operator*(const T& value) const;
	Vec3T<T>	operator/(const T& value) const;

	T			Dot(const Vec3T<T>& other) const;
	void		Zero();

	T	x, y, z;
};
#pragma pack()

// Float 
template class Vec3T<float>;
typedef Vec3T<float> Vec3;
// Int
template class Vec3T<int>;
typedef Vec3T<int> Vec3i;


#pragma pack(1)
template <typename T>
struct Vec4T
{
	Vec4T(T _x, T _y, T _z, T _i);
	Vec4T();

	virtual ~Vec4T() {};

	std::string	ToString() const;

	Vec4T<T>	operator-() const;

	Vec4T<T>&	operator-=(const Vec4T<T>& other);
	Vec4T<T>&	operator+=(const Vec4T<T>& other);

	Vec4T<T>&	operator*=(const T& value);
	Vec4T<T>&	operator/=(const T& value);

	// CONST Op
	bool		operator==(const Vec4T<T>& other) const;

	Vec4T<T>	operator-(const Vec4T& other) const;
	Vec4T<T>	operator+(const Vec4T<T>& other) const;

	Vec4T<T>	operator*(const T& value) const;
	Vec4T<T>	operator/(const T& value) const;

	T			Dot(const Vec4T<T>& other) const;

	T	x, y, z, i;
};
#pragma pack()

// Float 
template class Vec4T<float>;
typedef Vec4T<float> Vec4;
// Int
template class Vec4T<int>;
typedef Vec4T<int> Vec4i;

