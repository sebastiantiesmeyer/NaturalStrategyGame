#pragma once
#include "abstract_game.h"
#include <vector>
#include "abstract_player.h"
#include "human_player.h"
#include "official_game.h"
#include "simple_player.h"
#include "super_player.h"
#include "cyborg_strategy.h"
#include "probabilistic_tactic.h"
#include <memory>

class Tester
{
public:
	Tester()
	{
		//AbstractStrategy * strategy = new CyborgStrategy();
		std::shared_ptr<AbstractStrategy> strategy;
		//AbstractTactic * tactic = new ProbabilisticTactic(); //clean it up
		std::shared_ptr<AbstractTactic> tactic;
		players.push_back(new SuperPlayer(strategy, tactic));
		players.push_back(new SimplePlayer());
		players.push_back(new  HumanPlayer());
		players.push_back(new  HumanPlayer());

		games.push_back(new OfficialGame(players[0], players[1], 10));
		games.push_back(new OfficialGame(players[2], players[3], 8));

	};
	bool runGames() {
		int n = games.size();
		for(int i = 0; i <n; i++){ 
			if (games[i]->getPlayerScore() == glm::dvec2(0.0, 0.0)){
				games[i]->Update();
				games[i]->Render();
				break;
			}
		}
	}
private:
	std::vector<std::shared_ptr<AbstractPlayer>> players;
	std::vector<std::shared_ptr<AbstractGame>> games;
};