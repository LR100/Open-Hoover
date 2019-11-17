#pragma once

#include "DMemoryManager.h"

#include <iostream>


template <class T>
class DObject 
{
public:

	void Release()
	{
		_memManager->Release(this);
		IsRelease();
	}

	void * operator new(size_t size)
	{
		//std::cout << "Overloading new operator with size: " << size << std::endl;
		return (_memManager->Get());
	}

	// Be Sure That The Memory Manager is able to allow Block of the same size that the object size !!
	static DMemoryManager*		_memManager;

protected:

	virtual void	IsRelease() {};
};

template<class T>
DMemoryManager* DObject <T>::_memManager = new DMemoryManager(sizeof(T));

