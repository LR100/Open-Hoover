#include "Vector.h"

///////////////////////////////////////////////////////////////////////////////////////

///////////
// VEC 4 //
///////////
//
//template <typename T>
//Vec4T<T>::Vec4T(T _x, T _y, T _z, T _i)
//{
//	x = _x;
//	y = _y;
//	z = _z;
//	i = _i;
//}
//
//template <typename T>
//Vec4T<T>::Vec4T()
//{
//	x = 0;
//	y = 0;
//	z = 0;
//	i = 0;
//}
//
//template <typename T>
//std::string Vec4T<T>::ToString() const
//{
//	std::stringstream ss;
//	ss << "X (" << x << ") Y (" << y << ") Z (" << z << ") I (" << i << ")";
//	return (ss.str());
//}
//
//template <typename T>
//Vec4T<T> Vec4T<T>::operator-() const
//{
//	return Vec4T<T>(-x, -y, -z, -i);
//}
//
//template <typename T>
//Vec4T<T> & Vec4T<T>::operator-=(const Vec4T<T> & other)
//{
//	x -= other.x;
//	y -= other.y;
//	z -= other.z;
//	i -= other.i;
//	return (*this);
//}
//
//template <typename T>
//Vec4T<T> & Vec4T<T>::operator+=(const Vec4T<T> & other)
//{
//	x += other.x;
//	y += other.y;
//	z += other.z;
//	i += other.i;
//	return (*this);
//}
//
//template <typename T>
//Vec4T<T> & Vec4T<T>::operator*=(const T & value)
//{
//	x *= value;
//	y *= value;
//	z *= value;
//	i *= value;
//	return (*this);
//}
//
//template<typename T>
//Vec4T<T>& Vec4T<T>::operator/=(const T & value)
//{
//	x /= value;
//	y /= value;
//	z /= value;
//	i /= value;
//	return (*this);
//}
//
//template <typename T>
//bool Vec4T<T>::operator==(const Vec4T<T> & other) const
//{
//	return (x == other.x && y == other.y && z == other.z && i == other.i);
//}
//
//template<typename T>
//Vec4T<T> Vec4T<T>::operator-(const Vec4T & other) const
//{
//	return Vec4T<T>(x - other.x, y - other.y, z - other.z, i - other.i);
//}
//
//template<typename T>
//Vec4T<T> Vec4T<T>::operator+(const Vec4T<T>& other) const
//{
//	return Vec4T<T>(x + other.x, y + other.y, z + other.z, i + other.i);
//}
//
//template<typename T>
//Vec4T<T> Vec4T<T>::operator*(const T& value) const
//{
//	return Vec4T<T>(x * value, y * value, z * value, i * value);
//}
//
//template<typename T>
//Vec4T<T> Vec4T<T>::operator/(const T& value) const
//{
//	return Vec4T<T>(x / value, y / value, z / value, i / value);
//}
//
//template<typename T>
//T Vec4T<T>::Dot(const Vec4T<T>& other) const
//{
//	return T((x * other.x) + (y * other.y) + (z * other.z) + (i * other.i));
//}
//
///////////////////////////////////////////////////////////////////////////////////////