#pragma once
#include "data_types.h"
#include "abstract_player.h"
#include <random>

//Abstract Game class (for two players)
//Handles the following game dynamics:
// Board: a n times n grid of Cells that are empty, or has a unit on it
// Units: a set (map) of units containing units from both players: Rock, Scizzor, Paper.
// Command queue: each player returns a command queue of the commands issued to the units
//	 commands are executed one-by-one, randomly choosing each time the next comamnd form a player
// Command execution: only executes valid commands. Simulates Rock-Paper-Scizzor dynamics
//DOES NOT HANDLE: unit creation, checking winning, outposts dynamics
// Render renders stuff, Update proceeds with computation.
class AbstractGame
{
public:
	bool Update(); //TODO
	void Render() const; //TODO
	AbstractGame(AbstractPlayer*, AbstractPlayer*, int board_size);
	virtual glm::dvec2 getPlayerScore() const = 0;
protected:
	virtual void extra_rules() = 0; //eg. train units, check winning, and outposts
	void simulate_board();
	void execute_command(const Command &command, int player);
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