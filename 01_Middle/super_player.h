#pragma once
/*
This file provides SuperPlayer, with configurable tactics (local strategy) and stragey (global strategy).
One can freely combine different AbstractStrategys and AbstractTactics using this class.

AbstractTactics contolles individual units. The three most important caracteristics:
	- Surfival instinct: the units avoids immediate threats
	- Obeys Orders: tries to move towards its goal/target (while staying alive)
	- Takes opportunities of easy kills

AbstractStrategy issues Orders, which are executed by units via tactics. For example:
	- Heuristics crafted by The Laughing Machine
	- Human player only controlling the strategy => Cyborg Player :)
		> Real Time-Strategy game experience
		> Ideal for testing tactics and
		> Training other global strategy
	- Learned strategy ...
*/
#include "abstract_player.h"
#include "abstract_tactics.h"
#include "abstract_strategy.h"
#include "orderCommands.h"
#include <memory>

//Super player
//Create a general player from a tactic (local strategy) and a strategy (global)
//	usage: pass the pointer to strategy and tactic to constructor
class SuperPlayer : public AbstractPlayer
{
public:
	SuperPlayer(std::shared_ptr<AbstractStrategy> &strategy, std::shared_ptr<AbstractTactic> &tactic)
		: strategy(strategy), tactic(tactic) {};
	~SuperPlayer()
	{
		std::cout << "~SuperPlayer()" << std::endl;
	}
	virtual void setPlayerParameters(std::shared_ptr<const Board>_board, std::shared_ptr<const Units>_units, std::shared_ptr<UnitProgress>_unit_progress, int _player) {
		//AbstractPlayer::setPlayerParameters(_board, _units, _unit_progress, _player);
		strategy->setParams(queue, _board, _units, _unit_progress, _player);
		tactic->setBoard(board);
	}
protected:
	virtual void do_StartTurn()
	{
		strategy->setParams(queue, board, units, unit_progress, player);

		Units::const_iterator mid = units->lower_bound(0);	// negative = player 1 AND positive player 0
		for(auto it = (player == 0 ? mid : units->cbegin()); it != (player == 0 ? units->end() : mid); ++it)
			all_orders[it->first]; //constructs an empty order if it does not exist there

		strategy->changeOrders(all_orders); //global strategy module :)
		tactic->setBoard(board);
	}
	virtual bool do_Update() //todo real time
	{
		if(strategy->Update()) //
		{
			AllOrders tmp; //we recreate the map to filter out dead units
			Units::const_iterator mid = units->lower_bound(0); // negative = player 1 AND positive player 0
			for(auto it = (player == 0 ? mid : units->cbegin());
				it != (player == 0 ? units->end() : mid);
				++it)		// iterates thorugh the player's units only
			{	//we don't want to copy the underlying structures (vectors), so move swap
				tmp[it->first].swap(all_orders[it->first]);
				//then, transform Order to Command, and add it to queue.
				queue->unitcmds.push_back(tactic->step(it->second, tmp[it->first]));
			}
			//all_orders = tmp; //tmp is not needed anymore, we swap
			all_orders.swap(tmp);
			*queue = sortCommands(*queue, *units); //sort commands
			queue->train = strategy->train();
			return true;
		}
		else return false;
	}
	virtual void do_Render() //todo real time
	{
		strategy->Render();
	}
protected:
	std::shared_ptr<AbstractStrategy> strategy;
	std::shared_ptr<AbstractTactic> tactic;
	AllOrders all_orders; //maps Unit pointer to its OrderList as described above
};
