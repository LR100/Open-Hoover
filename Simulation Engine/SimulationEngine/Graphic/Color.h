#ifndef _COLORS_H_
#define _COLORS_H_

#include <vector>
#include <string>
#include "ColorDefines.h"
#include "ColorDefinitions.h"

class Color
{
public:

	Color();
	Color(const ColorDef& def);
	Color(const ColorFDef& fdef);
	Color(const ColorName& name);
	Color(const ColorName& name, const ColorFormat& format);
	Color(const ColorFormat& format);
	Color(const unsigned char& r, const unsigned char& g, const unsigned char& b);
	Color(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);
	Color(const unsigned char& r, const unsigned char& g, const unsigned char& b, const ColorFormat& format);
	Color(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a, const ColorFormat& format);

	virtual ~Color() {};

	Color&					operator=(const Color& other);
	bool					operator==(const Color& other) const;
	bool					operator!=(const Color& other) const;
	const ColorFormat&		GetFormat() const;
	const ColorDef&			GetDef() const;
	std::string				ToString() const;

	void					Set(const unsigned char& r, const unsigned char& g, const unsigned char& b);
	void					Set(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);
	void					Set(const ColorDef& def);

	// Not Compliant with Syntax Norms for more comfort of use
	const unsigned char&	r() const;
	const unsigned char&	g() const;
	const unsigned char&	b() const;
	const unsigned char&	a() const;
	const unsigned int&		value() const;

	// DEFAULT COLORS //
	static const Color& WHITE() { static Color color(ColorName::WHITE); return (color); }
	static const Color& BLACK() { static Color color(0, 0, 0); return (color); }
	static const Color& GREY() { static Color color(120, 120, 120); return (color); }
	static const Color& RED() { static Color color(255, 0, 0); return (color); }
	static const Color& GREEN() { static Color color(0, 255, 0); return (color); }
	static const Color& BLUE() { static Color color(0, 0, 255); return (color); }
	static const Color& ORANGE() { static Color color(255, 165, 0); return (color); }
	static const Color& YELLOW() { static Color color(255, 255, 0); return (color); }
	static const Color& PURPLE() { static Color color(130, 0, 130); return (color); }

private:

	// Compute Value by Format
	void					ComputeValue();
	static unsigned int		ComputeValueRGB(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);
	static unsigned int		ComputeValueRGBA(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);
	static unsigned int		ComputeValueARGB(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);

	void				_init(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a, const ColorFormat& format);
	void				_initFromName(const ColorName& name);
	ColorFormat			_format;
	unsigned int		_value;
	ColorDef			_def;

	
	typedef unsigned int (*ComputeValueFCT)(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);
	static const ComputeValueFCT	_computeValues[COLOR_FORMAT_COUNT];
	ComputeValueFCT					_computeValue;
};

#include <iostream> // TMP
class ColorFactory
{
public:

	static ColorFactory& Get()
	{
		static ColorFactory factory;
		return (factory);
	}

	void						SetFormat(ColorFormat format);
	Color						Create();
	Color						Create(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);
	const ColorFormat&			GetFormat() const;
	const unsigned int&			GetFormatBytesPerPixel() const;

private:
	ColorFactory();

	ColorFormat			_format;
	unsigned int		_formatBytesPerPixel;
};

//class ColorRGBF
//{
//public:
//	ColorRGBF(const unsigned int& avalue);
//	ColorRGBF(const float& _r, const float& _g, const float& _b);
//	ColorRGBF();
//	~ColorRGBF();
//
//	ColorRGBF(const ColorRGBF& other);
//	ColorRGBF& operator=(const ColorRGB& other);
//	ColorRGBF& operator=(const ColorRGBF& other);
//
//
//
//	ColorRGBF& operator*=(const float& value);
//	ColorRGBF operator*(const float& value);
//	ColorRGBF& operator+=(const ColorRGBF& other);
//
//	void				ComputeValue();
//	std::string			ToString() const;
//
//	float			r, g, b;
//	unsigned int	value;
//};


#endif /* !_COLORS_H_ */
