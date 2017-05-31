#pragma once
#include "data_types.h"
#include "sup_player.h"

class AbstractStrategy
{
public:
	void setParams(const CommandQueue &cqueue, const PlayerParameters &params)
	{
		queue = cqueue;
		unit_progress = params.unit_progress;
		board = params.board;
		units = params.units;
		ind = params.ind;
	}
	virtual void changeOrders(AllOrders &orders) = 0;
	virtual UNIT_TYPE train() = 0;
protected:
	CommandQueue & const queue;
	UnitProgress & const unit_progress;
	Board & const board;
	Units & const units;
	int ind = 0;
};
