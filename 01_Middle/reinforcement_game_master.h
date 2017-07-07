#pragma once
#include "abstract_game.h"
#include "super_player.h"
#include "reinforcement_tactics.h"
#include "genetic_strategy.h"
#include <random>
#include "Updater.h"

//static std::default_random_engine rnd_engine;


class QGameMaster
{
public:
	int speedup = 502;
	int skip = 1;

	QGameMaster(int board_size, int player_count);
	void initiate_players(int player_count);

	void addSimpleGames(Updater & games);

private:
	std::shared_ptr<AbstractPlayer> train_h1;
	int board_size;

};


