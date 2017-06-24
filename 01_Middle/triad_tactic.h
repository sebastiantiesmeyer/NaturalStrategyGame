#pragma once
#include "abstract_tactics.h"

//Avoids immadiate threat in 3x3 window, and obeys Orders
class TriadTactic : public AbstractTactic
{
public:
	TriadTactic() : AbstractTactic::AbstractTactic() {}

	virtual Command step(const Unit &unit, const OrderList &order_list);
private:

	bool is_in_triad(const Unit & unit);
	bool all_different(const Unit & unit, const Position & pos0, const Position & pos1);
};
