#include "genetic_game_master.h"
#include "genetic_strategy.h"
#include "genetic_tactics.h"
#include "official_game.h"
#include "simple_player.h"
#include <random>
#include <iostream>
#include <fstream>

GeneticGameMaster::GeneticGameMaster(int board_size, int player_count)
	: board_size(board_size), strategy_pool(player_count), train_h1(std::make_shared<SimplePlayer>())
{}

void GeneticGameMaster::initiate_players(int player_count)
{
	strategy_pool = std::vector<strategy_wrapper>(player_count);
}

const GeneticGameMaster::strategy_wrapper& GeneticGameMaster::get_winner()
{
	std::sort(strategy_pool.begin(), strategy_pool.end());
	return strategy_pool.front();
}

float calculate_player_fittnes(const glm::dvec2 &score, int player, float secondary_score, float progress)
{
	//assert(secondary_score <= 0.3f && secondary_score >= 0.f);
	//assert(progress <= 1.f && progress >= 0.f);
	//assert(score.x <= 1.f && score.x >= 0.f);
	//assert(score.y <= 1.f && score.y >= 0.f);
	//assert(player == 0 || player == 1);
	float fittness = (score[player] - score[!player])*50.f + secondary_score;
	if(score[player] == 1.f && score[!player] == 0.f)
		fittness -= progress;
	if(score[!player] == 1.f && score[player] == 0.f)
		fittness += progress;
	return fittness;
}

void GeneticGameMaster::addSimpleGames(Updater &games)
{
	for(int s = 0; s < strategy_pool.size(); s++)
	{
		games.AddTask( /*LAMBDA START*/ [this, s](int iterations)->bool
		{
			static OfficialGame * game = nullptr;
			//INIT
			if(iterations == 0)
			{
				std::shared_ptr<AbstractPlayer> p = std::make_shared<SuperPlayer>(
					std::static_pointer_cast<AbstractStrategy>(strategy_pool[s].gs),
					std::static_pointer_cast<AbstractTactic  >(strategy_pool[s].gt));
				game = new OfficialGame(p, train_h1, board_size);
			}
			glm::dvec2 score = game->getPlayerScore();
			//UPDATE
			for(int i = 0; iterations % skip == 0 && i < speedup && score == glm::dvec2(0); ++i)
			{
				strategy_pool[s].gs->activate();
				game->Update();
				score = game->getPlayerScore();
			}
			if(ImGui::Begin("Game Master")) ImGui::TextUnformatted("Aganist heuristics");
			ImGui::End();
			game->Render();

			//CLEAN
			if(score != glm::dvec2(0))
			{
				float score1 = calculate_player_fittnes(score, 0, game->get_secondary_score(0), game->GetProgress());
				strategy_pool[s].fitness += score1;

				//std::cout << "P" << s << " score: " << score1  << " fitness: " << strategy_pool[s].fitness << std::endl;
				delete game;
			}
			return score != glm::dvec2(0);
		} /*LAMBDA END*/);
	}
}

void GeneticGameMaster::addSimpleGames2(Updater &games)
{
	for(int s = 0; s < strategy_pool.size(); s++)
	{
		games.AddTask( /*LAMBDA START*/ [this, s](int iterations)->bool
		{
			static OfficialGame * game = nullptr;
			//INIT
			if(iterations == 0)
			{
				std::shared_ptr<AbstractPlayer> p = std::make_shared<SuperPlayer>(
					std::static_pointer_cast<AbstractStrategy>(strategy_pool[s].gs),
					std::static_pointer_cast<AbstractTactic>(strategy_pool[s].gt));
				game = new OfficialGame(train_h1, p, board_size);
			}
			glm::dvec2 score = game->getPlayerScore();
			//UPDATE
			for(int i = 0; iterations % skip == 0 && i < speedup && score == glm::dvec2(0); ++i)
			{
				strategy_pool[s].gs->activate();
				game->Update();
				score = game->getPlayerScore();
			}
			if(ImGui::Begin("Game Master")) ImGui::TextUnformatted("Aganist heuristics");
			ImGui::End();
			game->Render();

			//CLEAN
			if(score != glm::dvec2(0))
			{
				float score1 = calculate_player_fittnes(score, 1, game->get_secondary_score(1), game->GetProgress());
				strategy_pool[s].fitness += score1;

				//std::cout << "P" << s << " score: " << score1 << " fitness: " << strategy_pool[s].fitness << std::endl;
				delete game;
			}
			return score != glm::dvec2(0);
		} /*LAMBDA END*/);
	}
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
				for(int i = 0; iterations % skip == 0 && i < speedup && score == glm::dvec2(0); ++i)
				{
					strategy_pool[s1].gs->activate();
					strategy_pool[s2].gs->activate();
					game->Update();
					score = game->getPlayerScore();
				}
				if(ImGui::Begin("Game Master")) ImGui::TextUnformatted("Aganist each other");
				ImGui::End();
				game->Render();

				//CLEAN
				if(score != glm::dvec2(0))
				{
					float score0 = calculate_player_fittnes(score, 0, game->get_secondary_score(0), game->GetProgress());
					strategy_pool[s1].fitness += score0;

					float score1 = score[1];//calculate_player_fittnes(score, 1, game->get_secondary_score(1), game->GetProgress());
					strategy_pool[s2].fitness += score1;
					std::cout << score[0] << " : " << score[1] << std::endl;
					//std::cout << "P" << s1 << " score: " << score0 << "   VS   P" << s2 << " score: " << score1 << std::endl;
					delete game;
				}
				return score != glm::dvec2(0);
			} /*LAMBDA END*/ );
		}
	}

}

//sort your strategies according to fitness:
void GeneticGameMaster::addSort(Updater &games)
{
	games.AddTask( /*LAMBDA START*/ [this](int iterations)->bool
	{
		std::sort(strategy_pool.begin(), strategy_pool.end()); //first is the best

		std::cout << "Best " << strategy_pool[0].fitness << std::endl;

		const float replace_ratio = 0.33333333333333; //The worst performing 2/3 goes extinct :oC
		const int initial_pop = strategy_pool.size();
		for(int i = 0; i < initial_pop * replace_ratio; i++)
			strategy_pool.pop_back();

		// ... and is replaced by copies of the best performing third 
		for(int i = 0; i < initial_pop * replace_ratio; i++)
			strategy_pool.push_back(strategy_pool[i]);
		//int i = 0;
		//Some mutations & cross-overs ( -> love <3 )
		for(strategy_wrapper &sw : strategy_pool)
		{
			//std::cout << "Player " << i << " fitness: " << sw.fitness << std::endl;
			//++i;
			sw.gs->mutate(0.05f);
			sw.gt->mutate(0.05f);

			int n = (int)(std::rand() % strategy_pool.size());

			sw.gs->cross_over(strategy_pool[n].gs->weights, 0.07f);
			sw.gt->cross_over(strategy_pool[n].gt->weights0, strategy_pool[n].gt->weights1, 0.07f);
			sw.fitness = 0;
		}

		return true;
	} /*LAMBDA END*/ );
}
#include "cyborg_strategy.h"
#include "probabilistic_tactic.h"

void GeneticGameMaster::addCyborgWithBest(Updater &games)
{
	std::shared_ptr<AbstractPlayer> op = std::make_shared<SuperPlayer>(
		std::static_pointer_cast<AbstractStrategy>(std::make_shared<CyborgStrategy>()),
		std::static_pointer_cast<AbstractTactic>(std::make_shared<ProbabilisticTactic>()));
	games.AddTask( /*LAMBDA START*/ [this, op](int iterations)->bool
	{
		static OfficialGame * game = nullptr;
		//INIT
		if(iterations == 0)
		{
			std::shared_ptr<AbstractPlayer> p = std::make_shared<SuperPlayer>(
				std::static_pointer_cast<AbstractStrategy>(strategy_pool[0].gs),
				std::static_pointer_cast<AbstractTactic>(strategy_pool[0].gt));
			game = new OfficialGame(p,op, board_size);
		}
		//UPDATE

		strategy_pool[0].gs->activate();
		game->Update();
		glm::dvec2 score = game->getPlayerScore();

		if(ImGui::Begin("Game Master")) ImGui::TextUnformatted("Aganist human");
		ImGui::End();

		game->Render();

		//CLEAN
		if(score != glm::dvec2(0))
		{
			float score1 = calculate_player_fittnes(score, 0, game->get_secondary_score(1), game->GetProgress());
			std::cout << "P" << 0 << " score: " << score1 << " fitness: " << strategy_pool[0].fitness << std::endl;
			delete game;
		}
		return score != glm::dvec2(0);
	} /*LAMBDA END*/);
}

void GeneticGameMaster::save_matrix(const matrix &m, const std::string &filename) {
	std::ofstream outfile;
	outfile.open(filename);
	for (strang s : m) {
		for (float f : s) {
			outfile << f << " " ;

		}
		outfile << std::endl;
	}
	outfile.close();
}

matrix GeneticGameMaster::load_matrix(matrix &m, const std::string &filename) {

	std::ifstream infile;
	infile.open(filename);

	for (strang s : m) {
		for (float f : s) {
			infile >> f;
		}
	}

	return m;
}

