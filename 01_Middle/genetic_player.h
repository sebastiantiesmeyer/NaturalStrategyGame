#pragma once
#include "abstract_player.h"
#include "local.h"
#include <random>

class GeneticPlayer : public AbstractPlayer
{
public:
	GeneticPlayer(const PlayerParameters &pars) : AbstractPlayer::AbstractPlayer(pars) {}
protected:

	typedef std::vector<float> strang;
	typedef std::vector<strang> matrix;

	std::default_random_engine rnd_engine;

	static constexpr int n_output = 5;
	static constexpr int n_input = 18;
	int output[n_output] ;
	int input[n_input];

	matrix weights = matrix(n_input, strang(n_output,0));

	void init_weights(float scope);
	int * forward_pass(int input[]);
	void mutate(float scope);
	void cross_over(matrix& genome, float scope);


	virtual void do_StartTurn();
	virtual void do_Render();
	virtual bool do_Update()
	{
		return endturn;
	}
	static bool endturn;
	std::vector<Options> stored_options;
};
