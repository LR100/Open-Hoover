#pragma once

#include "Maths/AABB.h"
#include <vector>

// QuadTree Points to fit and subdivide all points to the best precise location
class QuadTreePoints
{
public:
	QuadTreePoints(const AABB2& _boundary, QuadTreePoints* _parent);
	~QuadTreePoints();
	
	// Constante arbitraire indiquant combien d'éléments peuvent être stockés dans ce nœud de QuadTreePoints
	const size_t QT_NODE_CAPACITY = 20;

	// Zone de délimitation alignée sur l'axe (représentée par sa demi-dimension et son centre)
	// représentant les limites de ce QuadTreePoints
	AABB2 					boundary;

	const QuadTreePoints*		GetDeepestNodeAtPosition(const Vec2& position) const;

	// Points de ce nœeud de QuadTreePoints
	std::vector<Vec2>			points;

	QuadTreePoints*				parent;
	// Enfants
	QuadTreePoints*				northWest;
	QuadTreePoints*				northEast;
	QuadTreePoints*				southWest;
	QuadTreePoints*				southEast;

	
	bool	InsertPoint(const Vec2& point);
	bool	Subdivide();
};