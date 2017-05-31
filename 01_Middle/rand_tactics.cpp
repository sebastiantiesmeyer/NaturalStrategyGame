#include "rand_tactics.h"

Command RandTactics::do_step(const Orders & order_list)
{
	Options opt = rand_step(order_list);
	return opt.choose();
}
