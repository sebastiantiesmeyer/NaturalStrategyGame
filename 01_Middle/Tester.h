#pragma once
#include "abstract_game.h"
#include <vector>

class Tester
{
public:

	Tester(const std::vector<AbstractGame> & g) { games = g; };
	void runGames() {
		int n = games.size();
		for(int i = 0; i <n; i++){ 
			if (games[i].getPlayerScore() == glm::dvec2(0.0, 0.0)){
				games[i].Update();
				games[i].Render();
				break;
			}
		}
	}
private:
	std::vector<AbstractGame> games;
};