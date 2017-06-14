#include "genetic_game_master.h"
#include "genetic_strategy.h"
#include "genetic_tactics.h"
#include "official_game.h"
#include <random>

GeneticGameMaster::GeneticGameMaster()
{
}


void GeneticGameMaster::play(int n_games)
{
	for (int i = 0; i < n_games; i++) {
		for (int s1 = 0; s1 < i; s1++) {
			for (int s2 = 0; s2 < s1; s2++) {
				Board board(5);
				Units units;
				UnitProgress unit_progress[2];
				SuperPlayer * p1 = new SuperPlayer({ board, units, unit_progress[0], 0 }, strategy_pool[s1].gs, strategy_pool[s1].gt);
				//SuperPlayer * p2 = new SuperPlayer({ board, units, unit_progress[1], 1 }, strategy_pool[s2].gs, strategy_pool[s2].gt);

				//To be updated according to the IO of OfficialGame()
				//OfficialGame * game = new OfficialGame(p1,p2);
				//game.start();

				//glm::dvec2 score = game.getPlayerScore();

				//strategy_pool[s1].fitness += (score[0]-score[1]);
				//strategy_pool[s2].fitness += (score[1]-score[0]);
			}
		}
	}
	std::sort(strategy_pool.begin,strategy_pool.end);
	for (int i = 0; i < strategy_pool.size() / 3; i++) {
		strategy_pool.pop_back();
	}
	int s = strategy_pool.size();
	for (int i = 0; i < strategy_pool.size()*2; i++) {
		strategy_pool.push_back(strategy_pool[i%s]);
	}

	for (strategy_wrapper sw : strategy_pool) {
		sw.gs -> mutate(0.2);
		sw.gt -> mutate(0.2);

		int n = (int) (std::rand()*strategy_pool.size());
		
		sw.gs->cross_over(strategy_pool[n].gs->gweights, 0.2);
	}
}

void GeneticGameMaster::initiate_players(int player_count)
{
	for (int i = 0; i < player_count; i++) {
		strategy_wrapper sw;
		strategy_pool.push_back(sw);
	}
}

void GeneticGameMaster::save_players()
{
}
