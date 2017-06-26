#pragma once
#include "abstract_game.h"
#include "super_player.h"
#include "genetic_tactics.h"
#include "genetic_strategy.h"
#include <random>
#include "Updater.h"
#include "official_game.h"

struct strategy_wrapper
{
	float fitness = 0;

	int n_input_strategy = 12;
	int n_output_strategy = 7;

	int n_input_tactics = 12;
	int n_output_tactics = 5;
	float scope = 3.f;


	std::shared_ptr<GeneticTactics> gt = std::make_shared<GeneticTactics>();

	std::shared_ptr<GeneticStrategy> gs = std::make_shared<GeneticStrategy>();

	bool operator <(const strategy_wrapper& p) const
	{	//const after the function means that it does not change the class.
		//we need this, so std::less can work with it
		return fitness < p.fitness;
	};
	bool operator >(const strategy_wrapper& p) const
	{	//const after the function means that it does not change the class.
		//we need this, so std::less can work with it
		return fitness > p.fitness;
	};
};

//static std::default_random_engine rnd_engine;


class GeneticGameMaster
{
public:

	GeneticGameMaster(int board_size, int player_count) : board_size(board_size), strategy_pool(player_count)
	{}

	//void play(int games);
	void initiate_players(int player_count);
	//void save_players();
	//std::shared_ptr<AbstractGame> game = nullptr;

	void addGames(Updater &games);

private:

	std::vector<strategy_wrapper> strategy_pool;	//Gene pool of strategies/tactics.
	int board_size;

};

