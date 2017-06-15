#include "probabilistic_tactic.h"
#include "local.h"

Command ProbabilisticTactic::step(const Unit & unit, const OrderList & order_list)
{
	Options options = { 1,1,1,1,1 };
	localAvoid3x3(unit, *board, options);
	float weights[3] = {0.3, 1.0, 5.0}; //biases towards goal
	if(order_list.size() > 1)
	{
		Position diff = unit.pos - order_list[0].target;
		int len = norm1(diff);
		Dir dir = clamp2(diff, -1, 1);
		options.up    *= weights[1 - dir.x]; //+1 is down ==> up*=smallest if dir.x=+1
		options.down  *= weights[1 + dir.x]; //similarly -1 is up
		options.left  *= weights[1 - dir.y]; //-1 is left
		options.right *= weights[1 + dir.y]; //+1 is right
		options.still *= (1.0 + order_list[0].sacrifice) / (float)(len + 1);
	}
	Command cmd;
	cmd.id = unit.id;
	cmd.dir = options.choose();
	return cmd;
}
