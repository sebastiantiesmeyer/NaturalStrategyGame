#pragma once
#include "abstract_game.h"

// A configurable battle arena for two players of any type.
// Primary use for this is training tactics (local strategy)
class BattleArena : public AbstractGame
{
public:
	BattleArena(AbstractPlayer*, AbstractPlayer*, int board_size, UNIT_TYPE, UNIT_TYPE, Position, Position); //call AbstractGame constructor
	/*virtual*/ glm::dvec2 getPlayerScore() {return score;}//Score eg. the cycles survived or 1/cycles needed to catch...
protected:
	/*virtual*/ void extra_rules(); // create 2 units at cycle = 0, eg. R, S. Game ends hern S dies.
};
