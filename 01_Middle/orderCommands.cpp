#include "orderCommands.h"
#include <vector>
#include <map>
#include <set>
#include <glm\glm.hpp>
#include <utility>
#include <list>

using namespace std;

bool dependent(const Units& units, const Command& c1, const Command& c2) //whether c1 is dependent on c2
{
	bool dependent = false;
	if (units.find(c1.id)->second.pos + c1.dir == units.find(c2.id)->second.pos) dependent = true;
	return dependent;
}


CommandQueue sortCommands(const CommandQueue& queue, const Units& units)
{
	CommandQueue newqueue;
	int length = queue.unitcmds.size();
	vector<list<int>> graph(length); //TODO: Build graph directly
	for (int i = 0; i < queue.unitcmds.size(); ++i)
	{
		for (int j = 0; j < queue.unitcmds.size(); ++j)
		{
			if (i != j && dependent(units, queue.unitcmds[i], queue.unitcmds[j]))
			{
				graph[i].push_back(j);
			}
		}
	}
	map<int, Command> commands;
	for (int i = 0; i < length; ++i) commands.insert(pair<int, Command>(i, queue.unitcmds[i]));

	int i = 0;
	while (!commands.empty() && i < length)
	{
		for (int j = 0; j < length; ++j)
		{
			if (graph[j].size() == 0 && commands.count(j) == 1)
			{
				newqueue.unitcmds.push_back(commands[j]);
				commands.erase(j);
				for (int k = 0; k < length; ++k) graph[k].remove(j);
			}
		}
		++i;
	}
	return newqueue;
}