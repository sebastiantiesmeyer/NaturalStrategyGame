#pragma once
#include "silent_genetic_strategy.h"
#include "abstract_game.h"
#include "super_player.h"
#include "genetic_tactics.h"
#include <random>
#include "Updater.h"

class SilentGameMaster{
public:
	struct silent_strategy_wrapper
	{
		float fitness = 0;

		/*int n_input_strategy = 12;
		int n_output_strategy = 7;

		int n_input_tactics = 12;
		int n_output_tactics = 5;
		float scope = 3.f;*/

		std::shared_ptr<GeneticTactics> gt = std::make_shared<GeneticTactics>();

		std::shared_ptr<SilentGeneticStrategy> gs = std::make_shared<SilentGeneticStrategy>();

		bool operator <(const silent_strategy_wrapper& p) const
		{	//const after the function means that it does not change the class.
			//we need this, so std::less can work with it
			return fitness > p.fitness;
		};
		silent_strategy_wrapper() = default;
		silent_strategy_wrapper(const silent_strategy_wrapper &other) : fitness(other.fitness),
			gt(std::make_shared<GeneticTactics>(*other.gt)), gs(std::make_shared<SilentGeneticStrategy>(*other.gs))
		{}
		//const strategy_wrapper & operator=(const strategy_wrapper& other) = default;
	};

public:
	int speedup = 502;
	int skip = 1;

	SilentGameMaster(int board_size, int player_count);

	//void play(int games);
	void initiate_players(int player_count);
	//void save_players();
	//std::shared_ptr<AbstractGame> game = nullptr;

	void addGames(Updater &games);
	void addSort(Updater & games);
	void addCyborgWithBest(Updater & games);
	void addSimpleGames(Updater & games);
	std::vector<silent_strategy_wrapper> strategy_pool;	//Gene pool of strategies/tactics.
	void addSimpleGames2(Updater & games);

protected:
	std::shared_ptr<AbstractPlayer> train_h1;

	int board_size;

public:
	const silent_strategy_wrapper& get_winner();
	float calculate_player_fittnes(const glm::dvec2 & score, int player, float secondary_score, float progress);
	static void save_matrix(const matrix &m, const std::string &name);
	static matrix load_matrix(matrix &m, const std::string &filename);
};
