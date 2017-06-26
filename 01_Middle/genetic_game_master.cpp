#include "genetic_game_master.h"
#include "genetic_strategy.h"
#include "genetic_tactics.h"
#include "official_game.h"
#include <random>
#include <functional>
#include <iostream>

void GeneticGameMaster::initiate_players(int player_count)
{
	strategy_pool.assign(player_count, strategy_wrapper());
}


void GeneticGameMaster::addGames(Updater &games)
{
	for(int s1 = 0; s1 < strategy_pool.size(); s1++)
	{	//let each player compete against..
		for(int s2 = 0; s2 < s1; s2++)
		{
			games.AddTask( /*LAMBDA START*/ [this, s1, s2](int iterations)->bool
			{
				//static std::shared_ptr<OfficialGame> game = nullptr;
				static OfficialGame * game = nullptr;
				//INIT
				if(iterations == 0)
				{
					std::shared_ptr<AbstractPlayer> p1 = std::make_shared<SuperPlayer>(
													std::static_pointer_cast<AbstractStrategy>(strategy_pool[s1].gs),
													std::static_pointer_cast<AbstractTactic  >(strategy_pool[s1].gt));
					std::shared_ptr<AbstractPlayer> p2 = std::make_shared<SuperPlayer>(
													std::static_pointer_cast<AbstractStrategy>(strategy_pool[s2].gs),
													std::static_pointer_cast<AbstractTactic  >(strategy_pool[s2].gt));
					//game.reset();
					//std::shared_ptr<AbstractGame> tmp = std::make_shared<OfficialGame>(p1, p2, board_size);
					//game = tmp;
					//tmp.reset();
					game = new OfficialGame(p1, p2, board_size);
				}

				//UPDATE
				strategy_pool[s1].gs->activate();
				strategy_pool[s2].gs->activate();
				game->Update();
				game->Render();
				glm::dvec2 score = game->getPlayerScore();

				//CLEAN
				if(score != glm::dvec2(0))
				{
					//delete game;			   //update player fitness:

					delete game;

					std::cout << "deleted" << std::endl;

					strategy_pool[s1].fitness += (score[0] - score[1]);
					strategy_pool[s2].fitness += (score[1] - score[0]);
				}
				return score != glm::dvec2(0);
			} /*LAMBDA END*/ );
		}
	}
	//sort your strategies according to fitness:
	games.AddTask( /*LAMBDA START*/ [this](int iterations)->bool
	{
		std::sort(strategy_pool.begin(), strategy_pool.end(), std::greater<strategy_wrapper>()); //first is the best

		const float replace_ratio = 0.3; //The worst performing 2/3 goes extinct :oC
		const int initial_pop = strategy_pool.size();
		for(int i = 0; i < initial_pop * replace_ratio; i++)
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

			int n = (int)(std::rand()%strategy_pool.size());

			sw.gs->cross_over(strategy_pool[n].gs->weights, 0.2);
			sw.gt->cross_over(strategy_pool[n].gt->weights0, strategy_pool[n].gt -> weights1, 0.2);
		}
		return true;
	} /*LAMBDA END*/ );
}
