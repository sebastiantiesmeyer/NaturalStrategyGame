#pragma once
#include "data_types.h"
#include "sup_player.h"

class AbstractTactic
{
public:
	AbstractTactic(){}
	virtual Command step(const Unit &unit, const OrderList &order_list) //call this
	{	return do_step(order_list);	}
protected:
	virtual Command do_step(const OrderList &order_list) = 0;
};