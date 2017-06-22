#pragma once
#include "data_types.h"
#include <memory>

//Abstract strategy class, it has two purely virtual functions.
// changeOrders: It generates an priority OrderList for every unit
// containing the target location where it the unit has to move (like a general)
// train: Decides what units should be trained in the current cycle
class AbstractStrategy
{
public:
	AbstractStrategy()
	//	: units(std::shared_ptr<Units>()), board(std::shared_ptr<Board>()), unit_progress(std::shared_ptr<UnitProgress>()), queue(std::shared_ptr<CommandQueue>())
	{};
	// The parameters for the strategy should be set via this function
	// ATTENTION! if you use the same instance for both of the players, you
	//			  have to call this function all the time
	// THEREFORE! create different instance for each player!!
	void setParams(const std::shared_ptr<CommandQueue> &cqueue, const std::shared_ptr<const Board> &_board,
				   const std::shared_ptr<const Units> &_units, const std::shared_ptr<const UnitProgress> &_unit_progress, int _player)
	{
		queue = cqueue;
		player = _player;
		unit_progress = _unit_progress;
		board = _board;	units = _units;
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
	std::shared_ptr<CommandQueue> queue;
	std::shared_ptr<const UnitProgress> unit_progress;//this means that the reference can change but the object cannot
	std::shared_ptr<const Board> board;//this means that the reference can change but the object cannot
	std::shared_ptr<const Units> units;//this means that the reference can change but the object cannot
	int player = 0; //player index
};
