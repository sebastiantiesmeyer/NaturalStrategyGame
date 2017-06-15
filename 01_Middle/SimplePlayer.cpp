#include "simple_player.h"
//#include "orderCommands.h"
#include "board_viewer.h"
#include "local.h"

bool SimplePlayer::endturn = false;

void SimplePlayer::do_StartTurn()
{
	if(std::rand() % 6 > 2)
		queue.train = UNIT_TYPE(std::rand() % 3);
	queue.train = UNIT_TYPE(player); //muhaha
	for(const auto &pair : *units)
	{
		const Unit& unit = pair.second;
		if(unit.player == player)
		{
			//Options options = getOptions(unit, board);
			Options options = { 1,1,1,1,1 };
			//localNxN(unit, board, options);
			localAvoid3x3(unit, *board, options);
			int unit_job = abs(unit.id % 3); //0 = defend base; {1, 2} = attack outpost {0, 1} then enemy base
			if(unit.pos.x == (*board).size() - 1 && unit.pos.y == (*board).size() - 1)
				options.still *= 20; //captureing enemy
			else if((*unit_progress).our_base_captured) //defence
			{
				options.up *= 3.5 + 30.f / norm1(unit.pos);
				options.left *= 3.5 + 30.f / norm1(unit.pos);
			}
			else if(unit_job == 0 && norm1(unit.pos) > 1) //not too close the base
			{
				options.up *= 3.5;
				options.left *= 3.5;
			}
			else if((*board).getPlayerAtOutpost(0, player) != player && unit_job == 1)
			{
				options.right *= 3.5;
				options.up *= 2.5;
			}
			else if((*board).getPlayerAtOutpost(1, player) != player && unit_job == 2)
			{
				options.down *= 3.5; //target is the enemy base
				options.left *= 2.5;
			}
			else
			{
				options.right *= 3; //target is the enemy base
				options.down *= 3;
			}
			Command cmd;
			cmd.dir = options.choose();
			stored_options.push_back(options);
			cmd.id = pair.first;
			queue.unitcmds.push_back(cmd);
			unit.options = &stored_options.back();
			unit.command = &queue.unitcmds.back();
		}
	}
	endturn = false;
}
void SimplePlayer::do_Render()
{
	view_board_and_add_command(*board, queue, player);
	if(iteration != 0)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,1,0.5 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,1,0.75 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,1,1 });
		endturn = ImGui::Button("End Turn", { -1,45 });
		ImGui::PopStyleColor(3);
	}
	command_editor(queue, *units, *board, player);
}