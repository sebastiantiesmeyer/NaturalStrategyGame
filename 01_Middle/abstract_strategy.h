#pragma once
#include "data_types.h"
#include "super_player.h"

//Abstract strategy class, it has two purely virtual functions.
// changeOrders: It generates an priority OrderList for every unit
// containing the target location where it the unit has to move (like a general)
// train: Decides what units should be trained in the current cycle
class AbstractStrategy
{
public:
	AbstractStrategy() = default;
	// The parameters for the strategy should be set via this function
	// ATTENTION! if you use the same instance for both of the players, you
	//			  have to call this function all the time
	// THEREFORE! create different instance for each player!!
	void setParams(const CommandQueue &cqueue, const PlayerParameters &params)
	{
		queue = cqueue;			player = params.ind;
		unit_progress = params.unit_progress;
		board = params.board;	units = params.units;
	}
	
	// The main function you have to implement, gives orders to units
	//		AllOrders = map<Unit*, OrderList> it maps a unit to its order list
	//		OrdeList = vector<Order> the priority list of orders, usually just one
	//		Order = struct{Unit*, Position, float sacrafice} subject to change!
	virtual void changeOrders(AllOrders &) = 0;

	// Sets what to train next, called AFTER the orders are changed
	virtual UNIT_TYPE train() = 0;

	//Redefine for implementing an interface. This is called even after the turn is ended
	virtual void Render() {}
	//Redefine for implementing an interface. Returns true when the turn is ended
	virtual bool Update() { return true; }
protected:
	CommandQueue & const queue; //this means that the reference can change but the object
	UnitProgress & const unit_progress;
	Board & const board;
	Units & const units;
	int player = 0; //player index
};
