#pragma once
#include <vector>
#include <map>
#include <array>
#include "imgui\imgui.h"

//const int game_size = 5;

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
	int id; //positive for player 0, and negative for player 1!!!
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
	int total_time = 10;
	int current_train_time = 0;
	int our_base_captured = 0;
	int enemy_base_captured = 0; 
};

typedef std::map<int, Unit> Units;
//typedef Units::iterator Unit_iterator;
//typedef Units::const_iterator Unit_const_iterator;

struct Cell
{
	int id = 0; //empty // we may not even need this, just the pointer
	Unit* unit = nullptr; // might be a lot faster
};

inline Position auto_rotate(const Position &dir, int player, int board_size)
{
	return (player == 0 ? dir : Position(board_size - 1) - dir);
}

struct Board
{
	Board(int board_size = 0)
		: board (std::vector<Cell>(board_size*board_size)), board_size(board_size)
	{}
	inline int size() const { return board_size; }

	inline void resize(int newsize) // Are you sure??
	{
		board_size = newsize;
		board.resize(board_size*board_size);
	}

	inline Cell& operator[] (const Position &pos)
	{		return board[pos.y*board_size + pos.x];	}

	//Safe player independent access for the board
	inline const Cell& at(const Position &pos) const
	{		return board[pos.y*board_size + pos.x];	}

	//Use this for safe access of the board
	inline const Cell& operator()(const Position &pos, int player) const
	{
		Position p = auto_rotate(pos, player, board_size);
		return board[p.y*board_size + p.x];
	}
	inline Cell& operator()(const Position &pos, int player)
	{
		Position p = auto_rotate(pos, player, board_size);
		return board[p.y*board_size + p.x];
	}

	//retuns the player number at outpost i
	inline int getPlayerAtOutpost(int outpost, int player) const
	{
		return outposts[(outpost + player) % 2]; //change view
	}
	//int op1 = -1, op2 = -1;
	int outposts[2] = {-1,-1};
private:
	int board_size = 0;
	std::vector<Cell> board = std::vector<Cell>(board_size*board_size);
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

// return value 0 = both die, 1 =  we die, 2 = we win}
inline int fight_output(const Unit &we, const Unit &enemy)
{	return (we.type - enemy.type + 3) % 3;	}

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