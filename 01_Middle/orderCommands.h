#pragma once
#include "data_types.h"
#include <vector>
#include <map>
#include <set>
#include <glm\glm.hpp>
#include <utility>
#include <list>

using namespace std;

bool dependent(const Units& units, const Command& c1, const Command& c2); //whether c1 is dependent on c2

CommandQueue sortCommands(const CommandQueue& queue, const Units& units);