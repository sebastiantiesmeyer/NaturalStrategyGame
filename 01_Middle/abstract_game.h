#pragma once
#include "data_types.h"
#include "abstract_player.h"
#include <random>
#include <memory>
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
	AbstractGame(const std::shared_ptr<AbstractPlayer> &, const std::shared_ptr<AbstractPlayer> &, int board_size);
	//This has to be called every frame for the game to progress
	//	 Returns true when game ends
	bool Update();
	//Draws stuff and interacts with player. Calls other draw funtions
	void Render();
	//Returns score of the players. Player 0 is x, and player 1 is y.
	//	Inharited class has to implement it
	virtual glm::dvec2 getPlayerScore() const = 0;
protected:
	//Implement it for rules that are not implemented in the AbstractGame();
	virtual void extra_rules() = 0; //eg. train units, check winning, and outposts

	//Use this function to create a unit easily. Optional parameter to set extra parameters of a Unit. Return true on success.
	bool create_unit(const Position &rel_pos, int player, UNIT_TYPE type, Unit *extra = nullptr);

private:	//following functions are used internally
	void simulate_board();
	void execute_command(const Command &command, int player);
	void kill_unit(Unit &unit);
	void move_unit(Unit &unit, const Position &newpos);
protected:
	glm::dvec2 score = glm::dvec2(0.0, 0.0); //This is the game status. Modify to end game!!
	UnitProgress unit_progress[2]; //change it as needed!

	Units units; //CHANGE THROUGH create_unit() only!!
	Board board; //DON'T CHANGE 

	int cycle = 0; //DON'T CHANGE the turn current turn or cycle number:
	const int max_cycles = 5000; //game ends after max_cycles turnes/cycles.
	bool player_0_done = false; //DON'T CHANGE 
	bool player_1_done = false; //DON'T CHANGE
	std::shared_ptr<const CommandQueue> queue0;
	std::shared_ptr<const CommandQueue> queue1;
	//const CommandQueue *queue0 = nullptr;
	//const CommandQueue *queue1 = nullptr;

private:
	std::default_random_engine rnd_engine; //Random generator
	std::uniform_int_distribution<int> rnd_distribution; //Usage: "rnd_distribution(rnd_engine)" returns a random integer
	std::shared_ptr<AbstractPlayer> player[2]; //pointers to players
	int largest_id[2] = { 1,-1 };
};