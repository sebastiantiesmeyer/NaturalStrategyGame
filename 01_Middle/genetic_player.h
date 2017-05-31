#pragma once
#include "abstract_player.h"
#include "local.h"
#include <random>

class GeneticPlayer : public AbstractPlayer
{
public:
	GeneticPlayer(const AbstPlayerConstrParams &pars) : AbstractPlayer::AbstractPlayer(pars) {}
protected:

	std::default_random_engine rnd_engine;


	int output[5] ;
	int input[18];

	float weights[sizeof(output)][sizeof(input)];

	void init_weights(float scope);
	void forward_pass();
	void mutate(float scope);
	void cross_over();


	virtual void do_StartTurn();
	virtual void do_Render();
	virtual bool do_Update()
	{
		return endturn;
	}
	static bool endturn;
	std::vector<Options> stored_options;
};
