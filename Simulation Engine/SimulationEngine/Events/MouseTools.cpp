#include "MouseTools.h"

MouseRelativeObjectPos::MouseRelativeObjectPos(const size_t & widthObject, const size_t & heightObject, const float & scalePos)
{
	_scalePos = scalePos;
	setObjectDimension(widthObject, heightObject);
}

void MouseRelativeObjectPos::setObjectDimension(const size_t & widthObject, const size_t & heightObject)
{
	_width = widthObject;
	_height = heightObject;
	if (_width < 1)
		_width = 1;
	if (_height < 1)
		_height = 1;
	computeMinMaxPos();
}

void MouseRelativeObjectPos::setScalePos(const float & scalePos)
{
	_scalePos = scalePos;
	if (_scalePos > 1.0f)
		_scalePos = 1.0f;
	else if (_scalePos < 0.00001f)
		_scalePos = 0.00001f;
	computeMinMaxPos();
}

MouseRelativeObjectPos::Pos MouseRelativeObjectPos::getRelativePos(const size_t & xPosMouse, const size_t & yPosMouse)
{
	size_t			p = 0;

	if (yPosMouse < _yTop)
		p = 1;
	else if (yPosMouse > _yBot)
		p = 2;
	if (xPosMouse < _xLeft)
		p += (1 << 2);
	else if (xPosMouse > _xRight)
		p += (1 << 3);
	return ((Pos)(p));
}

std::string MouseRelativeObjectPos::posToString(const Pos & pos)
{
	if (pos == CENTER)
		return ("CENTER");
	else if (pos == TOP)
		return ("TOP");
	else if (pos == BOT)
		return ("BOT");
	else if (pos == LEFT)
		return ("LEFT");
	else if (pos == RIGHT)
		return ("RIGHT");
	else if (pos == TOPRIGHT)
		return ("TOP-RIGHT");
	else if (pos == TOPLEFT)
		return ("TOP-LEFT");
	else if (pos == BOTRIGHT)
		return ("BOT-RIGHT");
	else if (pos == BOTLEFT)
		return ("BOT-LEFT");
	return ("UNKNOWN");
}

void MouseRelativeObjectPos::computeMinMaxPos()
{
	size_t hheight, hwidth;

	hheight = (size_t)(_height / 2.0f);
	hwidth = (size_t)(_width / 2.0f);
	_xLeft = ((size_t)(hwidth)* _scalePos);
	_yTop = ((size_t)(hheight)* _scalePos);
	_xRight = (_width - _xLeft);
	_yBot = (_height - _yTop);
}
