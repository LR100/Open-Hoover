#pragma once

#include <string>
#include <thread>

#include <mutex>
#include <condition_variable>
#include <queue>
#include <future>
#include <functional>
#include <string>
#include <exception>

#include <map>

class ConsoleParser
{
public:
	ConsoleParser();
	~ConsoleParser();

	void			Launch();
	void			Stop();

	template<class Function>
	void SetActionOnCommand(const std::string & command, Function f)
	{
		Action& action = _actions[command];
		{
			action._action = (_std::function<void()>(f));
		}
	};

	template<class Function, class ...Args>
	inline auto SetActionOnCommand(const std::string & command, Function && f, Args && ...args)
	{
		using return_type = typename std::result_of<Function(Args...)>::type;
		Action& action = _actions[command];

		auto task = std::make_shared<std::packaged_task<return_type()>>
			(std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
		{
			action._action = [task]() { (*task)(); };
		}
	};

protected:

	virtual void		ParseEntry();

private:

	struct Action
	{
		std::function<void()>				_action;
		std::mutex							_mutex;
	};

	void			GetEntry();


	std::map<std::string, Action>	_actions;
	bool							_running;
	std::string						_entry;
	std::thread*					_thread;
};
