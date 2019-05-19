#include "DMemoryManager.h"

DMemoryManager::DMemoryManager(size_t size)
{
	_size = size;
}

void * DMemoryManager::Get()
{
	void*	obj;

	if (_releasedObjects.size())
	{
		obj = _releasedObjects.back();
		_releasedObjects.pop_back();
	}
	else
	{
		obj = malloc(_size);
	}
	return (obj);
}

void DMemoryManager::Release(void * object)
{
	_releasedObjects.push_back(object);
}

void DMemoryManager::Resize(size_t objectsCount)
{
	if (_releasedObjects.size() > objectsCount)
	{
		for (size_t i = (_releasedObjects.size() - 1); i > objectsCount; i--)
		{
			delete (_releasedObjects.at(i));
		}
		_releasedObjects.resize(objectsCount);
	}
	else if (_releasedObjects.size() < objectsCount)
	{
		_releasedObjects.resize(objectsCount);
		for (size_t i = (_releasedObjects.size() - 1); i > objectsCount; i--)
		{
			_releasedObjects.at(i) = malloc(_size);
		}
	}

	
}

