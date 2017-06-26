#pragma once
#include "abstract_player.h"
#include "local.h"

// Heuristics controlled player. Implemented before the use of AbstractTactics and AbstractStrategy, but still supported
class SimplePlayer : public AbstractPlayer
{
public:
	SimplePlayer() {}
	~SimplePlayer()
	{
		std::cout << "~SimplePlayer()" << std::endl;
	}
protected:
	virtual void do_StartTurn();
	virtual void do_Render();
	virtual bool do_Update()
	{
		return true;
	}
	static bool endturn;
	std::vector<Options> stored_options;
};