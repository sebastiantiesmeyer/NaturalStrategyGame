#pragma once
#include "data_types.h"

//bool dependent(const Units& units, const Command& c1, const Command& c2); //whether c1 is dependent on c2

// Sorts issued commands to be able to execute the most of them,
//		so units are not stepping onto each other's toes (effective commands),
//		while the order of commands are changing as little as possible (stable priorities)
// When there is no cycle in the graph, all orders are issued.
// The minimum ammount of commands (edges) are removed to make the graph acyclic.
// The order is not changed where commands are independent.
CommandQueue sortCommands(const CommandQueue& queue, const Units& units);