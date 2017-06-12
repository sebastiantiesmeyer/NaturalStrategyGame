#pragma once
#include "abstract_player.h"
#include "local.h"

// Heuristics controlled player. Implemented before the use of AbstractTactics and AbstractStrategy, but still supported
class SimplePlayer : public AbstractPlayer
{
public:
	SimplePlayer(const PlayerParameters &pars) : AbstractPlayer::AbstractPlayer(pars) {}
protected:
	virtual void do_StartTurn();
	virtual void do_Render();
	virtual bool do_Update()
	{
		return endturn;
	}
	static bool endturn;
	std::vector<Options> stored_options;
};