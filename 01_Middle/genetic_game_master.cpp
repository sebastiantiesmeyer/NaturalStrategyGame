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
		for(int s2 = 0; s2 < strategy_pool.size(); s2++)
		{
			if(s1 == s2) continue;
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
					game = new OfficialGame(p1, p2, board_size);
				}
				glm::dvec2 score = game->getPlayerScore();
				//UPDATE
				for(int i = 0; i < speedup && score == glm::dvec2(0); ++i)
				{
					strategy_pool[s1].gs->activate();
					strategy_pool[s2].gs->activate();
					game->Update();
					score = game->getPlayerScore();
				}
				game->Render();

				//CLEAN
				if(score != glm::dvec2(0))
				{
					//delete game;			   //update player fitness:

					float score1 = (score[1] - score[0]) + game->get_secondary_score(0);
					float score2 = (score[0] - score[1]) + game->get_secondary_score(1);
					strategy_pool[s1].fitness += score1;
					strategy_pool[s2].fitness += score2;

					std::cout << "P" << s1 << " score: " << score1 <<
						 "   VS   P" << s2 << " score: " << score2 << std::endl;
					delete game;
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
		int i = 0;
		//Some mutations & cross-overs ( -> love <3 )
		for(strategy_wrapper sw : strategy_pool)
		{
			std::cout << "Player " << i << " fitness: " << sw.fitness << std::endl;
			++i;
			sw.gs->mutate(0.4f);
			sw.gt->mutate(0.4f);

			int n = (int)(std::rand() % strategy_pool.size());

			sw.gs->cross_over(strategy_pool[n].gs->weights, 0.05f);
			sw.gt->cross_over(strategy_pool[n].gt->weights0, strategy_pool[n].gt -> weights1, 0.05f);
			sw.fitness = 0;
		}
		return true;
	} /*LAMBDA END*/ );
}
