#include "genetic_game_master.h"
#include "genetic_strategy.h"
#include "genetic_tactics.h"
#include "official_game.h"
#include <random>
#include <functional>



void GeneticGameMaster::play(int n_games)
{
	for(int i = 0; i < n_games; i++)
	{					//play multiple training games in a row
		for(int s1 = 0; s1 < strategy_pool.size(); s1++)
		{	//let each player compete against..
			for(int s2 = 0; s2 < s1; s2++)
			{					//each opponent.

//Create the two players for this match:

				std::shared_ptr<AbstractPlayer> p1 = std::make_shared<SuperPlayer>(std::static_pointer_cast<AbstractStrategy>(strategy_pool[s1].gs), std::static_pointer_cast<AbstractTactic>(strategy_pool[s1].gt));
				std::shared_ptr<AbstractPlayer> p2 = std::make_shared<SuperPlayer>(std::static_pointer_cast<AbstractStrategy>(strategy_pool[s2].gs), std::static_pointer_cast<AbstractTactic>(strategy_pool[s2].gt));

				//To be updated according to the IO of OfficialGame()
				std::shared_ptr<AbstractGame> game = std::make_shared<OfficialGame>(p1, p2, board_size);
				//game.start();
				while(game->getPlayerScore() == glm::dvec2(0))
				{
					game->Update();
				}
				//:TODO: run the game...

				//get scores in the end:
				glm::dvec2 score = game->getPlayerScore();

				//update player fitness:
				strategy_pool[s1].fitness += (score[0] - score[1]);
				strategy_pool[s2].fitness += (score[1] - score[0]);
			}
		}

		//sort your strategies according to fitness:
		std::sort(strategy_pool.begin(), strategy_pool.end(), std::not2(std::less<strategy_wrapper>())); //first is the best

		//The worst performing 2/3 goes extinct :oC
		const float replace_ratio = 0.3;
		const int initial_pop = strategy_pool.size();
		for(int i = 0; i <  initial_pop * replace_ratio; i++)
		{
			strategy_pool.pop_back();
		}

		// ... and is replaced by copies of the best performing third 
		for(int i = 0; i < initial_pop * replace_ratio; i++)
		{
			strategy_pool.push_back(strategy_pool[i]);
		}

		//Some mutations & cross-overs ( -> love )
		for(strategy_wrapper sw : strategy_pool)
		{
			sw.gs->mutate(0.2);
			sw.gt->mutate(0.2);

			int n = (int)(std::rand()*strategy_pool.size());

			sw.gs->cross_over(strategy_pool[n].gs->weights, 0.2);
			sw.gt->cross_over(strategy_pool[n].gt->weights, 0.2);
		}
	}
}

void GeneticGameMaster::initiate_players(int player_count)
{
	strategy_pool.assign(player_count, strategy_wrapper());
}

void GeneticGameMaster::save_players()
{
}
