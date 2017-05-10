#pragma once
#include <vector>
#include <map>
#include <array>
#include <glm\glm.hpp>

const int game_size = 20;

enum UNIT_TYPE
{
	ROCK = 0, SCISSOR = 1, PAPER = 2,
};

/*enum CELL_TYPE
{
	NORMAL, BASE, OUTPOST
};*/

struct Cell
{
	int id = 0; //empty
};

typedef glm::ivec2 Dir;
typedef glm::ivec2 Position;

struct Unit
{
	int id;
	int player; //0 or 1
	Position pos;
	UNIT_TYPE type;
	bool moved = false;
};


/*enum DIRECTION
{
	NONE = 0,
	NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4,
	UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4,
};*/


struct Command
{
	int id;
	Dir dir = Dir(0,0);
};

struct UnitProgress
{
	std::array<int,3> progress;
	int total_time;
	int our_base_captured = 0;
	int enemy_base_captured = 0;
};

typedef std::map<int, Unit> Units;
//typedef std::vector<Cell> Board;

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

Position auto_rotate(Position dir, int i) //i = player
{
	return (i == 0 ? dir : Position(game_size-1) - dir);
}
