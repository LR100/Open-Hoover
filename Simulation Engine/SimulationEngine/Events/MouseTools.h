#ifndef _MOUSETOOLS_H_
#define _MOUSETOOLS_H_

#include <string>

class MouseRelativeObjectPos
{
public:

	enum Pos
	{
		CENTER = 0,
		TOP = 1,
		BOT = 2,
		LEFT = 4,
		RIGHT = 8,
		TOPLEFT = 5,
		TOPRIGHT = 9,
		BOTLEFT = 6,
		BOTRIGHT = 10
	};

	MouseRelativeObjectPos(const size_t& widthObject, const size_t& heightObject, const float& scalePos);
	~MouseRelativeObjectPos() {};

	void	setObjectDimension(const size_t& widthObject, const size_t& heightObject);
	void	setScalePos(const float& scalePos);
	Pos		getRelativePos(const size_t& xPosMouse, const size_t& yPosMouse);
	std::string	posToString(const Pos& pos);

private:

	void	computeMinMaxPos();

	size_t	_width, _height;
	float	_scalePos;
	size_t	_xLeft, _xRight, _yTop, _yBot;
};

#endif /* !_MOUSETOOLS_H_ */