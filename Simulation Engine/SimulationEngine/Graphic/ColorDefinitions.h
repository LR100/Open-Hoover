#pragma once

template <typename T>
class ColorDefT {
public:
	ColorDefT();
	ColorDefT(T _r, T _g, T _b);
	ColorDefT(T _r, T _g, T _b, T _a);
	// Attributes
	T r, g, b, a;
	// Methods
	bool			operator==(const ColorDefT<T>& other) const;
	bool			operator!=(const ColorDefT<T>& other) const;
	ColorDefT<T>&	operator*=(const T& value);
	ColorDefT<T>	operator*(const T& value);
	ColorDefT<T>&	operator+=(const ColorDefT<T>& other);
};

template<typename T>
inline ColorDefT<T>::ColorDefT()
{
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

template<typename T>
inline ColorDefT<T>::ColorDefT(T _r, T _g, T _b)
{
	r = _r;
	g = _g;
	b = _b;
	a = 0;
}

template<typename T>
inline ColorDefT<T>::ColorDefT(T _r, T _g, T _b, T _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

template<typename T>
inline bool ColorDefT<T>::operator==(const ColorDefT<T>& other) const
{
	return (!(r != other.r && g != other.g && b != other.b && a != other.a));
}

template<typename T>
inline bool ColorDefT<T>::operator!=(const ColorDefT<T>& other) const
{
	return (!(r == other.r && g == other.g && b == other.b && a == other.a));
}

template<typename T>
inline ColorDefT<T>& ColorDefT<T>::operator*=(const T& value)
{
	r *= value;
	g *= value;
	b *= value;
	return (*this);
}

template<typename T>
inline ColorDefT<T> ColorDefT<T>::operator*(const T& value)
{
	return (ColorDefT<T>(r * value, g * value, b * value));
}

template<typename T>
inline ColorDefT<T>& ColorDefT<T>::operator+=(const ColorDefT<T>& other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	return (*this);
}

// Float 
template class ColorDefT<float>;
typedef ColorDefT<float> ColorFDef;
// Char
template class ColorDefT<unsigned char>;
typedef ColorDefT<unsigned char> ColorDef;