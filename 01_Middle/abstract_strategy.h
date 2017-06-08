#pragma once
#include "data_types.h"
#include "sup_player.h"

//Abstract strategy class, it has two purely virtual functions.
// changeOrders: It generates an priority OrderList for every unit
// containing the target location where it the unit has to move (like a general)
// train: Decides what units should be trained in the current cycle
class AbstractStrategy
{
public:
	// The parameters for the strategy should be set via this function
	// ATTENTION! if you use the same instance for both of the players, you
	//			  have to call this function all the time
	// THEREFORE! create different instance for each player!!
	void setParams(const CommandQueue &cqueue, const PlayerParameters &params)
	{
		queue = cqueue;			ind = params.ind;
		unit_progress = params.unit_progress;
		board = params.board;	units = params.units;
	}
	//implement the following to define a global strategy
	virtual void changeOrders(AllOrders &orders) = 0; // the main function
						//AllOrders = map<Unit*, OrderList> it maps a unit to its order list
						//OrdeList = vector<Order> the priority list of orders, usually just one
						//Order = struct{Unit*, Position, float sacrafice} subject to change!
	virtual UNIT_TYPE train() = 0; // sets what to train next, called AFTER the orders are changed
	//for implementing an interface redefine the following
	virtual void Render() {}			   //this is called even after the turn is ended
	virtual bool Update() { return true; } //return true when the turn is ended
protected:
	CommandQueue & const queue;
	UnitProgress & const unit_progress;
	Board & const board;
	Units & const units;
	int ind = 0; //player index
};
