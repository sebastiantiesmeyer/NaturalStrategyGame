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
#include "genetic_strategy.h"
#include "genetic_game_master.h"
#include <memory>

class Tester
{
public:
	Tester()
	{
		//AbstractStrategy * strategy = new CyborgStrategy();
		//std::shared_ptr<AbstractStrategy> strategy = std::make_shared<GeneticStrategy>(20, 3);
		std::shared_ptr<AbstractStrategy> strategy0 = std::make_shared<CyborgStrategy>();
		std::shared_ptr<AbstractStrategy> strategy1 = std::make_shared<CyborgStrategy>();
		//AbstractTactic * tactic = new ProbabilisticTactic(); //clean it up
		//std::shared_ptr<AbstractTactic> tactic = std::make_shared<GeneticTactics>(20, 5);
		std::shared_ptr<AbstractTactic> tactic = std::make_shared<ProbabilisticTactic>();

		players.push_back(std::make_shared<SuperPlayer>(strategy0, tactic));
		players.push_back(std::make_shared<SimplePlayer>());
		//players.push_back(std::make_shared<HumanPlayer>());
		//players.push_back(std::make_shared<SimplePlayer>());

		games.push_back(std::make_shared<OfficialGame>(players[0], players[1], 10));
		//games.push_back(std::make_shared<OfficialGame>(players[2], players[3], 8));

	};
	void runGames() {
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