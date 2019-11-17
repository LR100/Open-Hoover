#ifndef _RELATIVEPOSTION_H_
#define _RELATIVEPOSTION_H_

#include <iostream>
#include <vector>
#include <map>

enum RelativePos: int
{
	CENTER = 0,
	TOP = 1,
	BOT = 2,
	LEFT = 4,
	RIGHT = 8,
	TOPLEFT = 5,
	TOPRIGHT = 9,
	BOTLEFT = 6,
	BOTRIGHT = 10,
	NOPOS = 11
};

class RelativePosComputer
{
public:
	RelativePosComputer();
	~RelativePosComputer();

	template <typename T>
	void Move(const RelativePos& pos, T& x, T& y, const T& value)
	{
		if (pos == RelativePos::TOP || pos == RelativePos::TOPLEFT || pos == RelativePos::TOPRIGHT)
			y -= value;
		else if (pos == RelativePos::BOT || pos == RelativePos::BOTLEFT || pos == RelativePos::BOTRIGHT)
			y += value;
		if (pos == RelativePos::LEFT || pos == RelativePos::TOPLEFT || pos == RelativePos::BOTLEFT)
			x -= value;
		else if (pos == RelativePos::RIGHT || pos == RelativePos::TOPRIGHT || pos == RelativePos::BOTRIGHT)
			x += value;
	}
	RelativePos	GetOppositePos(const RelativePos& pos);
	size_t		GetAngleByPos(const RelativePos& pos); // Considering RIGHT is 0, like on trigonometric circle;
	RelativePos	GetNextDir(const RelativePos& dir);
	RelativePos	GetPrevDir(const RelativePos& dir);
	void		ShowDir(const RelativePos& dir);

private:

	size_t	GetPosDir(const RelativePos& dir);

	std::map<RelativePos, size_t>	_pos;
	std::vector<RelativePos>		_sorted;
};

#endif /* !_RELATIVEPOSTION_H_ */
