#pragma once

#pragma once
#include <vector>
#include <functional>
#include "abstract_game.h"
#include "abstract_player.h"
#include "official_game.h"
#include "human_player.h"
#include "super_player.h"
#include "probabilistic_tactic.h"
#include "cyborg_strategy.h"
#include "simple_player.h"

typedef std::function<bool(int)> update_function_type; //return true when done

class Updater2
{
public:
	void AddTask(const update_function_type &func)
	{
		tasks.push_back(func);
	}

	void AddHumanVsHuman(int game_size)
	{
		std::shared_ptr<AbstractGame> game = std::make_shared<OfficialGame>(
			std::make_shared<HumanPlayer>(),
			std::make_shared<HumanPlayer>(),
			game_size);
		tasks.push_back([=](int iters)->bool
		{
			game->Update();
			game->Render();
			return game->getPlayerScore() != glm::dvec2(0);
		});
	}

	void AddCyborgVsHeuristics(int game_size)
	{
		std::shared_ptr<AbstractGame> game = std::make_shared<OfficialGame>(
			std::make_shared<SuperPlayer>(std::make_shared<CyborgStrategy>(), std::make_shared<ProbabilisticTactic>()),
			std::make_shared<SimplePlayer>(),
			game_size);
		tasks.push_back([=](int iters)->bool
		{
			game->Update();
			game->Render();
			return game->getPlayerScore() != glm::dvec2(0);
		});
	}

	void Update()
	{
		if(next_task_id < tasks.size())
		{
			bool b = tasks[next_task_id](iterations);
			if(b)
			{
				iterations = 0;
				++next_task_id;
			}
			else ++iterations;
		}
	}
private:
	std::vector<update_function_type> tasks;
	int next_task_id = 0;
	int iterations = 0;
};