#pragma once
#include "abstract_game.h"

//The game described in the project proposal
//	(former implementation was class GameMaster)
//  implements the following game dynamicts on top of AlbstractGame:
//	Capturing bases and outposts
//	Deciding when the game ands and appointing a winner
//	Training units, increasing training times over time, decreasing when putposts are own
class OfficialGame : public AbstractGame
{
public:
	OfficialGame(AbstractPlayer* p0, AbstractPlayer* p1, int board_size) : AbstractGame::AbstractGame(p0, p1, board_size) {}
	/*virtual*/ glm::dvec2 getPlayerScore();
protected:
	/*virtual*/ void extra_rules();
	bool did_loose_player(int player);
	void train_for_player(UNIT_TYPE what_to_train, UnitProgress &unit_progress, int player);
	void set_outpost_ownership();
protected:
	int largest_id[2] = { 1,-1 };
};