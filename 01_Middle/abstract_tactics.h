#pragma once
#include "data_types.h"

class AbstractTactic
{
public:
	AbstractTactic(const Unit &unit) :unit(unit) {}
	virtual Command step(const Orders & order_list) //call this
	{	return do_step(order_list);	}
protected:
	virtual Command do_step(const Orders &order_list) = 0;

protected:
	const Unit &unit;
};