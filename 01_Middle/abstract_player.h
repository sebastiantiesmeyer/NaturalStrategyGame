#pragma once
#include "data_types.h"

class AbstractPlayer
{
public:
	AbstractPlayer(const Board &board, const Units &units): board(board), units(units){}
	const CommandQueue& move(UnitProgress _unit_progress)
	{
		unit_progress = _unit_progress;
		queue.unitcmds.clear();
		do_move();
		return queue;
	}
protected:
	virtual void do_move() {};
	CommandQueue queue;
	UnitProgress unit_progress;
	const Board &board;
	const Units &units;
};

class DummyPlayer : public AbstractPlayer
{
public:
	DummyPlayer(const Board &board, const Units &units) : AbstractPlayer::AbstractPlayer(board,units){}
	virtual void do_move()
	{
		std::cout << "ASD" << std::endl;
		queue.train = ROCK;
	}
};
