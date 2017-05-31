#pragma once
#include "data_types.h"
#include "imgui\imgui.h"
#include "board_viewer.h"
#include "orderCommands.h"
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

#include <string>
#include <iostream>

class DummyPlayer : public AbstractPlayer //Example Player implementation
{
public:
	DummyPlayer(const PlayerParameters &pars) : AbstractPlayer::AbstractPlayer(pars){}

	virtual void do_StartTurn()
	{
		endturn = false;
		q2.unitcmds.clear();
	}

	virtual void do_Render()
	{
		if(iteration != 0)
			ImGui::TextColored({ 1,0,1,1 }, "Thinking hard");
		else
			ImGui::TextColored({ 0,1,0,1 }, "Not my turn");
		ImGui::Separator(); char buff[256];

		float current_training_time = unit_progress.current_train_time;

		sprintf_s(buff, "%d/%d", unit_progress.progress[ROCK], (int)current_training_time);
		ImGui::RadioButton("Rock", (int*)&queue.train, (int)ROCK); ImGui::SameLine(100);
		if(queue.train == ROCK) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
		ImGui::ProgressBar(unit_progress.progress[ROCK] / current_training_time, { -1,0 }, buff);
		if(queue.train == ROCK) ImGui::PopStyleColor();

		sprintf_s(buff, "%d/%d", unit_progress.progress[SCISSOR], (int)current_training_time);
		ImGui::RadioButton("Scissor", (int*)&queue.train, (int)SCISSOR); ImGui::SameLine(100);
		if(queue.train == SCISSOR) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
		ImGui::ProgressBar(unit_progress.progress[SCISSOR] / current_training_time, { -1,0 }, buff);
		if(queue.train == SCISSOR) ImGui::PopStyleColor();

		sprintf_s(buff, "%d/%d", unit_progress.progress[PAPER], (int)current_training_time);
		ImGui::RadioButton("Paper", (int*)&queue.train, (int)PAPER); ImGui::SameLine(100);
		if(queue.train == PAPER) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
		ImGui::ProgressBar(unit_progress.progress[PAPER] / current_training_time, { -1,0 }, buff);
		if(queue.train == PAPER) ImGui::PopStyleColor();

		ImGui::Separator();
		view_board_and_add_command(board, q2, ind);
		ImGui::Separator();

		if(iteration != 0)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,1,0.5 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,1,0.75 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,1,1 });
			endturn = ImGui::Button("End Turn", { -1,45 });
			ImGui::PopStyleColor(3);
		}
		command_editor(q2, units, board, ind);
		auto train = queue.train;
		queue = sortCommands(q2, units);
		queue.train = train;
		command_editor(queue, units, board, ind);
	}
	CommandQueue q2;
	virtual bool do_Update()
	{
		return endturn;
	}
	int max_iterations = 20;
	bool endturn = false;
};

class RandomPlayer : public AbstractPlayer //Example Player implementation
{
public:
	RandomPlayer(const PlayerParameters &pars) : AbstractPlayer::AbstractPlayer(pars) {}

	virtual void do_StartTurn()
	{
		endturn = false;
	}

	virtual void do_Render()
	{
		if (iteration != 0)
			ImGui::TextColored({ 1,0,1,1 }, "Thinking hard");
		else
			ImGui::TextColored({ 0,1,0,1 }, "Not my turn");
		ImGui::Separator(); char buff[256];

		float current_training_time = unit_progress.current_train_time;

		sprintf_s(buff, "%d/%d", unit_progress.progress[ROCK], (int)current_training_time);
		ImGui::RadioButton("Rock", (int*)&queue.train, (int)ROCK); ImGui::SameLine(100);
		if (queue.train == ROCK) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
		ImGui::ProgressBar(unit_progress.progress[ROCK] / current_training_time, { -1,0 }, buff);
		if (queue.train == ROCK) ImGui::PopStyleColor();

		sprintf_s(buff, "%d/%d", unit_progress.progress[SCISSOR], (int)current_training_time);
		ImGui::RadioButton("Scissor", (int*)&queue.train, (int)SCISSOR); ImGui::SameLine(100);
		if (queue.train == SCISSOR) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
		ImGui::ProgressBar(unit_progress.progress[SCISSOR] / current_training_time, { -1,0 }, buff);
		if (queue.train == SCISSOR) ImGui::PopStyleColor();

		sprintf_s(buff, "%d/%d", unit_progress.progress[PAPER], (int)current_training_time);
		ImGui::RadioButton("Paper", (int*)&queue.train, (int)PAPER); ImGui::SameLine(100);
		if (queue.train == PAPER) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
		ImGui::ProgressBar(unit_progress.progress[PAPER] / current_training_time, { -1,0 }, buff);
		if (queue.train == PAPER) ImGui::PopStyleColor();

		ImGui::Separator();
		view_board_and_add_command(board, queue, ind);
		ImGui::Separator();

		command_editor(queue, units, board, ind);
	}

	virtual bool do_Update()
	{
		if(iteration == 1)
		{

			//change production in ~every fourth round
			if(std::rand() % 4 > 2)
			{
				queue.train = UNIT_TYPE(std::rand() % 3);
			}

			//parse through all units
			for(const auto &unit : units)
			{
				int r = std::rand() % 4;
				if(r < 4)
				{
					//if r>4, no command, else move in a random direction:
					Dir dir = Dir(r % 2, (r + 1) % 2);
					Command command;
					command.id = unit.second.id;
					command.dir = dir;
					queue.unitcmds.push_back(command);
				}
			}
		}
		//return iteration == 100;
		return true;
	}
	int max_iterations = 20;
	bool endturn = false;
};
