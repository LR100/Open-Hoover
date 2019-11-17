#pragma once

class PX2World;
class PX2Body;

#include <unordered_map>
#include <deque>
#include <vector>

#include "../Maths/Vector.h"

/// Managing Collision with a QuadTreePoints // Not anymore , eveything is based ont SweptVolume (CF: PX2BodyManager, And PX2Bodies)


//#include "QuadTreePoints.h" /// Exit this include
// Managing Collision with a QuadTreeAABB2 


#include "PX2DebugDrawer.h" // TMPP

#include "PX2CollisionSolver.h" 
#include "PX2CollisionQuadTree.h"



class PX2BodyManager
{
public:

	PX2BodyManager(PX2World * world);

	struct SpatializedObject
	{
		SpatializedObject();

		bool												exist;
		PX2Body*											body;
		QuadTreeAABB2T<float, AABB2*>::Node::Index*			index;
		PX2CollisionQuadTree::Node::Index*					indexCollision;
		PX2CQTBodyData										cqtData;
	};

	void										Simulate(const float& timePassed, const Vec2& gravity);

	size_t										AddBody(PX2Body* body);

	PX2Body*									GetBody(const size_t& id) const;
	std::vector<PX2Body*>						GetBodies() const;
	std::vector<PX2Body*>						GetBodiesInAABB(const AABB2&aabb) const;
	std::vector<PX2Body*>						GetBodiesAtPosition(const Vec2& pos) const;
	

	void										DestroyBody(const size_t& id);
	void										DestroyAllBodies();

	const QuadTreeAABB2T<float, AABB2*>* 		GetTreeBodies() const;
	const PX2CollisionQuadTree* 				GetTreeCollision() const;


	const std::unordered_map<size_t, PX2Shape*>&	GetShapes()	const;
	void											ClearAllShapes();


	const float&								GetHighVelocityLength() const;


	// RETURN SIZE_T MAX On NON Available Case
	size_t										GetBodyAvailableID();

private:

	void										CorrectNeighborsAtInsert(PX2CollisionQuadTree::Node::Index*	index);

	

	// BODIIIIIES //
	std::deque<size_t>												_bodiesAvailableIDs;
	size_t															_bodyCountMax;
	std::vector<SpatializedObject*>									_bodies;

	// !! NO SHAPE -> NO COLLISIONS !!

	// Objects Are Refered By Their ID
	std::unordered_map<size_t, SpatializedObject*>					_bodiesShapedDynamic;
	std::unordered_map<size_t, SpatializedObject*>					_bodiesShapedStatic;
	std::unordered_map<size_t, SpatializedObject*>					_bodiesNoShaped; // NO COLLISION (FREE MOVE !!!!!)


	// SHAPEEEESSS //

	std::unordered_map<size_t, PX2Shape*>							_shapes;

	// WOOOORLD AND TREEESS and Also COLLISIIIIIOOOON //

	PX2World*										_world;
	AABB2 											_aabbWorld;

	// QuadTree AABB2 ave been created FOR Collisions System Or Item Localisation 
	QuadTreeAABB2T<float, AABB2*>*					_treeBodiesShaped;
	// PX2Collision is a QuadTree AABB2 hich have been upgraded for collision 
	PX2CollisionQuadTree*							_treeBodiesShapedCollision;



	//const size_t MAX_COLLISIONS = 500000; // 100 000
	//std::vector<PX2Manifold*>						_pairs;
	float											_highVelocityLength;
	
	PX2CollisionListener*							_collisionListener;
	PX2CollisionSolver*								_collisionSolver;

	float											_timeAccumulator;
};