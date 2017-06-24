#include "triad_tactic.h"
#include "local.h"
#include <vector>

Command TriadTactic::step(const Unit & unit, const OrderList & order_list)
{
	Command cmd;
	return cmd;
}

bool TriadTactic::is_in_triad(const Unit & unit)
{
	bool ret = false;
	std::vector<Position> near_positions = {Position(-1,-1), Position(-1,0), Position(-1,1), Position(0,-1), Position(0,1), Position(1,-1), Position(1,0), Position(1,1)};

	for (int i = 0; i < near_positions.size() - 1; ++i)
	{
		Position pos0 = unit.pos + near_positions[i];
		Position pos1 = unit.pos + near_positions[i];
		if (pos0.x >= 0 && pos0.x < board->size() && pos0.y >= 0 && pos0.y < board->size() && pos1.x >= 0 && pos1.x < board->size() && pos1.y >= 0 && pos1.y < board->size())
		{
			//all units are different
			if (unit.type != (board->at(pos0).unit)->type && unit.type != (board->at(pos1).unit)->type && (board->at(pos1).unit)->type != (board->at(pos0).unit)->type) return true;

		}

	}
	return ret;
}
