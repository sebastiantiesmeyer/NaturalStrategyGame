#include <string>
#include <iostream>
#include "human_player.h"
#include "board_viewer.h"
#include "orderCommands.h"

void HumanPlayer::do_StartTurn()
{
	endturn = false;
	q2.unitcmds.clear();
}

void HumanPlayer::do_Render()
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
	view_board_and_add_command(board, q2, player);
	ImGui::Separator();

	if(iteration != 0)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,1,0.5 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,1,0.75 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,1,1 });
		endturn = ImGui::Button("End Turn", { -1,45 });
		ImGui::PopStyleColor(3);
	}
	command_editor(q2, units, board, player);
	auto train = queue.train;
	queue = sortCommands(q2, units);
	queue.train = train;
	command_editor(queue, units, board, player);
}