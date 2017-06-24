#include "probabilistic_tactic.h"
#include "local.h"

Command ProbabilisticTactic::step(const Unit & unit, const OrderList & order_list)
{
	Options options = { 1,1,1,1,1 };
	localAvoid3x3(unit, *board, options);
	localNxN(unit, *board, options);
	float weights[4] = {0.2, 0.7, 8.0, 32.0}; //biases towards goal
	if(order_list.size() >= 1)
	{
		Position diff = unit.pos - order_list[0].target;
		int len = norm1(diff);
		Dir dir = clamp2(diff, -1, 1);
		options.down    *= weights[1 - dir.x]; //-1 is down ==> up*=smallest if dir.x=+1
		options.up  *= weights[1 + dir.x]; //similarly +1 is up
		options.right  *= weights[1 - dir.y]; //however, -1 is left
		options.left *= weights[1 + dir.y]; //+1 is right
		options.still *= (1.0 + order_list[0].sacrifice) / (float)(len + 1);
		if(order_list[0].target == unit.pos)
			options.still *= weights[3];
	}
	Command cmd;
	cmd.id = unit.id;
	cmd.dir = options.choose();
	if(unit.options) *unit.options = options;
	else unit.options = new Options(options);
	if(unit.command) *unit.command = cmd;
	else unit.command = new Command(cmd);
	return cmd;
}
