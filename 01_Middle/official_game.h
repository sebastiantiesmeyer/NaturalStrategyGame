#pragma once
#include "abstract_game.h"

//
class OfficialGame : public AbstractGame
{
public:
	virtual glm::dvec2 getPlayerScore();
	OfficialGame(AbstractPlayer* p0, AbstractPlayer* p1, int board_size) : AbstractGame::AbstractGame(p0, p1, board_size) {}
protected:
	virtual void extra_rules();
	bool did_loose_player(int player);
	void train_for_player(UNIT_TYPE what_to_train, UnitProgress &unit_progress, int player);
	void set_outpost_ownership();

protected:
	int largest_id[2] = { 1,-1 };
};