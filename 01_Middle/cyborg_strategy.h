#pragma once
#include "abstract_strategy.h"

class CyborgStrategy : public AbstractStrategy
{
public:
	virtual void changeOrders(AllOrders &allorders)
	{ //just like startTurn;
		allorders = allorders;
		endturn = false;
	}
	virtual bool Update() //return true when the turn is ended
	{
		return endturn;
	}
	virtual void Render()
	{

	}
public:
	AllOrders &allorders;
	bool endturn = false;
};