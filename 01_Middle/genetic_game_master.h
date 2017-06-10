#pragma once
#include "abstract_game.h"
#include "super_player.h"
#include "genetic_tactics.h"

struct strategy_wrapper{

	float fitness = 0;
	
	AbstractStrategy * gs = new GeneticStrategy();
	AbstractTactic * gt = new GeneticTactics();

	bool operator <(const strategy_wrapper& p) {
		return p.fitness < fitness;
	};
};

typedef std::vector<float> strang;
typedef std::vector<strang> matrix;

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

