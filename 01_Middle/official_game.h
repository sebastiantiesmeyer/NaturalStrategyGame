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
	OfficialGame(const std::shared_ptr<AbstractPlayer>&p0, const std::shared_ptr<AbstractPlayer>&p1, int board_size)
		: AbstractGame::AbstractGame(p0, p1, board_size) {}
	virtual glm::dvec2 getPlayerScore() const;
	~OfficialGame() {}
protected:
	virtual void extra_rules();
private:
	bool did_loose_player(int player);
	void train_for_player(UNIT_TYPE what_to_train, UnitProgress &unit_progress, int player);
	void set_outpost_ownership();
	//int largest_id[2] = { 1,-1 }; //REMOVED
};