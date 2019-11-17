#include "QuadTreePoints.h"

QuadTreePoints::QuadTreePoints(const AABB2& _boundary, QuadTreePoints* _parent)
{
	parent = _parent;
	boundary = _boundary;
	boundary.ComputeMinMax();
	points.clear();
	northEast = NULL;
	northWest = NULL;
	southEast = NULL;
	southWest = NULL;
}

QuadTreePoints::~QuadTreePoints()
{
}

const QuadTreePoints * QuadTreePoints::GetDeepestNodeAtPosition(const Vec2 & position) const
{
	if (boundary.ContainsPoint(position))
	{
		if (northWest == NULL)
		{
			return (this); // Last Node Containing Point
		}
		else
		{
			const QuadTreePoints* goodNode;
			if ((goodNode = northWest->GetDeepestNodeAtPosition(position))) { return (goodNode); }
			if ((goodNode = southEast->GetDeepestNodeAtPosition(position))) { return (goodNode); }
			if ((goodNode = northEast->GetDeepestNodeAtPosition(position))) { return (goodNode); }
			if ((goodNode = southWest->GetDeepestNodeAtPosition(position))) { return (goodNode); }
		}
	}
	return (NULL);
}

#include <iostream> // TMP

bool QuadTreePoints::InsertPoint(const Vec2 & point)
{
	if (!boundary.ContainsPoint(point))
		return (false);

	
	if (points.size() < QT_NODE_CAPACITY)
	{
		points.push_back(point);
		//std::cout << "Push Point" << std::endl;
		return (true);
	}

	if (northWest == NULL)
	{
		if (!Subdivide())
			return (false);
	}

	if (northWest->InsertPoint(point)) { return (true); }
	if (southEast->InsertPoint(point)) { return (true); }
	if (northEast->InsertPoint(point)) { return (true); }
	if (southWest->InsertPoint(point)) { return (true); }
	

	return (false);
}

bool QuadTreePoints::Subdivide()
{
	AABB2 	nbound;
	//std::cout << "Subdivide" << std::endl;

	// Create SubNode
	nbound.halfSize.x = (boundary.halfSize.x / 2);
	nbound.halfSize.y = (boundary.halfSize.y / 2);

	if (nbound.halfSize.x <= 0 || nbound.halfSize.y <= 0)
		return (false);

	nbound.center.x = (boundary.center.x + nbound.halfSize.x);
	nbound.center.y = (boundary.center.y - nbound.halfSize.y);
	northEast = new QuadTreePoints(nbound, this);

	nbound.center.x = (boundary.center.x - nbound.halfSize.x);
	nbound.center.y = (boundary.center.y - nbound.halfSize.y);
	northWest = new QuadTreePoints(nbound, this);

	nbound.center.x = (boundary.center.x + nbound.halfSize.x);
	nbound.center.y = (boundary.center.y + nbound.halfSize.y);
	southEast = new QuadTreePoints(nbound, this);

	nbound.center.x = (boundary.center.x - nbound.halfSize.x);
	nbound.center.y = (boundary.center.y + nbound.halfSize.y);
	southWest = new QuadTreePoints(nbound, this);

	// Then Dispatch Existing Points into children
	int nbChildren = (points.size() - 1);
	while (nbChildren >= 0)
	{
		InsertPoint(points.at(nbChildren));
/*
			if (!northWest->InsertPoint(points.at(nbChildren)))
				if (!southEast->InsertPoint(points.at(nbChildren)))
					if (!southWest->InsertPoint(points.at(nbChildren)))
						return (false);*/
		//std::cout << "Parent Points Pushed" << std::endl;
		nbChildren -= 1;
	}
	
	return (true);
}

