#ifndef _COLORS_H_
#define _COLORS_H_

#include <vector>

class Color
{
public:
	Color(const unsigned int& avalue);
	Color(const unsigned char& _r, const unsigned char& _g, const unsigned char& _b);
	Color();
	~Color();
	
	Color(const Color& other);
	Color&	operator=(const Color& other);
	bool	operator==(const Color& other) const;
	bool	operator!=(const Color& other) const;

	static const Color& WHITE() { static Color color(255, 255, 255); return (color); }
	static const Color& BLACK() { static Color color(0, 0, 0); return (color); }
	static const Color& RED() { static Color color(255, 0, 0); return (color); }
	static const Color& GREEN() { static Color color(0, 255, 0); return (color); }
	static const Color& BLUE() { static Color color(0, 0, 255); return (color); }
	static const Color& ORANGE() { static Color color(255, 165, 0); return (color); }
	static const Color& YELLOW() { static Color color(255, 255, 0); return (color); }
	static const Color& PURPLE() { static Color color(130, 0, 130); return (color); }


	void				ComputeValue();
	std::string			ToString() const;

	unsigned char	r, g, b;
	unsigned int	value;
};

class ColorF
{
public:
	ColorF(const unsigned int& avalue);
	ColorF(const float& _r, const float& _g, const float& _b);
	ColorF();
	~ColorF();

	ColorF(const ColorF& other);
	ColorF& operator=(const Color& other);
	ColorF& operator=(const ColorF& other);



	ColorF& operator*=(const float& value);
	ColorF operator*(const float& value);
	ColorF& operator+=(const ColorF& other);

	void				ComputeValue();
	std::string			ToString() const;

	float			r, g, b;
	unsigned int	value;
};


#endif /* !_COLORS_H_ */
