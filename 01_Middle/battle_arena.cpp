#include "battle_arena.h"

BattleArena::BattleArena(AbstractPlayer* p0, AbstractPlayer* p1, int board_size, UNIT_TYPE u0, UNIT_TYPE u1, Position pos0, Position pos1)
	:AbstractGame::AbstractGame(p0, p1, board_size)
{
	Unit new_unit0, new_unit1;
	new_unit0.id = 1;
	new_unit1.id = -1;
	new_unit0.moved = false;
	new_unit1.moved = false;
	new_unit0.player = 0;
	new_unit1.player = 1;
	new_unit0.type = u0;
	new_unit1.type = u1;
	new_unit0.pos = pos0;
	new_unit1.pos = pos1;
	units.emplace(std::make_pair(1, new_unit0));
	units.emplace(std::make_pair(-1, new_unit1));
	board(new_unit0.pos, new_unit0.player).id = new_unit0.id; //updating cell
	board(new_unit1.pos, new_unit1.player).id = new_unit1.id; //updating cell
	board(new_unit0.pos, new_unit0.player).unit = &units.at(new_unit0.id);
	board(new_unit1.pos, new_unit1.player).unit = &units.at(new_unit1.id);
}

void BattleArena::extra_rules()
{
	if (units.size() == 0 || ++cycle > max_cycles) score = glm::dvec2(-1, -1);
	if(units.size() == 2) score = glm::dvec2(0, 0);
	if (units.find(1) == units.end()) score =  glm::dvec2(-cycle, cycle);
	if (units.find(-1) == units.end()) score = glm::dvec2(cycle, -cycle);
}