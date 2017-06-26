#pragma once
#include "abstract_game.h"

// A configurable battle arena for two players of any type.
// Primary use for this is training tactics (local strategy)
class BattleArena : public AbstractGame
{
public:
	BattleArena(const std::shared_ptr<AbstractPlayer>&,const std::shared_ptr<AbstractPlayer>&, int board_size, UNIT_TYPE, UNIT_TYPE, Position, Position); //call AbstractGame constructor
	~BattleArena() {}
	virtual glm::dvec2 getPlayerScore() const {return score;}//Score eg. the cycles survived or 1/cycles needed to catch...
protected:
	virtual void extra_rules(); // create 2 units at cycle = 0, eg. R, S. Game ends hern S dies.
};
