#pragma once
#include "abstract_tactics.h"

//Avoids immadiate threat in 3x3 window, and obeys Orders
class ProbabilisticTactic : public AbstractTactic
{
public:
	ProbabilisticTactic() : AbstractTactic::AbstractTactic(){}

	/*virtual*/ Command step(const Unit &unit, const OrderList &order_list);
};
