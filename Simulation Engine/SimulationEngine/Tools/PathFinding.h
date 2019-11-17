#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_

#define PATHPINF 2147483646

#include <string>

// FOR 2D GRID IMPLEMENTATION
struct PathPoint
{
	PathPoint()
	{
		pos = 0;
		x = 0;
		y = 0;
		cost = 1;
		cost_fro_src = PATHPINF;
		visited = false;
		parent = NULL;
	}
	int			pos;
	int			x, y;
	int			cost;
	int			cost_fro_src;
	bool		visited;
	PathPoint	*parent;
};

struct PathPointSort
{
	bool operator()(const PathPoint* a, const PathPoint* b)
	{
		return (a->cost_fro_src > b->cost_fro_src);
	}
};

#endif _PATHFINDING_H_

