#pragma once
#include "abstract_player.h"
#include "local.h"
#include <random>

class OLDGeneticPlayer : public AbstractPlayer
{
public:
	typedef std::vector<float> strang;
	typedef std::vector<strang> matrix;

	OLDGeneticPlayer(const PlayerParameters &pars) : AbstractPlayer::AbstractPlayer(pars) {}
	void mutate(float scope);
	void cross_over(matrix& genome, float scope);
	void init_weights(float scope);
	void init_gweights(float scope);

protected:

	std::default_random_engine rnd_engine;

	static constexpr int n_output = 5;
	static constexpr int n_input = 18;
	int output[n_output] ;
	int input[n_input];

	static constexpr int n_goutput = 3;
	static constexpr int n_ginput = 7;
	int goutput[n_goutput];
	int ginput[n_ginput];


	matrix weights = matrix(n_input, strang(n_output,0));
	matrix gweights = matrix(n_ginput, strang(n_goutput));


	void init_abst_weights(matrix weights, float scope);
	int * gpass(int input[]);
	int * wpass(int input[]);
	int * forward_pass(matrix lweights, int input[]);

	virtual void do_StartTurn();
	virtual void do_Render();
	virtual bool do_Update()
	{
		return endturn;
	}
	static bool endturn;
	std::vector<Options> stored_options;
};
