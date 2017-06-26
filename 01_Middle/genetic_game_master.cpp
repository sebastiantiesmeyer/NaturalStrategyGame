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

void GeneticGameMaster::addSimpleGames(Updater &games)
{
	for(int s = 0; s < strategy_pool.size(); s++)
	{
		games.AddTask( /*LAMBDA START*/ [this, s](int iterations)->bool
		{
			//static std::shared_ptr<OfficialGame> game = nullptr;
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
			game->Render();

			//CLEAN
			if(score != glm::dvec2(0))
			{
				//delete game;			   //update player fitness:

				float score1 = (score[0] - score[1])*10.f + game->get_secondary_score(0);
				if(score[0] == 0 && score[1] == 1) score1 += game->GetProgress();
				if(score[0] == 1 && score[1] == 0) score1 -= game->GetProgress();
				strategy_pool[s].fitness += score1;

				std::cout << "P" << s << " score: " << score1  << " fitness: " << strategy_pool[s].fitness << std::endl;
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
			//static std::shared_ptr<OfficialGame> game = nullptr;
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
			game->Render();

			//CLEAN
			if(score != glm::dvec2(0))
			{
				//delete game;			   //update player fitness:

				float score1 = (score[1] - score[0])*10.0f + game->get_secondary_score(1);
				if(score[0] == 1 && score[1] == 0) score1 += game->GetProgress();
				if(score[0] == 0 && score[1] == 1) score1 -= game->GetProgress();
				strategy_pool[s].fitness += score1;

				std::cout << "P" << s << " score: " << score1 << " fitness: " << strategy_pool[s].fitness << std::endl;
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
				for(int i = 0; iterations % skip == 0 && i < speedup && score == glm::dvec2(0); ++i)
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

					float score0 = (score[1] - score[0]) + game->get_secondary_score(0);
					float score1 = (score[0] - score[1]) + game->get_secondary_score(1);
					if(score[0] == 1 && score[1] == 0)
					{
						score0 -= game->GetProgress();
						score1 += game->GetProgress();
					}
					if(score[0] == 0 && score[1] == 1)
					{
						score0 -= game->GetProgress();
						score1 += game->GetProgress();
					}
					strategy_pool[s1].fitness += score0;
					strategy_pool[s2].fitness += score1;

					std::cout << "P" << s1 << " score: " << score0 <<
						 "   VS   P" << s2 << " score: " << score1 << std::endl;
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
		for(strategy_wrapper &sw : strategy_pool)
		{
			std::cout << "Player " << i << " fitness: " << sw.fitness << std::endl;
			++i;
			sw.gs->mutate(0.1f);
			sw.gt->mutate(0.1f);

			int n = (int)(std::rand() % strategy_pool.size());

			sw.gs->cross_over(strategy_pool[n].gs->weights, 0.05f);
			sw.gt->cross_over(strategy_pool[n].gt->weights0, strategy_pool[n].gt->weights1, 0.05f);
			sw.fitness = 0;
		}

		return true;
	} /*LAMBDA END*/ );

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

matrix GeneticGameMaster::load_matrix(const matrix &m, const std::string &filename) {

	std::ifstream infile;
	infile.open(filename);

	for (strang s : m) {
		for (float f : s) {
			infile >> f;
		}
	}

	return m;
}

