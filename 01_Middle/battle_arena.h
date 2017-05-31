#pragma once
#include "abstract_game.h"

class BattleArena : AbstractGame
{
public:
	virtual glm::dvec2 getPlayerScore(); //Score eg. the cycles survived or 1/cycles needed to catch...
	BattleArena(AbstractPlayer*, AbstractPlayer*, int board_size); //call AbstractGame constructor
protected:
	virtual void extra_rules(); // create 2 units at cycle = 0, eg. R, S. Game ends hern S dies.
};
