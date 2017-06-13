#pragma once
#include "data_types.h"
#include "imgui\imgui.h"

class AbstractPlayer
{
public: // AbstractGame and its children should call these functions only

	virtual void setPlayerParameters(const Board &_board, const Units &_units, UnitProgress &const _unit_progress, int _player)
	{
		board = _board; 
		units = _units; 
		unit_progress = _unit_progress;
		player = _player;
		queue.train = ROCK;

	}

	//AbstractPlayer() = default;
	// Messages to the player that the turn has started.
	void StartTurn()
	{
		iteration = 1;
		queue.unitcmds.clear();
		do_StartTurn(); //implement do_StartTurn()
	}

	// Does (further) calculation if needed. Returns true when calculation is done
	bool Update()
	{
		if(0 != iteration) // implement do_Update()
			if(do_Update()) iteration = 0; //only return true when you end your turn!
			else ++iteration;
		return 0 == iteration; //returns true when aswer available => call GetCommandQueue then
	}

	// Draws GUI elements and user interaction. Dont call if that is not needed.
	void Render()
	{	//does (some)calculation and/or draws GUI elements, return true when plyaer ended turn
		if(ImGui::Begin((0 == player ? "Player 1" : "Player 2"))) //window for this player
			do_Render(); //implement do_Render()
		ImGui::End();
	}

	// Depricted. Use Render() and Update() instead !!
	bool RenderUpdate() = delete;

	//Returns the command queue, call only when Update() already returned true!
	const CommandQueue& GetCommandQueue()
	{
		if(iteration != 0) throw std::exception("Computation should be already done");
		return queue;
	}

protected: //TO IMPLEMENT:

	// Implement to initializes your stuff at the beginning of your turn
	//		You can do computation here if you dont want to show its progress
	virtual void do_StartTurn() = 0;

	// Implement to do the calculation here, returns true if the turn is ended
	virtual bool do_Update() = 0;
	
	// draws stuff you want to draw, and human interacion
	virtual void do_Render() = 0;
protected:
	CommandQueue queue;
	UnitProgress &const unit_progress;
	Board &const board;
	Units &const units;
	
	int player = 0; //dont change! the player's index = 0 or 1.
	int iteration; //dont change! the frames spent calculating or deciding what to do
};