#pragma once
#include <vector>
#include <map>
#include <array>
#include "imgui\imgui.h"

const int game_size = 5;

enum UNIT_TYPE{	ROCK = 0, SCISSOR = 1, PAPER = 2,};

typedef glm::ivec2 Position; //Board is indexed with Positions
typedef glm::ivec2 Dir;

struct Options; //forward declarations

struct Command
{
	int id;
	Dir dir = Dir(0,0);		// x,y offsets
};

struct Unit
{
	int id;
	int player; //0 or 1
	Position pos;
	UNIT_TYPE type;
	bool moved = false;
	//debugging:
	mutable Options * options = nullptr; //so we are allowed to change it
	mutable Command * command = nullptr;
	mutable glm::vec2 movementvec = glm::vec2(0);
	mutable int numberofenemys = 0;
};


struct UnitProgress
{
	std::array<int, 3> progress = {9,9,9};
	int total_time = game_size/2;
	int current_train_time = game_size/4;
	int our_base_captured = 0;
	int enemy_base_captured = 0; 
};

typedef std::map<int, Unit> Units;

struct Cell
{
	int id = 0; //empty // we may not even need this, just the pointer
	Unit* unit = nullptr; // might be a lot faster
};

inline Position auto_rotate(const Position &dir, int player)
{
	return (player == 0 ? dir : Position(game_size-1) - dir);
}

struct Board
{
	std::vector<Cell> board = std::vector<Cell>(game_size*game_size);

	inline Cell& operator[] (const Position &pos)
	{
		return board[pos.y*game_size + pos.x];
	}
	inline const Cell& at(const Position &pos) const
	{
		return board[pos.y*game_size + pos.x];
	}
	inline const Cell& operator()(const Position &pos, int player) const
	{
		Position p = auto_rotate(pos, player);
		return board[p.y*game_size + p.x];
	}
	inline Cell& operator()(const Position &pos, int player)
	{
		Position p = auto_rotate(pos, player);
		return board[p.y*game_size + p.x];
	}
	//int op1 = -1, op2 = -1;
	int outposts[2] = {-1,-1};

	inline int getPlayerAtOutpost(int outpost, int player) const
	{
		return outposts[(outpost + player) % 2]; //change view
	}
};

struct CommandQueue
{
	std::vector<Command> unitcmds;
	UNIT_TYPE train;
};

enum GAME_STATUS{	ONGOING = 0, DRAW = -1, PLAYER1WON = 1, PLAYER2WON = 2};


template<typename T> T clamp(const T &x, const T &m, const T &M)
{ return (x < m ? m : (x > M ? M : x)); }

inline int norm1(const glm::ivec2 &v) { return abs(v.x) + abs(v.y); }


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