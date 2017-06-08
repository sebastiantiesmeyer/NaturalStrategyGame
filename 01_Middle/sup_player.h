#pragma once
#include "abstract_player.h"
#include "abstract_tactics.h"
#include "abstract_strategy.h"
#include "orderCommands.h"

struct Order //is given to local strategy (tactics)
{
	Unit *unit = nullptr;	// The unit that has to "order" 'guides'
	Position target = glm::ivec2(0,0);	// The unit has to move towards this RELATIVE position
	float sacrifice = 0.5;	// Any value between 0 and 1, examples:
							//  0.0 means it will run from the same kind
							//  0.5 means it will not run, but will not attack either
							//  1.0 means it will attack its kind
};

typedef std::vector<Order> OrderList; // First order is the top-priority
typedef std::map<const Unit*, OrderList> AllOrders;
//AllOrders = map<Unit*, OrderList> it maps a unit to its order list
//OrdeList = vector<Order> the priority list of orders, usually just one
//Order = struct{Unit*, Position, float sacrafice} subject to change!

//Super player
//Create a general player from a tactic (local strategy) and a strategy (global)
// usage: pass the strategies
class SupPlayer : protected AbstractPlayer
{
public:
	SupPlayer(const PlayerParameters &params, AbstractStrategy *strategy, AbstractTactic *tactic):
		AbstractPlayer::AbstractPlayer(params), strategy(strategy), tactic(tactic)
	{
		strategy->setParams(queue, params);
	}
protected:
	void do_StartTurn()
	{
		strategy->changeOrders(all_orders);
		Units::const_iterator mid = units.lower_bound(0); // negative = player 1 AND positive player 0
		for(auto it = (ind == 0 ? mid : units.cbegin()); it != (ind == 0 ? units.end(): mid); ++it)
		{
			queue.unitcmds.push_back(tactic->step(it->second, all_orders[&it->second]));
		}
		queue = sortCommands(queue, units);
		strategy->train();
	}
	bool do_Update() //todo real time
	{
		return strategy->Update();
	}
	void do_Render() //todo real time
	{
		strategy->Redner();
	}

protected:
	AbstractStrategy *strategy;
	AbstractTactic *tactic;
	AllOrders all_orders;
};
