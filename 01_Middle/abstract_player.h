#pragma once
#include "data_types.h"

struct PlayerParameters //Simple constructors for Player classes
{
	const Board &board;
	const Units &units;
	const UnitProgress &unit_progress;
	int ind;
};

class AbstractPlayer
{
public:
	AbstractPlayer(const PlayerParameters &pars)
		: board(pars.board), units(pars.units), unit_progress(pars.unit_progress), ind(pars.ind)
	{
		queue.train = ROCK; //Just in case
	}

	void StartTurn()
	{	//messages to the player the the turn has started. Dont do computation here, just initializations!
		iteration = 1;
		queue.unitcmds.clear();
		do_StartTurn();
	}

	bool RenderUpdate()
	{	//does (some)calculation and/or draws GUI elements, return true when plyaer ended turn
		if(0 != iteration)
		{
			if(do_Update()) iteration = 0; //only return true when you end your turn!
			else ++iteration; // my mistake
		}
		if(ImGui::Begin((0 == ind ? "Player 1" : "Player 2")))
		{	//creates a window for this player, do_Render only runs when window is active!
			do_Render();
		}
		ImGui::End();
		return  0 == iteration; //returns true when aswer available
	}

	const CommandQueue& GetCommandQueue()
	{	//returns the command queue
		if(iteration != 0) throw std::exception("Computation should be already done");
		return queue;
	}
protected:
	//TO IMPLEMENT:
	virtual void do_StartTurn() = 0;	// initializes own stuff
	virtual bool do_Update() = 0;		// does (some) calculation, returns true if turn is ended
	virtual void do_Render() = 0;		// draws stuff you want to draw
protected:
	CommandQueue queue;
	const UnitProgress &unit_progress;
	const Board &board;
	const Units &units;
	int ind = 0;
	int iteration; // zero if anser has already been provided
};