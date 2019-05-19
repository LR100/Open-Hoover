#include "ValueTable.h"

#define MATH_TABLE_PI 3.14159265359f
#define MATH_TABLE_D_TO_R(n) (n * ( MATH_TABLE_PI / 180.0f))

PrecomputeFloatValueTable::PrecomputeFloatValueTable(unsigned int precision)
{
	_precision = precision;
}

PrecomputeFloatValueTable::~PrecomputeFloatValueTable()
{
}

unsigned int PrecomputeFloatValueTable::GetPrecision() const
{
	return (_precision);
}

float PrecomputeFloatValueTable::GetValue(unsigned int inc) const
{
	if (inc >= _table.size())
		inc = (inc % _table.size());
	return (_table.at(inc));
}

void PrecomputeFloatValueTable::ApplyMulCoef(float coef)
{
	for (unsigned int i = 0; i < _table.size(); i += 1)
		_table[i] *= coef;
}

unsigned int PrecomputeFloatValueTable::Size() const
{
	return (_table.size());
}

//#include "../Tools/Logger.h"

void PrecomputeFloatValueTable::InitValueTable(unsigned int precision, unsigned int nbValue)
{
	for (unsigned int i = 0; i < (nbValue * precision); i += 1)
	{
		_table.push_back(ComputeValue(precision, nbValue, i));
	}
	//GLogger::Get().GLog("")->CLog(" Size Of table %d", sizeof(std::vector<float>) + (sizeof(float) * _table.size()));
}

CosTable::CosTable(unsigned int precision) : PrecomputeFloatValueTable(precision)
{
	InitValueTable(precision, 360);
}

float CosTable::ComputeValue(unsigned int precision, unsigned int nbValue, unsigned int inc) const
{
	float value;
	if (inc > 0)
	{
		value = (float)inc / (float)precision;
	}
	else
	{
		value = 0;
	}
	return (cosf(MATH_TABLE_D_TO_R(value)));
}

SinTable::SinTable(unsigned int precision) : PrecomputeFloatValueTable(precision)
{
	InitValueTable(precision, 360);
}

float SinTable::ComputeValue(unsigned int precision, unsigned int nbValue, unsigned int inc) const
{
	float value;
	if (inc > 0)
	{
		value = (float)inc / (float)precision;
	}
	else
	{
		value = 0;
	}
	
	return (sinf(MATH_TABLE_D_TO_R(value)));
}
