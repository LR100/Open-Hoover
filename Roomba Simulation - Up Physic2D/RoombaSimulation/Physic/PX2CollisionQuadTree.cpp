#include "PX2CollisionQuadTree.h"

float PX2CollisionQuadTree::PASS = 0;

PX2CollisionQuadTree::PX2CollisionQuadTree(const AABB2& _boundary, unsigned short maxDepth) : PX2CollisionQuadTreeT(_boundary, maxDepth)
{
}

PX2CollisionQuadTree::~PX2CollisionQuadTree()
{
}

void PX2CollisionQuadTree::Update()
{
	std::vector<PX2CollisionQuadTree::Node::Index*> parents;

	//std::cout << "PX2CollisionQuadTree::Update()" << std::endl;
	// Reserve MAXIMUM Indexes Possibility
	parents.reserve(GetRoot()->GetChildrenIndexesCount());
	// First Reset Manifold List
	_manifoldList.Reset();
	// Then Set it and Clear Empty Nodes BTW
	BuildManifoldListAndCleanEmpty(GetRoot(), parents);
	_manifoldList.SetManifoldSortedByType();
}

const PX2ManifoldList & PX2CollisionQuadTree::GetManifoldList() const
{
	return (_manifoldList);
}

void PX2CollisionQuadTree::CreateCollisionsManifolds(const DList<PX2CollisionQuadTree::Node::Index*>& objects, const std::vector<PX2CollisionQuadTree::Node::Index*>& parentsObjects)
{
	//std::cout << "CreateCollisionsManifolds" << std::endl;
	// Parents Objects

	DList<PX2CollisionQuadTree::Node::Index*>::Node* cursor;
	DList<PX2CollisionQuadTree::Node::Index*>::Node* cursorOther;

	cursor = objects.GetFront();
	for (size_t m = 0; m < objects.GetSize(); m += 1)
	{
		_a = cursor->data->GetData()->body;
		for (size_t n = 0; n < parentsObjects.size(); n += 1)
		{
			_b = parentsObjects.at(n)->GetData()->body;
			if (_a->GetSweptVolume().IntersectAABB(_b->GetSweptVolume())) // Check If Collide before Insertion
				_manifoldList.PushFrontManifold(_a, _b);
		}
		cursor = cursor->next;
	}
	// And Self Objects
	cursor = objects.GetFront();
	for (size_t m = 0; m < objects.GetSize(); m += 1)
	{
		cursorOther = cursor->next;
		_b = cursor->data->GetData()->body;
		for (size_t n = (m + 1); n < objects.GetSize(); n += 1)
		{
			_a = cursorOther->data->GetData()->body;
			if (_a->GetSweptVolume().IntersectAABB(_b->GetSweptVolume())) // Check If Collide before Insertion
				_manifoldList.PushFrontManifold(_a, _b);
			cursorOther = cursorOther->next;
		}
		cursor = cursor->next;
	}
}
//
//void PX2CollisionQuadTree::BuildManifoldListAndCleanEmpty(PX2CollisionQuadTree::Node * node, std::vector<PX2CollisionQuadTree::Node::Index*>& parentsObjects)
//{
//	NodeSetIndexesParents(node, parentsObjects);
//
//	// No More Children Objects To Add -> Stop and Clean Old Node
//	if (node->GetChildrenIndexesCount() == 0)
//	{
//
//		// Add Objects Collisions Left
//		if (node->GetIndexes().size())
//		{
//			CreateCollisionsManifolds(node->GetIndexes(), node->GetIndexesParents());
//		}
//
//		NodeReleaseChildrenAll(node);
//		return;
//	}
//
//	if (node->GetIndexes().size())
//	{
//		CreateCollisionsManifolds(node->GetIndexes(), node->GetIndexesParents());
//
//		std::vector<Node::Index*> parents = NodeGetIndexesParents(node);
//
//
//		parents.insert(parents.end(), parents);
//		//NodeGetIndexesParents(node).insert(NodeGetIndexesParents(node).end(), node->GetIndexes().begin(), node->GetIndexes.end());
//	}
//
//	if (node->GetChildren()[0] != NULL)
//	{
//		BuildManifoldListAndCleanEmpty(NodeGetChildren(node)[0], NodeGetIndexesParents(node));
//		BuildManifoldListAndCleanEmpty(NodeGetChildren(node)[1], NodeGetIndexesParents(node));
//		BuildManifoldListAndCleanEmpty(NodeGetChildren(node)[2], NodeGetIndexesParents(node));
//		BuildManifoldListAndCleanEmpty(NodeGetChildren(node)[3], NodeGetIndexesParents(node));
//	}
//}

//#include <iostream> // TMP
//void PX2CollisionQuadTree::UpdateTree()
//{
//
//	std::vector<QuadNodeAABB2 ObjectIndex*> parents;
//
//	// First Reset Manifold List
//	_manifoldList.Reset();
//	// Then Set it and Clear Empty Nodes BTW
//	//ClearEmptyNodes(_root);
//	BuildManifoldListAndCleanEmpty(_root, parents);
//	_manifoldList.SetManifoldSortedByType();
//}
//
//
//void PX2CollisionQuadTree::CreateCollisionsManifolds(const std::vector<QuadNodeAABB2 ObjectIndex*>& objects, const std::vector<QuadNodeAABB2 ObjectIndex*>& parentsObjects)
//{
//	PX2Body* a;
//	PX2Body* b;
//	// Parents Objects
//	for (size_t m = 0; m < objects.size(); m += 1)
//		for (size_t n = 0; n < parentsObjects.size(); n += 1)
//		{
//			a = (PX2Body*)objects.at(m)->GetID();
//			b = (PX2Body*)parentsObjects.at(n)->GetID();
//			
//			if (a->GetSweptVolume().IntersectAABB(b->GetSweptVolume()) ) // Check If Collide before Insertion
//				_manifoldList.PushFrontManifold(a, b);
//		}
//	// And Self Objects
//	for (size_t m = 0; m < objects.size(); m += 1)
//		for (size_t n = (m + 1); n < objects.size(); n += 1)
//		{
//			a = (PX2Body*)objects.at(m)->GetID();
//			b = (PX2Body*)objects.at(n)->GetID();
//
//			if (a->GetSweptVolume().IntersectAABB(b->GetSweptVolume())) // Check If Collide before Insertion
//				_manifoldList.PushFrontManifold(a, b);
//		}
//}
//
//void PX2CollisionQuadTree::BuildManifoldListAndCleanEmpty(QuadNodeAABB2  node, std::vector<QuadNodeAABB2 ObjectIndex*>& parentsObjects)
//{
//	node->parentsObjects = parentsObjects;
//
//	// No More Children Objects To Add -> Stop and Clean Old Node
//	if (node->GetChildrenObjectsCount() == 0)
//	{
//
//		// Add Objects Collisions Left
//		if (node->objects.size())
//		{
//			CreateCollisionsManifolds(node->objects, node->parentsObjects);
//		}
//		node->ReleaseAllChildren();
//		return;
//	}
//
//	if (node->objects.size())
//	{
//		CreateCollisionsManifolds(node->objects, node->parentsObjects);
//		node->parentsObjects.insert(node->parentsObjects.end(), node->objects.begin(), node->objects.end());
//	}
//
//	if (node->northWest != NULL)
//	{
//		BuildManifoldListAndCleanEmpty(node->northWest, node->parentsObjects);
//		BuildManifoldListAndCleanEmpty(node->northEast, node->parentsObjects);
//		BuildManifoldListAndCleanEmpty(node->southWest, node->parentsObjects);
//		BuildManifoldListAndCleanEmpty(node->southEast, node->parentsObjects);
//	}
//}
