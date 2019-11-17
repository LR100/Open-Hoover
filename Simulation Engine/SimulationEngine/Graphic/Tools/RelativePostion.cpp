#include "RelativePostion.h"

RelativePosComputer::RelativePosComputer()
{
	_sorted.push_back(RIGHT);
	_pos[RIGHT] = (_sorted.size() - 1);
	_sorted.push_back(TOPRIGHT);
	_pos[TOPRIGHT] = (_sorted.size() - 1);
	_sorted.push_back(TOP);
	_pos[TOP] = (_sorted.size() - 1);
	_sorted.push_back(TOPLEFT);
	_pos[TOPLEFT] = (_sorted.size() - 1);
	_sorted.push_back(LEFT);
	_pos[LEFT] = (_sorted.size() - 1);
	_sorted.push_back(BOTLEFT);
	_pos[BOTLEFT] = (_sorted.size() - 1);
	_sorted.push_back(BOT);
	_pos[BOT] = (_sorted.size() - 1);
	_sorted.push_back(BOTRIGHT);
	_pos[BOTRIGHT] = (_sorted.size() - 1);
}


RelativePosComputer::~RelativePosComputer()
{

}

RelativePos RelativePosComputer::GetOppositePos(const RelativePos & pos)
{
	if (pos == TOP)
		return (BOT);
	else if (pos == LEFT)
		return (RIGHT);
	else if (pos == BOT)
		return (TOP);
	else if (pos == RIGHT)
		return (LEFT);
	else if (pos == BOTLEFT)
		return (TOPRIGHT);
	else if (pos == TOPRIGHT)
		return (BOTLEFT);
	else if (pos == TOPLEFT)
		return (BOTRIGHT);
	else if (pos == BOTRIGHT)
		return (TOPLEFT);
}

size_t RelativePosComputer::GetAngleByPos(const RelativePos & pos)
{
	if (pos == RIGHT)
		return (0);
	else if (pos == TOPRIGHT)
		return (45);
	else if (pos == TOP)
		return (90);
	else if (pos == TOPLEFT)
		return (135);
	else if (pos == LEFT)
		return (180);
	else if (pos == BOTLEFT)
		return (225);
	else if (pos == BOT)
		return (270);
	else if (pos == BOTRIGHT)
		return (315);
}

RelativePos RelativePosComputer::GetNextDir(const RelativePos & pos)
{
	size_t p = GetPosDir(pos);
	if (p == (_sorted.size() - 1))
		return (_sorted.at(0));
	return (_sorted.at(p + 1));
}

RelativePos RelativePosComputer::GetPrevDir(const RelativePos & pos)
{
	size_t p = GetPosDir(pos);
	if (p == 0)
		return (_sorted.at(_sorted.size() - 1));
	return (_sorted.at(p - 1));
}

void RelativePosComputer::ShowDir(const RelativePos & pos)
{
	if (pos == TOP)
		std::cout << "TOP";
	else if (pos == LEFT)
		std::cout << "LEFT";
	else if (pos == BOT)
		std::cout << "BOT";
	else if (pos == RIGHT)
		std::cout << "RIGHT";
	else if (pos == BOTLEFT)
		std::cout << "BOTLEFT";
	else if (pos == TOPRIGHT)
		std::cout << "TOPRIGHT";
	else if (pos == TOPLEFT)
		std::cout << "TOPLEFT";
	else if (pos == BOTRIGHT)
		std::cout << "BOTRIGHT";
}

size_t  RelativePosComputer::GetPosDir(const RelativePos & dir)
{
	if (!_pos.count(dir))
		return (0);
	return (_pos.at(dir));
}
