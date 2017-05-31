#pragma once
#include "abstract_tactics.h"
#include "local.h"

class RandTactics : public AbstractTactics
{
public:
	RandTactics(const Unit &unit) : AbstractTactics::AbstractTactics(unit) {}
protected:
	virtual Command do_step(const Orders &order_list);
	virtual Options rand_step(const Orders &order_list) = 0;
};