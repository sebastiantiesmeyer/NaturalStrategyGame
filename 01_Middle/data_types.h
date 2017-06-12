#pragma once
#include <vector>
#include <map>
#include <array>
#include <glm/glm.hpp>

//const int game_size = 5;

enum UNIT_TYPE{	ROCK = 0, SCISSOR = 1, PAPER = 2,};

typedef glm::ivec2 Position;	//Board is indexed with Positions
typedef glm::ivec2 Dir;			//The same type, but for indicating directions

struct Options; //forward declaration

struct Command //AbstractGame executes the Command(s) on Unit(s)
{
	int id;
	Dir dir = Dir(0,0);		// x,y offsets
};

struct Unit
{
	int id; //positive for player 0, and negative for player 1!!!
	int player; //0 or 1
	Position pos; //position on the board
	UNIT_TYPE type; //Rock, paper, or scizzor
	bool moved = false; //when a unit already moved in a turn it is true, and cannot move again
	//For debugging purpuses:
	mutable Options * options = nullptr; //so we are allowed to change it
	mutable Command * command = nullptr;
	mutable glm::vec2 movementvec = glm::vec2(0);
	mutable int numberofenemys = 0;
};

struct UnitProgress // Training progress, AND base capture data are stored here.
{
	std::array<int, 3> progress = {9,9,9};
	int total_time = 10;
	int current_train_time = 0;
	int our_base_captured = 0;	 //TODO: rethink, move to board
	int enemy_base_captured = 0; //TODO: rethink, move to board
};

typedef std::map<int, Unit> Units; //Contains Units with their id-s.

struct Cell //One element of a board
{
	int id = 0; //empty // we may not even need this, just the pointer
	Unit* unit = nullptr; // might be a lot faster
};

//Helper function that automaticly rotates position for the player's point of view
//TODO: move into Board
inline Position auto_rotate(const Position &dir, int player, int board_size)
{
	return (player == 0 ? dir : Position(board_size - 1) - dir);
}

// The game board containing information for every cell in it.
//	It is a 2d array. Use the () operator to acess its elements. Usage: board(pos, player)
// TODO: another file.
struct Board
{
	//Create board of a given size.
	//ATTENTION! by default it is zero by zero board!
	Board(int board_size = 0)
		: board (std::vector<Cell>(board_size*board_size)), board_size(board_size)
	{}
	inline int size() const { return board_size; }

	//Resizes board to newsize. The newsize is the side-length of the board.
	//Cells are not deleted! Use with care!
	inline void resize(int newsize)
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

// Commands after one another, and the UNIT_TYPE that we train next turn
// Players have to return one of these at the end of every turn
struct CommandQueue 
{
	std::vector<Command> unitcmds;
	UNIT_TYPE train;
};

//Depricted.
enum GAME_STATUS{	ONGOING = 0, DRAW = -1, PLAYER1WON = 1, PLAYER2WON = 2};

// clamp values, needs less operator ( < ).
template<typename T> T clamp(const T &x, const T &m, const T &M)
{ return (x < m ? m : (M < x ? M : x)); }

//first-norm implementation for integer valued 2D vectors
inline int norm1(const glm::ivec2 &v) { return abs(v.x) + abs(v.y); }

// return value 0 = both die, 1 =  we die, 2 = we win}
inline int fight_output(const Unit &we, const Unit &enemy)
{	return (we.type - enemy.type + 3) % 3;	}

//Order is given to local strategy (tactics) as target, or guide where to go/be.
struct Order
{
	Position target = glm::ivec2(0, 0);	// The unit has to move towards this RELATIVE position
	float sacrifice = 0.5;	// Any value between 0 and 1, examples:
							//  0.0 means it will run from the same kind
							//  0.5 means it will not run, but will not attack either
							//  1.0 means it will attack its kind
	std::vector<int> instruction = { 0, 0, 0, 0, 0, 0 };
};

typedef std::vector<Order> OrderList; // First order is the top-priority order
									  //maps Unit ptr to list of orders
									  //ATTENTION! Dont try to access units thorugh dereferencing key values! Units may die.
typedef std::map<const Unit*, OrderList> AllOrders;

//AllOrders = map<Unit*, OrderList> it maps a unit to its order list
//OrdeList = vector<Order> the priority list of orders, usually just one
//Order = struct{Position, float sacrafice} subject to change!

//More simple constructors for Player classes
/*struct PlayerParameters
{
	Board &const board;
	Units &const units;
	UnitProgress &const unit_progress;
	int ind;
};*/
