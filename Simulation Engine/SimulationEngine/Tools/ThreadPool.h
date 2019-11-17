#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <functional>
#include <string>
#include <exception>
#include "windows.h"


class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();
	void Init(size_t nb_thread);
	void Quit();
	bool IsWorking();

	template < class Function>
	void AddWork(Function f);
	template<class Function, class ...Args>
	auto AddWork(Function && f, Args && ...args);


private:

	struct WorkList
	{
		std::deque<std::function<void()>>	_works;
		std::mutex							_mutexWork;
		std::condition_variable				_condition;
		bool								_running;
	};

	class ThreadWorker
	{
	public:
		ThreadWorker(WorkList& poolList);
		void operator()();
		
	private:
		WorkList&		_poolList;
	};

	std::vector<std::thread>			_workers;
	WorkList							_workList;
};

#endif /* !_THREAD_POOL_H_ */

template<class Function>
inline void ThreadPool::AddWork(Function f)
{

	{ // acquire lock
		std::unique_lock<std::mutex> lock(_workList._mutexWork);

		// add the task
		_workList._works.push_back(std::function<void()>(f));
	} // release lock

	  // wake up one thread
	_workList._condition.notify_one();
}

template<class Function, class ...Args>
inline auto ThreadPool::AddWork(Function && f, Args && ...args)
{
	using return_type = typename std::result_of<Function(Args...)>::type;
	auto task = std::make_shared<std::packaged_task<return_type()>>
		(std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
	{ // acquire lock
		std::unique_lock<std::mutex> lock(_workList._mutexWork);

		// add the task
		_workList._works.push_back([task]() { (*task)(); });
	} // release lock

	  // wake up one thread
	_workList._condition.notify_one();
}