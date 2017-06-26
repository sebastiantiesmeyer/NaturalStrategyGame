#include "battle_arena.h"

BattleArena::BattleArena(const std::shared_ptr<AbstractPlayer> &p0, const std::shared_ptr<AbstractPlayer> &p1, int board_size, UNIT_TYPE u0, UNIT_TYPE u1, Position pos0, Position pos1)
	:AbstractGame::AbstractGame(p0, p1, board_size)
{
	create_unit(pos0, 0, u0);
	create_unit(auto_rotate(pos1, 1, board->size()), 1, u1);
}

void BattleArena::extra_rules()
{	//++cycle; //cannot be after "||", because or is lazy in C++.
	//Update: we dont even need to add to cycle as it is done automatically
	if (units->size() == 0 || cycle > max_cycles) score = glm::dvec2(-1, -1);
	if(units->size() == 2) score = glm::dvec2(0, 0);
	if (units->find(1) == units->end()) score =  glm::dvec2(-cycle, cycle);
	if (units->find(-1) == units->end()) score = glm::dvec2(cycle, -cycle);
}