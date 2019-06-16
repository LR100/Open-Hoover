#pragma once

#include "../QuadTree.hpp"
#include "PX2ManifoldList.h"
#include "../Maths/IntersectionComputer.h" // For Raytracing in QuadTree

#include "PX2DebugDrawer.h" ///// TMPPPPPPPPPPPPP

class PX2CQTBodyData
{
public:
	PX2CQTBodyData()
	{
		body = NULL;
	}
	virtual ~PX2CQTBodyData() {};
	PX2Body*	body;
};

template class QuadTreeAABB2T<float, PX2CQTBodyData>;
typedef QuadTreeAABB2T<float, PX2CQTBodyData*> PX2CollisionQuadTreeT;

class PX2CollisionQuadTree : public PX2CollisionQuadTreeT
{
public:
	PX2CollisionQuadTree(const AABB2&_boundary, unsigned short maxDepth);
	virtual ~PX2CollisionQuadTree();

	void					Update();


	const PX2ManifoldList&	GetManifoldList() const;

	std::vector<const typename PX2CollisionQuadTree::Node*>		GetNodesInRay(const Vec2& origin, const Vec2& dir, float maxDist) const
	{
		std::vector<const typename PX2CollisionQuadTree::Node*> nodes;

		Vec2			positionMove(origin);
		Vec2			positionMoveOld(origin);

		const Node*		nodeMove;
		unsigned short	depthMove;
		Vec2			positionT;
		const int		incMax = 20;
		int				inc = 0;

		Vec2			microMove = (dir * 0.1f);
		Vec2			dist((dir * maxDist));
		Vec2			moved(0, 0);

		do
		{
			positionT = positionMove;
			// Should Check Collision with Objects while getting deeper in the nodes
			if ((nodeMove = GetDeepestNodeAtPosition(positionT)))
			{
				nodes.push_back(nodeMove);
				positionMoveOld = positionMove;
				IntersectionComputer::RayInsideAABB(positionMoveOld, dir, nodeMove->GetBoundary(), positionMove);
				positionMove += microMove;
			}

		} while (nodeMove);
		return (nodes);
	}

	struct Ray
	{
		Vec2	origin;
		Vec2	dir;
		Vec2	end;
		// Intersection Part
		float			t = 0;
		Vec2			point;
		Node::Index*	index = NULL;
		// Buffers
		float			ts[2];
		Vec2			points[2];
		Vec2			normals[2];
		AABB2			surrounding;

		Vec2			positionMove;
		Vec2			positionMoveOld;
		Vec2			microMove;
		const Node*		node = NULL;
	};

public:

	const Node::Index*		GetFirstIndexInRay(const Vec2& origin, const Vec2& dir, Vec2& intersection, float maxDist = 10000.0f) const
	{
		Ray			ray;

		ray.origin = origin;
		ray.dir = dir;
		ray.end = (origin + dir * maxDist);
		ray.t = FLT_MAX;
		ray.ts[0] = FLT_MAX;
		ray.ts[1] = FLT_MAX;
		ray.index = NULL;
		ray.point = origin;
		ray.positionMove = origin;
		ray.positionMoveOld = origin;
		ray.microMove = (dir * 0.001f);


		const Node*		nodeMove;
		unsigned short	depthMove;
		Vec2			positionT;

		const int		incMax = 20;
		int				inc = 0;


		PX2CollisionQuadTree::PASS = 0;


		//std::cout << "GetFirstIndexInRay" << std::endl;
		// FIRST PASS Does Not Count For Intersection Validation (Result are keep)
		/*if ((nodeMove = GetRoot()->GetDeepestNodeAtPosition(ray.positionMove)))
		{
			QuadtreeDrawer->DrawLine(nodeMove->GetBoundary().max.x, nodeMove->GetBoundary().min.y, nodeMove->GetBoundary().min.x, nodeMove->GetBoundary().max.y, Color(255, 255, 255));
			ray.positionMoveOld = ray.positionMove;
			IntersectionComputer::RayInsideAABB(ray.positionMoveOld, dir, nodeMove->GetBoundary(), ray.positionMove);
			QuadtreeDrawer->DrawCircle(ray.positionMoveOld.x, ray.positionMoveOld.y, 5, Color::RED());
		}
		PX2CollisionQuadTree::PASS += 1;

		ray.surrounding = AABB2::FindSurrounding(ray.positionMove, ray.positionMoveOld);

		ray.positionMove = ray.positionMoveOld;*/

		do
		{
			positionT = ray.positionMove;
			// Should Check Collision with Objects while getting deeper in the nodes
			SetDeepestNodeAtPositionAndTestRayWithBodies(GetRoot(), positionT, ray);
			nodeMove = ray.node;
			/*QuadtreeDrawer->DrawLine(nodeMove->GetBoundary().max.x, nodeMove->GetBoundary().min.y, nodeMove->GetBoundary().min.x, nodeMove->GetBoundary().max.y, Color(255, 255, 255));
			QuadtreeDrawer->DrawCircle(ray.positionMoveOld.x, ray.positionMoveOld.y, 5, Color::RED());*/
			//QuadtreeDrawer->DrawLine(positionMoveOld.x, positionMoveOld.y, positionMove.x, positionMove.y, Color::GREEN());
			//QuadtreeDrawer->DrawLine(positionMove.x, positionMove.y, positionMove.x + dir.x * 100.0f, positionMove.y + dir.y * 100.0f, Color(255, 15 * depthMove, 255));	

			if (ray.t != FLT_MAX) // STOP !!
			{
				//QuadtreeDrawer->DrawLine(ray.origin.x, ray.origin.y, ray.point.x, ray.point.y, Color::PURPLE());
				//QuadtreeDrawer->DrawCircleFill(ray.point.x, ray.point.y, 8, Color::ORANGE());
				ray.index = ray.index;
				nodeMove = NULL;
				inc = incMax;
				intersection = ray.point;
			}
			PX2CollisionQuadTree::PASS += 1;
			inc += 1;
			//std::cout << "PASS" << std::endl;
		} while (nodeMove && inc < incMax);

		//if (ray.t == FLT_MAX)
			//QuadtreeDrawer->DrawLine(ray.origin.x, ray.origin.y, ray.end.x, ray.end.y, Color::PURPLE());
		return (ray.index);
	}
	static float PASS;

private:



	void	SetDeepestNodeAtPositionAndTestRayWithBodies(const Node* node, const Vec2 & position, Ray& ray) const
	{
		if (node->GetBoundary().ContainsPoint(position))
		{
			//PX2DebugDrawer::DrawAABB(node->GetBoundary(), Color(155, 0, PX2CollisionQuadTree::PASS * 55));

			// Test Objects
			if (node->IsLeaf())
			{
				// Last Node Containing Point

			   // Compute AABB
				ray.positionMoveOld = ray.positionMove;
				IntersectionComputer::RayInsideAABB(ray.positionMoveOld, ray.dir, node->GetBoundary(), ray.positionMove);
				ray.positionMove += ray.microMove;
				ray.surrounding = AABB2::FindSurrounding(ray.origin, ray.positionMove);
				ray.node = node;
				//PX2DebugDrawer::DrawAABB(ray.surrounding, Color::ORANGE());
			}
			else
			{
				// Dir Test Coul Be Precompute ? Switch from dir ? (ex: going North -> switch from check north to south)
				SetDeepestNodeAtPositionAndTestRayWithBodies(node->GetChildren()[0], position, ray);
				SetDeepestNodeAtPositionAndTestRayWithBodies(node->GetChildren()[1], position, ray);
				SetDeepestNodeAtPositionAndTestRayWithBodies(node->GetChildren()[2], position, ray);
				SetDeepestNodeAtPositionAndTestRayWithBodies(node->GetChildren()[3], position, ray);
			}

			//
			//

			const DList<PX2CollisionQuadTree::Node::Index*>& indexes = node->GetIndexes();
			const DList<PX2CollisionQuadTree::Node::Index*>::Node* cursor = indexes.GetFront();
			const PX2Shape* shape; // Test Ray With Shape
			const PX2Body*	body;
			

			

			for (; cursor; cursor = cursor->next)
			{
				//std::cout << "Test Intersect" << std::endl;
				body = cursor->data->GetData()->body;
				shape = body->GetShape();

				if (ray.surrounding.IntersectAABB(body->GetAABB()))
				{
					// TEST RAY
					if (shape->GetType() == PX2Shape::TYPE::CIRCLE)
					{

						//std::cout << "With Circle" << std::endl;
						if (IntersectionComputer::LineCircle(ray.origin, ray.dir, body->GetPosition(), shape->GetRadius(), ray.ts, ray.points, ray.normals))
						{
							//std::cout << "INTERSECT" << std::endl;
							if (ray.ts[0] < ray.t)
							{
								ray.t = ray.ts[0];
								ray.point = ray.points[0];
								ray.index = cursor->data;
							}
						}
					}
					else if (shape->GetType() == PX2Shape::TYPE::BOX)
					{
						//std::cout << "With Box" << std::endl;
						if (IntersectionComputer::RayAABB(ray.origin, ray.dir, body->GetAABB(), ray.ts, ray.points, ray.normals))
						{
							//std::cout << "INTERSECT" << std::endl;
							if (ray.ts[0] < ray.t)
							{
								ray.t = ray.ts[0];
								ray.point = ray.points[0];
								ray.index = cursor->data;
							}
						}
					}
				}
			}
		}
	}

	/// ///////// ///
	////MANIFOLDS////
	/// ///////// ///


	void			CreateCollisionsManifolds(const DList<Node::Index*>& objects, const std::vector<Node::Index*>& parentsObjects);
	void			BuildManifoldListAndCleanEmpty(Node* node, std::vector<Node::Index*>& parentsObjects)
	{
		NodeSetIndexesParents(node, parentsObjects);

		// No More Children Objects To Add -> Stop and Clean Old Node
		if (node->GetChildrenIndexesCount() == 0)
		{

			// Add Objects Collisions Left
			if (node->GetIndexes().GetSize())
			{
				CreateCollisionsManifolds(node->GetIndexes(), node->GetIndexesParents());
			}

			NodeReleaseChildrenAll(node);
			return;
		}

		if (node->GetIndexes().GetSize())
		{
			CreateCollisionsManifolds(node->GetIndexes(), node->GetIndexesParents());

			std::vector<Node::Index*>& parents = NodeGetIndexesParents(node);

			const DList<PX2CollisionQuadTree::Node::Index*>& indexes = node->GetIndexes();
			const DList<PX2CollisionQuadTree::Node::Index*>::Node* cursor = indexes.GetFront();

			size_t i = parents.size();
			parents.resize(i + indexes.GetSize());
			
			for (; cursor; cursor = cursor->next)
			{
				//for (size_t i = 0; i < _indexes.size(); i += 1)
				//{

				parents.at(i) = cursor->data;
				i += 1;
				//}
			}
			//parents.insert(parents.end(), NodeGetIndexes(node).begin(), NodeGetIndexes(node).end());
		}

		if (!node->IsLeaf())
		{
			BuildManifoldListAndCleanEmpty(NodeGetChildren(node)[0], NodeGetIndexesParents(node));
			BuildManifoldListAndCleanEmpty(NodeGetChildren(node)[1], NodeGetIndexesParents(node));
			BuildManifoldListAndCleanEmpty(NodeGetChildren(node)[2], NodeGetIndexesParents(node));
			BuildManifoldListAndCleanEmpty(NodeGetChildren(node)[3], NodeGetIndexesParents(node));
		}
	}


	PX2Body*			_a;
	PX2Body*			_b;
	PX2ManifoldList		_manifoldList;
};

