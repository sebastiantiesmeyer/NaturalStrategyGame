#pragma once
#include "abstract_game.h"
#include "super_player.h"
#include "genetic_tactics.h"
#include "genetic_strategy.h"
#include <random>

struct strategy_wrapper
{
	float fitness = 0;
	//Board b;

	int n_input_strategy = 12;
	int n_output_strategy = 7;

	int n_input_tactics = 12;
	int n_output_tactics = 5;
	int scope = 1;

	
	std::shared_ptr<GeneticTactics> gt = std::make_shared<GeneticTactics>();

	std::shared_ptr<GeneticStrategy> gs = std::make_shared<GeneticStrategy>();

	bool operator <(const strategy_wrapper& p) const
	{	//const after the function means that it does not change the class.
		//we need this, so std::less can work with it
		return fitness < p.fitness;
	};
};

static std::default_random_engine rnd_engine;

class GeneticGameMaster
{
public:

	GeneticGameMaster(int board_size) : board_size(board_size){}
	//Constructor inheritance? TODO: implement

	void play(int games);
	void initiate_players(int player_count);
	void save_players();

private:

	std::vector<strategy_wrapper> strategy_pool;	//Gene pool of strategies/tactics.
	int board_size;

};

