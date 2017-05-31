#pragma once
#include "abstract_player.h"
#include "local.h"

class GeneticPlayer : public AbstractPlayer
{
public:
	GeneticPlayer(const AbstPlayerConstrParams &pars) : AbstractPlayer::AbstractPlayer(pars) {}
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
