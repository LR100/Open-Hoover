#pragma once

#include "PX2Manifold.h"
#include "../Tools/DList.h"
#include <vector>
 
class PX2ManifoldList : public DList<PX2Manifold*>
{
public:
	PX2ManifoldList();
	virtual ~PX2ManifoldList();

	
	void								Reset();

	void								PushBackManifold(PX2Manifold* manifold);
	void								PushBackManifold(PX2Body* a, PX2Body* b);

	void								PushFrontManifold(PX2Manifold* manifold);
	void								PushFrontManifold(PX2Body* a, PX2Body* b);

	const size_t&						GetDDCount() const;

	void								SetManifoldSortedByType();
	const std::vector<PX2Manifold*>&	GetManifoldSorted() const;

private:

	size_t							_nodeDDCount;
	std::vector<PX2Manifold*>		_manifoldSorted;
};

