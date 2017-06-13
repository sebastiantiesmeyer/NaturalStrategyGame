#pragma once
#include "abstract_game.h"
#include "super_player.h"
#include "genetic_tactics.h"
#include <random>

struct strategy_wrapper{

	float fitness = 0;
	Board b;
	GeneticStrategy * gs ;
	GeneticTactics * gt = new GeneticTactics(b);

	bool operator <(const strategy_wrapper& p) {
		return p.fitness < fitness;
	};
};

typedef std::vector<float> strang;
typedef std::vector<strang> matrix;

std::default_random_engine rnd_engine;

class GeneticGameMaster
{
public:

	GeneticGameMaster();
	void play(int games);
	void initiate_players(int player_count);
	void save_players();

private:

	std::vector<strategy_wrapper> strategy_pool;

	

};

