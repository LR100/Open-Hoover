#include "PX2ManifoldList.h"

#include <algorithm> // To Sort Manifold to have DD first

PX2ManifoldList::PX2ManifoldList()
{	
	_nodeDDCount = 0;
}

PX2ManifoldList::~PX2ManifoldList()
{

}

const size_t & PX2ManifoldList::GetDDCount() const
{
	return (_nodeDDCount);
}

void PX2ManifoldList::Reset()
{
	Clear();
	_nodeDDCount = 0;
}

void PX2ManifoldList::PushBackManifold(PX2Manifold * manifold)
{
	if (manifold->Type() == 2) // DD
		_nodeDDCount += 1;
	PushBack(manifold);
}

void PX2ManifoldList::PushBackManifold(PX2Body * a, PX2Body * b)
{
	PX2Manifold*	manifold = new PX2Manifold(a, b);
	
	manifold->Set(a, b);
	if (manifold->Type() == 2) // DD
		_nodeDDCount += 1;
	PushBack(manifold);
}

void PX2ManifoldList::PushFrontManifold(PX2Manifold * manifold)
{
	if (manifold->Type() == 2) // DD
		_nodeDDCount += 1;
	PushFront(manifold);
}

void PX2ManifoldList::PushFrontManifold(PX2Body * a, PX2Body * b)
{
	PX2Manifold*	manifold = new PX2Manifold(a, b);

	//std::cout << "Push Manifold" << std::endl;

	manifold->Set(a, b);
	if (manifold->Type() == 2) // DD
		_nodeDDCount += 1;
	PushFront(manifold);
}

bool CompareManifoldType(PX2Manifold* a, PX2Manifold* b)
{
	return (a->Type() > b->Type());
}

void PX2ManifoldList::SetManifoldSortedByType()
{
	PX2ManifoldList::Node*	cursor = GetFront();

	// Allow Size for vector
	_manifoldSorted.clear();
	_manifoldSorted.resize(GetSize());

	for (size_t i = 0; i < GetSize(); i += 1)
	{
		_manifoldSorted.at(i) = cursor->data;
		cursor = cursor->next;
	}

	std::sort(_manifoldSorted.begin(), _manifoldSorted.end(), &CompareManifoldType);
}

const std::vector<PX2Manifold*>& PX2ManifoldList::GetManifoldSorted() const
{
	return (_manifoldSorted);
}

