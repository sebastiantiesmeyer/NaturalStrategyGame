#pragma once
#include "abstract_game.h"
#include "super_player.h"
#include "genetic_tactics.h"
#include "genetic_strategy.h"
#include <random>
#include "Updater.h"
#include "official_game.h"

/*struct strategy_wrapper
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

};*/


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

	struct Parameters
	{
		strategy_wrapper *ps1;
		strategy_wrapper *ps2;
		int board_size;
	};

	GeneticGameMaster(int board_size, int player_count) : board_size(board_size) {
		strategy_pool.assign(player_count, strategy_wrapper());
	}
	//Constructor inheritance? TODO: implement

	//void play(int games);
	void initiate_players(int player_count);
	//void save_players();

	std::vector<Parameters> params;
	Updater games;

	void createGames(int n_games);
	/*{
		for (int i = 0; i < n_games; i++)
		{					//play multiple training games in a row
			for (int s1 = 0; s1 < strategy_pool.size(); s1++)
			{	//let each player compete against..
				for (int s2 = 0; s2 < s1; s2++)
				{
					params.push_back({ &strategy_pool[s1], &strategy_pool[s2], board_size }); //PARAMETERS!

					games.AddTask( //start of lambda
						[](int iterations, void * in_params)->bool
					{
						Parameters * params = static_cast<Parameters*>(in_params);
						static OfficialGame *game = nullptr;
						if (iterations == 0) //INIT
						{
							std::shared_ptr<AbstractPlayer> p1 = std::make_shared<SuperPlayer>(std::static_pointer_cast<AbstractStrategy>(params->ps1->gs),
								std::static_pointer_cast<AbstractTactic>(params->ps1->gt));
							std::shared_ptr<AbstractPlayer> p2 = std::make_shared<SuperPlayer>(std::static_pointer_cast<AbstractStrategy>(params->ps2->gs),
								std::static_pointer_cast<AbstractTactic>(params->ps2->gt));
							game = new OfficialGame(p1, p2, params->board_size);
						}

						//UPDATE
						params->ps1->gs->activate();
						params->ps2->gs->activate();
						game->Update();
						game->Render();

						glm::dvec2 score = game->getPlayerScore();
						if (score != glm::dvec2(0)) //CLEANUP
						{
							delete game;
							//update player fitness:
							params->ps1->fitness += (score[0] - score[1]);
							params->ps2->fitness += (score[1] - score[0]);
							return true;
						}
						else return false;
					} //end of lambda
					, static_cast<void*>(&params.back()));

				}
			}
			//sort your strategies according to fitness:
			games.AddTask(
				[](int iterations, void * in_params)->bool //LAMBDA START
			{
				std::vector<strategy_wrapper> &strategy_pool = *static_cast<std::vector<strategy_wrapper>*>(in_params);

				std::sort(strategy_pool.begin(), strategy_pool.end(), std::not2(std::less<strategy_wrapper>())); //first is the best

																												 //The worst performing 2/3 goes extinct :oC
				const float replace_ratio = 0.3;
				const int initial_pop = strategy_pool.size();
				for (int i = 0; i < initial_pop * replace_ratio; i++)
				{
					strategy_pool.pop_back();
				}

				// ... and is replaced by copies of the best performing third 
				for (int i = 0; i < initial_pop * replace_ratio; i++)
				{
					strategy_pool.push_back(strategy_pool[i]);
				}

				//Some mutations & cross-overs ( -> love )
				for (strategy_wrapper sw : strategy_pool)
				{
					sw.gs->mutate(0.2);
					sw.gt->mutate(0.2);

					int n = (int)(std::rand()*strategy_pool.size());

					sw.gs->cross_over(strategy_pool[n].gs->weights, 0.2);
					sw.gt->cross_over(strategy_pool[n].gt->weights, 0.2);
				}
				return true;
			} //LAMBDA END
			, static_cast<void*>(&strategy_pool));
		}
	}*/

private:

	std::vector<strategy_wrapper> strategy_pool;	//Gene pool of strategies/tactics.
	int board_size;

};

