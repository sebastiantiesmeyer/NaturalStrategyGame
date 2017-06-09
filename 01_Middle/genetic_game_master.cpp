#include "genetic_game_master.h"
#include "genetic_strategy.h"
#include "genetic_tactics.h"

GeneticGameMaster::GeneticGameMaster()
{
}


void GeneticGameMaster::play(int n_games)
{
	for (int i = 0; i < n_games; i++) {
		for (int s1 = 0; s1 < i; s1++) {
			for (int s2 = 0; s2 < s1; s2++) {
				Board board;
				Units * units;
				UnitProgress unit_progress[2];
				SupPlayer * p1 = new SupPlayer({ board, units, unit_progress[0], 0 }, strategy_pool[s1].gs, strategy_pool[s1].gt);
				SupPlayer * p2 = new SupPlayer({ board, units, unit_progress[1], 1 }, strategy_pool[s2].gs, strategy_pool[s2].gt);

			}
		}
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
