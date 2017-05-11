#pragma once
#include <vector>
#include <map>
#include <array>
#include <glm\glm.hpp>

const int game_size = 20;

enum UNIT_TYPE{	ROCK = 0, SCISSOR = 1, PAPER = 2,};

typedef glm::ivec2 Position; //Board is indexed with Positions
typedef glm::ivec2 Dir;

struct Unit
{
	int id;
	int player; //0 or 1
	Position pos;
	UNIT_TYPE type;
	bool moved = false;
};

struct Command
{
	int id;
	Dir dir = Dir(0,0);		// x,y offsets
};

struct UnitProgress
{
	std::array<int,3> progress;
	int total_time;
	int our_base_captured = 0;
	int enemy_base_captured = 0;
};

typedef std::map<int, Unit> Units;

struct Cell
{
	int id = 0; //empty // we may not even need this, just the pointer
	Unit* unit = nullptr; // might be a lot faster
};

struct Board
{
	std::vector<Cell> board = std::vector<Cell>(game_size*game_size);

	Cell& operator[] (const Position &pos)
	{
		return board[pos.y*game_size + pos.x];
	}
	const Cell& operator[] (const Position &pos) const
	{
		return board[pos.y*game_size + pos.x];
	}
};

struct CommandQueue
{
	std::vector<Command> unitcmds;
	UNIT_TYPE train;
};

inline Position auto_rotate(const Position &dir, int player)
{
	return (player == 0 ? dir : Position(game_size-1) - dir);
}

enum GAME_STATUS{	ONGOING = 0, DRAW = -1, PLAYER1WON = 1, PLAYER2WON = 2};


/*enum DIRECTION
{
	NONE = 0,
	NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4,
	UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4,
};*/

/*enum CELL_TYPE
{
	NORMAL, BASE, OUTPOST
};*/