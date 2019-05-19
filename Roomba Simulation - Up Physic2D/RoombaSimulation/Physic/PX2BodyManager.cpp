#include "PX2BodyManager.h"

#include "PX2Body.h"
#include "../Maths/VectorTransformer.h"

#include <unordered_set>
#include <algorithm>


#include <iostream> // TMP

#include "PX2CollisionQuadTree.h"



#define MAX_BODIES 10000
#define MAX_WIDTH_WORLD 10000.0f
#define MAX_HEIGHT_WORLD 10000.0f
 
PX2BodyManager::PX2BodyManager(PX2World * world)
{
	std::cout << "PX2BodyManager::Initialization..." << std::endl;

	_timeAccumulator = PX2_DT_STEP;

	_bodyCountMax = MAX_BODIES;

	_world = world;
	// World Size
	_aabbWorld.halfSize.x = (MAX_WIDTH_WORLD * .5f); // * .5f -> Half DImension
	_aabbWorld.halfSize.y = (MAX_HEIGHT_WORLD * .5f);
	std::cout << "Init AABB2 rld W(" << _aabbWorld.halfSize.x * 0.5f << ") H(" << _aabbWorld.halfSize.y * 0.5f << ")" << std::endl;

	// World Center
	_aabbWorld.center.x = _aabbWorld.halfSize.x;
	_aabbWorld.center.y = _aabbWorld.halfSize.y;
	_aabbWorld.ComputeMinMax();

	// Search the minimum depth for which the size of the node is 1
	float maxLengthWorld = std::max(_aabbWorld.halfSize.x, _aabbWorld.halfSize.y);
	unsigned short maxDepth = 0;

	while (maxLengthWorld >= 1.0f)
	{
		maxDepth += 1;
		maxLengthWorld *= 0.5f;
	}
	maxDepth += 1;

	// High Velocity MUST be proportional to world/Tree size
	float minSideSize = std::min(_aabbWorld.halfSize.x, _aabbWorld.halfSize.y);

	unsigned short maxDepthStep = (maxDepth / 3);
	for (unsigned short i = 0; i < maxDepthStep; i += 1)
		minSideSize *= 0.5f;


	_highVelocityLength = (minSideSize / PX2_DT_STEP);
	std::cout << "Min Side Size (" << minSideSize << ")" << std::endl;
	std::cout << "High Velocity Length (" << _highVelocityLength << ")" << std::endl;

	/*_pairs.reserve(MAX_COLLISIONS);
	for (size_t i = 0; i < MAX_COLLISIONS; i += 1)
	{
		_pairs.push_back(new PX2Manifold(NULL, NULL));
	}*/

	_treeBodiesShaped = new QuadTreeAABB2T<float, AABB2*>(_aabbWorld, maxDepth);
	_treeBodiesShapedCollision = new PX2CollisionQuadTree(_aabbWorld, maxDepth);

	// COLLISION SOLVER
	_collisionListener = new PX2CollisionListener(_bodyCountMax);
	_collisionSolver = new PX2CollisionSolver(_collisionListener);

	_bodies.resize(_bodyCountMax);
	// Init IDS For Bodies
	for (size_t i = 0; i < _bodyCountMax; i += 1)
	{
		_bodiesAvailableIDs.push_back(_bodyCountMax - i);
		_bodies.at(i) = new SpatializedObject();
	}
	// And Allow Space For Bodies



	std::cout << "PX2BodyManager::Initialization OK" << std::endl;
}

#include "PX2DebugDrawer.h" // TMP

void PX2BodyManager::Simulate(const float& timePassed, const Vec2& gravity)
{
	_timeAccumulator += timePassed;

	PX2Body*									body;
	//PX2Body*									neighbor;

	//const SpatializedObject*				object;

	Vec2	acceleration;
	Vec2	velOld;
	Vec2	vel;
	Vec2	posN;
	AABB2	aabbN;

	while (_timeAccumulator > PX2_DT_STEP) // Should be Replace by While
	{

		// SIMULATE ONLY SHAPED DYNAMIC OBJECTS FOR THE MOMENT
		std::unordered_map<size_t, SpatializedObject*>::iterator it = _bodiesShapedDynamic.begin();
		std::unordered_map<size_t, SpatializedObject*>::iterator itEnd = _bodiesShapedDynamic.end();

		while (it != itEnd)
		{
			body = it->second->body;
			// Save First AABB2in old SWEPT VOLUME

			aabbN = body->GetAABB();

			// May Have Changed
			aabbN.ComputeMinMax();
			//PX2DebugDrawer::DrawAABB(aabbN, Color(0, 0, 115));


			

			// For the moment we consider body is Not Coliding

			//Move Body

			//if (it->second.body->IsSleeping()) // Body Was at vel 0 So no need to reply gravity (force compensate)
			//	it->second.body->Move(timePassed);
			//else
			//{
			//	it->second.body->SetColliding(false);
			//	
			//}

			it->second->body->ApplyForce(gravity);
			it->second->body->Move(PX2_DT_STEP);
			

			// Recompute AABB2 Thoses are only Dynamic Objects) -> Static are not recompute
			it->second->body->_aabb.ComputeMinMax();

			// Compute SWEPT VOLUME

			// Make Swept Volume Old AABB2  litle Bit Bigger to avoid excluding Contact Too Fast
//#define UP_DIMENSION_COEF 0.05f
//			aabbN.halfSize += (aabbN.halfSize * UP_DIMENSION_COEF);
//			aabbN.ComputeMinMax();
			it->second->body->_sweptVolume = AABB2::FindSurrounding(aabbN, it->second->body->GetAABB());

			//PX2DebugDrawer::DrawAABB(it->second->body->_sweptVolume, Color(0, 0, 255));
			//PX2Window->Refresh();



			//acceleration = body->GetForce();

			//velOld = body->GetVelocityLinear();
			//// Must Recompute if is HighVelocity
			//vel = ((velOld + (acceleration * dt)));
			//
			//body->_positionOld = body->_aabb.center;
			//


			//body->SetVelocityLinear(vel);
			////lastVelocity.x = lastVelocity.x + (gravity.x * dt);
			////lastVelocity.y = lastVelocity.y + (gravity.y * dt);

			//aabbN = body->_sweptVolume;

			//aabbN.center.x = (aabbN.center.x + ((velOld.x + vel.x) * 0.5f * dt));
			//aabbN.center.y = (aabbN.center.y + ((velOld.y + vel.y) * 0.5f * dt));

			//aabbN.ComputeMinMax();

			//it->second->body->_sweptVolume = AABB2 FindSurrounding(aabbN, it->second->body->GetAABB());




			// Relocate SWEPT VOLUME in Collision Tree
			if (it->second->body->IsHighVelocityInWorld()) // From Top if Fast or from bot if slow
				it->second->indexCollision->RelocateFromTop();
			else
				it->second->indexCollision->RelocateFromBot();


			it++;
		}

		// Update Neighbors (From Changed Nodes) and clean Empty Nodes
		//_treeBodiesShaped->UpdateTree();

		/// BROAD PHASE 
		_treeBodiesShapedCollision->Update();

		const PX2ManifoldList&	manifoldList = _treeBodiesShapedCollision->GetManifoldList();
		_collisionSolver->Solve(manifoldList, PX2_DT_STEP);

		_timeAccumulator -= PX2_DT_STEP;
	}
	//system("pause");
}

// Other Formula (speed to compare)
/*Vec2 u1;

body->_velocity = u1;
neighbor->_velocity = u2;


u1.x = (((neighbor->_mass * neighbor->_velocity.x) * (body->_restitution + 1.0f)) + (body->_velocity.x * (body->_mass - neighbor->_mass) * body->_restitution)) / (body->_mass + neighbor->_mass);
u1.y = (((neighbor->_mass * neighbor->_velocity.y) * (body->_restitution + 1.0f)) + (body->_velocity.y * (body->_mass - neighbor->_mass) * body->_restitution)) / (body->_mass + neighbor->_mass);

Vec2 u2;

u2.x = (((body->_mass * body->_velocity.x) * (body->_restitution + 1.0f)) + (neighbor->_velocity.x * (neighbor->_mass - body->_mass) * body->_restitution)) / (body->_mass + neighbor->_mass);
u2.y = (((body->_mass * body->_velocity.y) * (body->_restitution + 1.0f)) + (neighbor->_velocity.y * (neighbor->_mass - body->_mass) * body->_restitution)) / (body->_mass + neighbor->_mass);


body->Move(10.0f);
neighbor->Move(10.0f);*/


#include <iostream> // TMP
size_t PX2BodyManager::AddBody(PX2Body * body)
{
	size_t idBody = body->GetID();
	if (idBody >= _bodies.size())
		return (0); // ERRROOR

	if (_bodies.at(idBody)->exist)
	{
		//std::cout << "Body with id (" << idBody << ") already exist\n";
		return (idBody);
	}

	SpatializedObject*	object = _bodies.at(idBody);

	_bodies.at(idBody)->exist = true;
	// SHAPED BODY
	const PX2Shape*	shape = body->GetShape();

	if (shape != NULL)
	{
		// Check IF Shapes is already indexed
		if (!_shapes.count((size_t)shape)) // else index it
			_shapes.emplace((size_t)shape, (PX2Shape*)shape);

		if (body->GetType() == PX2Body::TYPE::DYNAMIC)
		{
			_bodiesShapedDynamic[idBody] = object;

		}
		else
		{
			_bodiesShapedStatic[idBody] = object;
		}

		body->_aabb.ComputeMinMax();
		body->_sweptVolume = body->_aabb;

		object->body = body;
		object->cqtData.body = body;

		size_t addrBody = (size_t)(body);

		if (body->GetType() == PX2Body::TYPE::DYNAMIC) // If Have been inserted
		{
			object->index = _treeBodiesShaped->InsertData(body->GetAABB(), (AABB2*)&body->GetAABB());
			object->indexCollision = _treeBodiesShapedCollision->InsertData(body->GetSweptVolume(), &object->cqtData);

			/*if (object->index->node)
				CorrectNeighborsAtInsert(object->index);*/
				// Else will probably be inserted later in simulation loop (if free space)
		}
		else // STATIC
		{
			object->index = _treeBodiesShaped->InsertData(body->GetAABB(), (AABB2*)&body->GetAABB());
			object->indexCollision = _treeBodiesShapedCollision->InsertData(body->GetSweptVolume(), &object->cqtData);
		}

		body->SetSleeping(false); // Body Just Created is awake
	}

	return (idBody);
}

PX2Body * PX2BodyManager::GetBody(const size_t & id) const
{
	if (_bodies.at(id)->exist)
	{
		return (_bodies.at(id)->body);
	}
	return (NULL);
}

std::vector<PX2Body*> PX2BodyManager::GetBodies() const
{
	std::vector<PX2Body*> bodies;

	for (size_t i = 0; i < _bodies.size(); i += 1)
	{
		if (_bodies.at(i)->exist)
			bodies.push_back(_bodies.at(i)->body);
	}
	return (bodies);
}

std::vector<PX2Body*> PX2BodyManager::GetBodiesInAABB(const AABB2& aabb) const
{
	std::vector<PX2Body*> bodies;

	for (size_t i = 0; i < _bodies.size(); i += 1)
	{
		if (_bodies.at(i)->exist)
			bodies.push_back(_bodies.at(i)->body);
	}
	return (bodies);
}

std::vector<PX2Body*> PX2BodyManager::GetBodiesAtPosition(const Vec2 & pos) const
{
	std::vector<const PX2CollisionQuadTree::Node::Index*>	objectsIndexes = _treeBodiesShapedCollision->GetIndexesAtPosition(pos);
	std::vector<PX2Body*>									bodies;

	if (objectsIndexes.size())
	{
		bodies.resize(objectsIndexes.size());
		for (size_t i = 0; i < objectsIndexes.size(); i += 1)
		{
			bodies.at(i) = objectsIndexes.at(i)->GetData()->body;
		}
	}

	return (bodies);
}

void PX2BodyManager::DestroyBody(const size_t & id)
{
	if (_bodies.at(id)->exist)
	{
		_bodies.at(id)->exist = false;
		if (_bodies.at(id)->index)
			_bodies.at(id)->index->Remove();

		if (_bodies.at(id)->indexCollision)
			_bodies.at(id)->indexCollision->Remove();

		if (_bodies.at(id)->body->GetShape())
		{
			if (_bodies.at(id)->body->GetType() == PX2Body::TYPE::DYNAMIC)
				_bodiesShapedDynamic.erase(id);
			else
				_bodiesShapedStatic.erase(id);
		}
		else
		{
			_bodiesNoShaped.erase(id);
		}
			

		_bodiesAvailableIDs.push_back(id);

		delete (_bodies.at(id)->body);
	}
}

void PX2BodyManager::DestroyAllBodies()
{
	for (size_t i = 0; i < _bodies.size(); i += 1)
	{
		DestroyBody(i);
	}

	_bodiesNoShaped.clear();
	_bodiesShapedDynamic.clear();
	_bodiesShapedStatic.clear();
}

const QuadTreeAABB2T<float, AABB2*>*  PX2BodyManager::GetTreeBodies() const
{
	return (_treeBodiesShaped);
}

const PX2CollisionQuadTree*  PX2BodyManager::GetTreeCollision() const
{
	return (_treeBodiesShapedCollision);
}

const std::unordered_map<size_t, PX2Shape*>& PX2BodyManager::GetShapes() const
{
	return (_shapes);
}

void PX2BodyManager::ClearAllShapes()
{
	_shapes.clear();
}

const float & PX2BodyManager::GetHighVelocityLength() const
{
	return (_highVelocityLength);
}


void PX2BodyManager::CorrectNeighborsAtInsert(PX2CollisionQuadTree::Node::Index* index)
{
	std::vector<PX2CollisionQuadTree::Node::Index*>	neighbors;
	PX2Body*										neighbor;
	PX2Body*										body = (PX2Body*)index->GetID();
	PX2Manifold										manifold;
	PX2ManifoldList									manifoldList;

	//index->GetParentsObjects(neighbors);

	//std::cout << "Have (" << neighbors.size()  << ") neighbors" << std::endl;
	for (size_t c = 0; c < neighbors.size(); c += 1)
	{
		neighbor = (PX2Body *)neighbors.at(c)->GetID();
		manifold.Set(body, neighbor);

		manifoldList.PushBackManifold(body, neighbor);
	}

	_collisionSolver->Solve(manifoldList, 0.00001f);
}

size_t PX2BodyManager::GetBodyAvailableID()
{
	size_t	id;

	if (_bodiesAvailableIDs.size())
	{
		id = _bodiesAvailableIDs.back();
		_bodiesAvailableIDs.pop_back();
	}
	else
	{
		id = SIZE_MAX;
	}
	return (id);
}

PX2BodyManager::SpatializedObject::SpatializedObject()
{
	exist = false;
	body = NULL;
	index = NULL;
	indexCollision = NULL;
}
