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

class Tester
{
public:

	Tester()
	{
		AbstractStrategy * strategy = new CyborgStrategy();
		AbstractTactic * tactic = new ProbabilisticTactic();
		players.push_back((AbstractPlayer*)(new SuperPlayer(strategy, tactic)));
		players.push_back((AbstractPlayer*)(new SimplePlayer()));
		players.push_back((AbstractPlayer*)(new  HumanPlayer()));
		players.push_back((AbstractPlayer*)(new  HumanPlayer()));

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
	std::vector<AbstractPlayer*> players;
	std::vector<AbstractGame*> games;
};