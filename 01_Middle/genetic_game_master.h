#pragma once
#include "abstract_game.h"
#include "super_player.h"
#include "genetic_tactics.h"
#include <random>

struct strategy_wrapper{

	float fitness = 0;
	Board b;

	int n_input_strategy = 12;
	int n_output_strategy = 7;

	int n_input_tactics = 12;
	int n_output_tactics = 5;

	GeneticStrategy * gs = new GeneticStrategy(n_input_strategy, n_output_strategy);
	GeneticTactics * gt = new GeneticTactics(b, n_input_tactics, n_output_tactics);

	bool operator <(const strategy_wrapper& p) {
		return p.fitness < fitness;
	};
};

typedef std::vector<float> strang;
typedef std::vector<strang> matrix;

std::default_random_engine rnd_engine;

class GeneticGameMaster : AbstractGame
{
public:

	GeneticGameMaster();

	void play(int games);
	void initiate_players(int player_count);
	void save_players();

private:

	std::vector<strategy_wrapper> strategy_pool;

};

