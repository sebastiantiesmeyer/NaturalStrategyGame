#pragma once

#pragma once
#include <vector>
#include <functional>

typedef std::function<bool(int)> update_function_type;

class Updater2
{
public:
	void AddTask(const update_function_type &func)
	{
		tasks.push_back(func);
	}
	void Update()
	{
		if(next_task_id < tasks.size())
		{
			bool b = tasks[next_task_id](iterations);
			if(b)
			{
				iterations = 0;
				++next_task_id;
			}
			else ++iterations;
		}
	}
private:
	std::vector<update_function_type> tasks;
	int next_task_id = 0;
	int iterations = 0;
};