#include <iostream>
#include <sstream>
#include "ThreadPool.h"

ThreadPool::ThreadPool()
{	
}

ThreadPool::~ThreadPool()
{
	Quit();
}

void ThreadPool::Init(size_t nb_thread)
{
	_workList._running = true;
	for (size_t inc = 0; inc < nb_thread; inc++)
		_workers.push_back(std::thread(ThreadWorker(_workList)));
}

void ThreadPool::Quit()
{
	if (_workList._running)
	{
		_workList._running = false;
		_workList._condition.notify_all();
		for (size_t inc = 0; inc < _workers.size(); inc++)
			_workers.at(inc).join();
		_workers.clear();
	}
}

bool ThreadPool::IsWorking()
{
	return (_workList._works.size());
}


ThreadPool::ThreadWorker::ThreadWorker(WorkList& poolList) : _poolList(poolList)
{
	std::cout << "New Thread" << std::endl;
	
	//_thread = new Thread(&ThreadPool::ThreadWorker::Loop, this);	
}


void ThreadPool::ThreadWorker::operator()()
{
	std::function<void()>	f;

	while (true)
	{
		/*
		La classe condition_variable est une primitive de synchronisation qui peut être utilisé pour bloquer un fil ou plusieurs threads en même temps, jusqu'à ce que:
		- une notification est reçue d'un autre thread
		- un délai expire, ou
		- un réveil faux se produit
		Tout thread qui a l'intention d'attendre le std::condition_variable doit acquérir un std::unique_lock premier.
		Les opérations d'attente atomique libérer le mutex et de suspendre l'exécution du thread.
		Lorsque la variable de condition est notifié, le thread est réveillé, et le mutex est acquise de nouveau .
		Le std::condition_variable classe est un StandardLayoutType. Il n'est pas CopyConstructible, MoveConstructible, CopyAssignable, MoveAssignable .
		*/
		{ // Lock
			std::unique_lock<std::mutex>	lock(_poolList._mutexWork);
			while (_poolList._running && _poolList._works.empty())
				_poolList._condition.wait(lock);
			if (!_poolList._running)
				return;
			f = _poolList._works.front();
			_poolList._works.pop_front();
		} // Release
		  f();
	}
	std::cout << "END THREAD" << std::endl;
}

