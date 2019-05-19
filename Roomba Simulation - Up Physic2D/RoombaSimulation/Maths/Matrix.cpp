#include "Matrix.h"

template<typename T>
inline void Mat22T<T>::Zero()
{
	colA.Zero();
	colB.Zero();
}
