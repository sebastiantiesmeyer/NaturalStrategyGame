#pragma once
#include "abstract_tactics.h"

//Avoids immadiate threat in 3x3 window, and obeys Orders
class TriadTactic : public AbstractTactic
{
public:
	TriadTactic() : AbstractTactic::AbstractTactic() {}

	virtual Command step(const Unit &unit, const OrderList &order_list);
};
