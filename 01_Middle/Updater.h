#pragma once
#include <vector>
#include <list>
#include <functional>
#include "genetic_strategy.h"
#include"genetic_tactics.h"

class Updater
{
public:
typedef std::function<bool(int)> update_function_type; //Function type that return true when done, see Updater class.
	void AddHumanVsHuman(int game_size);
	void AddCyborgVsHeuristics(int game_size);
	void AddCyborgVsHuman(int game_size);
	void AddMutantvsHuman(int game_size, std::shared_ptr<GeneticTactics>  gt, std::shared_ptr<GeneticStrategy> gs);

	//Add an std::function object to the tasks
	//The function should return true until the task is completed
	//One argument: an integer: the number of times the function was called
	//Suggestion: use lambdas, ex.: updater.Addtask( [w](int iterations)->bool {return iterations >= w;} ); //(waits w iterations)
	inline void AddTask(const update_function_type &func) { tasks.push_back(func); }
	
	//memory can be freed up by deleting tasks that are not needed anymore
	inline void PopFirstTask() { tasks.pop_front(); }
	inline void PopLastTask() { tasks.pop_back(); }
	inline void SkipCurrentTask() { ++current_task_it; }
	inline void PreviousTask() { --current_task_it; }
	inline void SetToFirstTask() { current_task_it = tasks.begin(); }

	//Call this in the main loop after the tasks have been set
	//Calls the next task until it returns true, then the next one until all returned true
	inline void Update()
	{
		if(current_task_it == tasks.end()) return; //when there is no task to execute, does nothing
		if((*current_task_it)(iterations)) 
		{	//go to next task
			iterations = 0;
			++current_task_it;
		}
		else ++iterations;
	}

private:
	std::list<update_function_type> tasks; //tasks are functors of type int -> bool
	std::list<update_function_type>::iterator current_task_it = tasks.end(); //iterator pointing to next task to be executed
	int iterations = 0;		//the iteration count of a given task

	//int next_task_id = 0;	//the id of the task to be executed next
	//std::vector<update_function_type> tasks; //tasks are functors of type int -> bool
};