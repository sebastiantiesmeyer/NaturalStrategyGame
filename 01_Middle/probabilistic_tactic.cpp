#include "probabilistic_tactic.h"
#include "local.h"

Command ProbabilisticTactic::step(const Unit & unit, const OrderList & order_list)
{ //TODO: order target!
	Options options = { 1,1,1,1,1 };
	localAvoid3x3(unit, board, options);
	Command cmd;
	cmd.id = unit.id;
	cmd.dir = options.choose();
	return cmd;
}
