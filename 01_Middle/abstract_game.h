#pragma once
#include "data_types.h"
#include "abstract_player.h"
#include <random>

class AbstractGame
{
public:
	bool Update();
	bool Render() const;
	AbstractGame(AbstractPlayer*, AbstractPlayer*, int board_size);
	virtual glm::dvec2 getPlayerScore() const = 0;
protected:
	virtual void extra_rules() = 0; //eg. train units, check winning, and outposts
	void simulate_board();
	void execute_command(const Command &command, int i);
private:
	void kill_unit(Unit &unit);
	void move_unit(Unit &unit, const Position &newpos);
protected:
	int board_size;
	Units units; //training is not done here
	Board board;
	AbstractPlayer *player[2];
	UnitProgress unit_progress[2];

	std::default_random_engine rnd_engine;
	std::uniform_int_distribution<int> rnd_distribution;

	int cycle = 0;
	const int max_cycles = 5000;
	bool player_0_done = false;
	bool player_1_done = false;

	const CommandQueue *queue0 = nullptr;
	const CommandQueue *queue1 = nullptr;
	glm::dvec2 score = glm::dvec2(0.0, 0.0); //this is the new game status
	//GAME_STATUS game_status = ONGOING; 
};