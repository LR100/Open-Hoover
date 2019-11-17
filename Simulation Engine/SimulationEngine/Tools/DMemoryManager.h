#pragma once

#include <deque>

class DMemoryManager
{
public:
	DMemoryManager(size_t sizeObject);
	virtual ~DMemoryManager() {};

	void*		Get();
	void		Release(void* object);
	// 
	void		Resize(size_t objectsCount);

private:

	// Managed Memory (Block of the same size)
	std::deque<void*>		_releasedObjects;
	size_t					_size;
};


