#pragma once
#include "abstract_player.h"

// Fully human, flesh and bone, brain and nerve, manualy controled player
// Use this to control a player's every move
class HumanPlayer : public AbstractPlayer
{
public:
	HumanPlayer() = default;
	~HumanPlayer() {}
	virtual void do_StartTurn();
	virtual void do_Render();
	virtual bool do_Update(){return endturn;}
private:
	int max_iterations = 20;
	CommandQueue q2;
	bool endturn = false;
};
