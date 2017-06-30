#pragma once
#include "abstract_game.h"
#include "super_player.h"
#include "genetic_tactics.h"
#include "genetic_strategy.h"
#include <random>
#include "Updater.h"

//static std::default_random_engine rnd_engine;


class GeneticGameMaster
{
public:
	struct strategy_wrapper
	{
		float fitness = 0;

		/*int n_input_strategy = 12;
		int n_output_strategy = 7;

		int n_input_tactics = 12;
		int n_output_tactics = 5;
		float scope = 3.f;*/

		std::shared_ptr<GeneticTactics> gt = std::make_shared<GeneticTactics>();

		std::shared_ptr<GeneticStrategy> gs = std::make_shared<GeneticStrategy>();

		bool operator <(const strategy_wrapper& p) const
		{	//const after the function means that it does not change the class.
			//we need this, so std::less can work with it
			return fitness > p.fitness;
		};
		strategy_wrapper() = default;
		strategy_wrapper(const strategy_wrapper &other) : fitness(other.fitness),
			gt(std::make_shared<GeneticTactics>(*other.gt)), gs(std::make_shared<GeneticStrategy>(*other.gs))
		{}
		//const strategy_wrapper & operator=(const strategy_wrapper& other) = default;
	};
private:
public:
	int speedup = 502;
	int skip = 1;

	GeneticGameMaster(int board_size, int player_count);

	//void play(int games);
	void initiate_players(int player_count);
	//void save_players();
	//std::shared_ptr<AbstractGame> game = nullptr;

	void addGames(Updater &games);
	void addSort(Updater & games);
	void addCyborgWithBest(Updater & games);
	void addSimpleGames(Updater & games);

	void addSimpleGames2(Updater & games);

private:
	std::shared_ptr<AbstractPlayer> train_h1;
	std::vector<strategy_wrapper> strategy_pool;	//Gene pool of strategies/tactics.
	int board_size;

public:
	const strategy_wrapper& get_winner();
	static void save_matrix(const matrix &m, const std::string &name);
	static matrix load_matrix(matrix &m, const std::string &filename);
};


