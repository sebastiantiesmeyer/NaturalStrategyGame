#include "reinforcement_game_master.h"

QGameMaster::QGameMaster(int board_size, int player_count)
{
}

void QGameMaster::addSimpleGames(Updater &games)
{
	/*for (int s = 0; s < strategy_pool.size(); s++)
	{
		games.AddTask( /*LAMBDA START*//* [this, s](int iterations)->bool
		{
			static OfficialGame * game = nullptr;
			//INIT
			if (iterations == 0)
			{
				std::shared_ptr<AbstractPlayer> p = std::make_shared<SuperPlayer>(
					std::static_pointer_cast<AbstractStrategy>(strategy_pool[s].gs),
					std::static_pointer_cast<AbstractTactic  >(strategy_pool[s].gt));
				game = new OfficialGame(p, train_h1, board_size);
			}
			glm::dvec2 score = game->getPlayerScore();
			//UPDATE
			for (int i = 0; iterations % skip == 0 && i < speedup && score == glm::dvec2(0); ++i)
			{
				strategy_pool[s].gs->activate();
				game->Update();
				score = game->getPlayerScore();
			}
			if (ImGui::Begin("Game Master")) ImGui::TextUnformatted("Aganist heuristics");
			ImGui::End();
			game->Render();

			//CLEAN
			if (score != glm::dvec2(0))
			{
				float score1 = calculate_player_fittnes(score, 0, game->get_secondary_score(0), game->GetProgress());
				strategy_pool[s].fitness += score1;

				//std::cout << "P" << s << " score: " << score1  << " fitness: " << strategy_pool[s].fitness << std::endl;
				delete game;
			}
			return score != glm::dvec2(0);
		} /*LAMBDA END*//*);
	} */
}
