#include "Vector.h"
#include <sstream>


///////////
// VEC 2 //
///////////

template <typename T>
Vec2T<T>::Vec2T(T _x, T _y)
{
	x = _x;
	y = _y;
}

template <typename T>
Vec2T<T>::Vec2T()
{
	x = 0;
	y = 0;
}

template <typename T>
std::string Vec2T<T>::ToString() const
{
	std::stringstream ss;
	ss << "X (" << x << ") Y (" << y << ")";
	return (ss.str());
}

template <typename T>
Vec2T<T> Vec2T<T>::operator-() const
{
	return Vec2T<T>(-x, -y);
}

template <typename T>
Vec2T<T> & Vec2T<T>::operator-=(const Vec2T<T> & other)
{
	x -= other.x;
	y -= other.y;
	return (*this);
}

template<typename T>
Vec2T<T>& Vec2T<T>::operator-=(const T & value)
{
	x -= value;
	y -= value;
	return (*this);
}

template <typename T>
Vec2T<T> & Vec2T<T>::operator+=(const Vec2T<T> & other)
{
	x += other.x;
	y += other.y;
	return (*this);
}

template<typename T>
Vec2T<T>& Vec2T<T>::operator+=(const T & value)
{
	x += value;
	y += value;
	return (*this);
}

template <typename T>
Vec2T<T> & Vec2T<T>::operator*=(const T & value)
{
	x *= value;
	y *= value;
	return (*this);
}

template<typename T>
Vec2T<T>& Vec2T<T>::operator/=(const T & value)
{
	x /= value;
	y /= value;
	return (*this);
}

template <typename T>
bool Vec2T<T>::operator==(const Vec2T<T> & other) const
{
	return (x == other.x && y == other.y);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator-(const Vec2T & other) const
{
	return Vec2T<T>(x - other.x, y - other.y);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator+(const Vec2T<T>& other) const
{
	return Vec2T<T>(x + other.x, y + other.y);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator*(const T& value) const
{
	return Vec2T<T>(x * value, y * value);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator*(const Vec2T & other) const
{
	return Vec2T<T>(x * other.x, y * other.y);
}

template<typename T>
Vec2T<T> Vec2T<T>::operator/(const T& value) const
{
	return Vec2T<T>(x / value, y / value);
}

template<typename T>
void Vec2T<T>::Zero()
{
	x = 0;
	y = 0;
}

template<typename T>
T Vec2T<T>::Dot(const Vec2T<T>& other) const
{
	return T((x * other.x) + (y * other.y));
}

template<typename T>
T Vec2T<T>::Cross(const Vec2T<T>& other) const
{
	return T((x * other.y) - (y * other.x));
}
///////////////////////////////////////////////////////////////////////////////////////

///////////
// VEC 3 //
///////////

template <typename T>
Vec3T<T>::Vec3T(T _x, T _y, T _z)
{
	x = _x;
	y = _y;
	z = _z;
}

template <typename T>
Vec3T<T>::Vec3T()
{
	x = 0;
	y = 0;
	z = 0;
}

template <typename T>
std::string Vec3T<T>::ToString() const
{
	std::stringstream ss;
	ss << "X (" << x << ") Y (" << y << ") Z (" << z << ")";
	return (ss.str());
}

template <typename T>
Vec3T<T> Vec3T<T>::operator-() const
{
	return Vec3T<T>(-x, -y, -z);
}

template <typename T>
Vec3T<T> & Vec3T<T>::operator-=(const Vec3T<T> & other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return (*this);
}

template <typename T>
Vec3T<T> & Vec3T<T>::operator+=(const Vec3T<T> & other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return (*this);
}

template <typename T>
Vec3T<T> & Vec3T<T>::operator*=(const T & value)
{
	x *= value;
	y *= value;
	z *= value;
	return (*this);
}

template<typename T>
Vec3T<T>& Vec3T<T>::operator/=(const T & value)
{
	x /= value;
	y /= value;
	z /= value;
	return (*this);
}

template <typename T>
bool Vec3T<T>::operator==(const Vec3T<T> & other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

template<typename T>
Vec3T<T> Vec3T<T>::operator-(const Vec3T & other) const
{
	return Vec3T<T>(x - other.x, y - other.y, z - other.z);
}

template<typename T>
Vec3T<T> Vec3T<T>::operator+(const Vec3T<T>& other) const
{
	return Vec3T<T>(x + other.x, y + other.y, z + other.z);
}

template<typename T>
Vec3T<T> Vec3T<T>::operator*(const T& value) const
{
	return Vec3T<T>(x * value, y * value, z * value);
}

template<typename T>
Vec3T<T> Vec3T<T>::operator/(const T& value) const
{
	return Vec3T<T>(x / value, y / value, z / value);
}

template<typename T>
T Vec3T<T>::Dot(const Vec3T<T>& other) const
{
	return T((x * other.x) + (y * other.y) + (z * other.z));
}
template<typename T>
void Vec3T<T>::Zero()
{
	x = 0;
	y = 0;
	z = 0;
}
///////////////////////////////////////////////////////////////////////////////////////

///////////
// VEC 4 //
///////////

template <typename T>
Vec4T<T>::Vec4T(T _x, T _y, T _z, T _i)
{
	x = _x;
	y = _y;
	z = _z;
	i = _i;
}

template <typename T>
Vec4T<T>::Vec4T()
{
	x = 0;
	y = 0;
	z = 0;
	i = 0;
}

template <typename T>
std::string Vec4T<T>::ToString() const
{
	std::stringstream ss;
	ss << "X (" << x << ") Y (" << y << ") Z (" << z << ") I (" << i << ")";
	return (ss.str());
}

template <typename T>
Vec4T<T> Vec4T<T>::operator-() const
{
	return Vec4T<T>(-x, -y, -z, -i);
}

template <typename T>
Vec4T<T> & Vec4T<T>::operator-=(const Vec4T<T> & other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	i -= other.i;
	return (*this);
}

template <typename T>
Vec4T<T> & Vec4T<T>::operator+=(const Vec4T<T> & other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	i += other.i;
	return (*this);
}

template <typename T>
Vec4T<T> & Vec4T<T>::operator*=(const T & value)
{
	x *= value;
	y *= value;
	z *= value;
	i *= value;
	return (*this);
}

template<typename T>
Vec4T<T>& Vec4T<T>::operator/=(const T & value)
{
	x /= value;
	y /= value;
	z /= value;
	i /= value;
	return (*this);
}

template <typename T>
bool Vec4T<T>::operator==(const Vec4T<T> & other) const
{
	return (x == other.x && y == other.y && z == other.z && i == other.i);
}

template<typename T>
Vec4T<T> Vec4T<T>::operator-(const Vec4T & other) const
{
	return Vec4T<T>(x - other.x, y - other.y, z - other.z, i - other.i);
}

template<typename T>
Vec4T<T> Vec4T<T>::operator+(const Vec4T<T>& other) const
{
	return Vec4T<T>(x + other.x, y + other.y, z + other.z, i + other.i);
}

template<typename T>
Vec4T<T> Vec4T<T>::operator*(const T& value) const
{
	return Vec4T<T>(x * value, y * value, z * value, i * value);
}

template<typename T>
Vec4T<T> Vec4T<T>::operator/(const T& value) const
{
	return Vec4T<T>(x / value, y / value, z / value, i / value);
}

template<typename T>
T Vec4T<T>::Dot(const Vec4T<T>& other) const
{
	return T((x * other.x) + (y * other.y) + (z * other.z) + (i * other.i));
}

///////////////////////////////////////////////////////////////////////////////////////