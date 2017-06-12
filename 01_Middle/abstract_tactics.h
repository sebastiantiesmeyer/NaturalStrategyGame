#pragma once
#include "data_types.h"
#include "super_player.h"

//Abstract class for tactics (local strategy) used by SuperPlayer
// Local strategy is independent of the game situtation in large, for eg. outpost ownership
// A local strategy converts an OrderList to a Command
//	An OrderList consists of orders, the first one being the most important
//	The OrderList is issued by the a global strategy.
//	The unit may or may not move towards the goal of the order
//	This is useful for survival
class AbstractTactic
{
public:
	AbstractTactic(Board &const board) : board(board) {}
	void setBoard(Board &const nboard) { board = nboard; }
	//implement the following function to create a tactic:
	virtual Command step(const Unit &unit, const OrderList &order_list) = 0;
protected:
	Board &const board;
};