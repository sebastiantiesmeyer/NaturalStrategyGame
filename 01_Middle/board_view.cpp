#include "board_viewer.h"

void view_board(const Board &board)
{
	float w = clamp(ImGui::GetContentRegionAvail().x, 200.f, 600.f);
	float h = clamp(ImGui::GetContentRegionAvail().y + 110.f, 200.f, 600.f);

	float button_size = std::min(w, h) / game_size - 8;
	for(int i = 0; i < game_size; ++i)
	{
		glm::vec4 base0 = glm::mix(base00, base01, glm::vec4(i / (float)(game_size - 1))); //linear interpolation
		glm::vec4 base1 = glm::mix(base10, base11, glm::vec4(i / (float)(game_size - 1))); //linear interpolation
		ImGui::PushID(i);
		for(int j = 0; j < game_size; ++j)
		{
			glm::vec4 color = glm::mix(base0, base1, glm::vec4(j / (float)(game_size - 1))); //(bi)linear interpolation
			const Cell &cell = board.at(Position(i, j));

			if(i == game_size - 1 && j == game_size - 1) color = glm::mix(color, playr1, interp);
			else if(i == 0 && j == 0)					 color = glm::mix(color, playr0, interp);


			if (cell.unit && cell.unit->player == 0)
				color = glm::mix(color, playr0, interp);
			else if (cell.unit && cell.unit->player != 0)
				color = glm::mix(color, playr1, interp);
			else if (i == 0 && j == game_size - 1)
			{
				switch (board.getPlayerAtOutpost(0, 0))
				{
				case -1:	color = glm::mix(color,naturl, interp); break;
				case 0:		color = glm::mix(color,playr0, interp); break;
				case 1:		color = glm::mix(color,playr1, interp); break;
				}
			}
			else if (j == 0 && i == game_size - 1)
			{
				switch(board.getPlayerAtOutpost(1, 0))
				{
				case -1:	color = glm::mix(color, naturl, interp); break;
				case 0:		color = glm::mix(color, playr0, interp); break;
				case 1:		color = glm::mix(color, playr1, interp); break;
				}
			}
			ImGui::PushID(j);
			ImGui::PushStyleColor(ImGuiCol_Button, conv(color*normal));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, conv(color*hoover));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, conv(color*active));

			ImGui::Button((cell.unit ? RSP[cell.unit->type] : " "), ImVec2(button_size, button_size));
			ImGui::PopStyleColor(3); //Pop 3 colors at once from the color stack
			ImGui::PopID();	//Have to pop button id
			ImGui::SameLine(); //Next Button appears on the same line
		}
		ImGui::PopID();		//Have to pop row id
		ImGui::NewLine();	   //Next row below
	}
}

void view_board_and_add_command(const Board &board, CommandQueue &queue, int player)
{
	float w = clamp(ImGui::GetContentRegionAvail().x, 200.f, 600.f);
	float h = clamp(ImGui::GetContentRegionAvail().y + 110.f, 200.f, 600.f);
	static Position lastclikk[2] = { Position(0,0), Position(0, 0) };	// last click is saved per-player
	static bool selected[2] = { false,false };							// player i selected a player

	float button_size = std::min(w, h) / game_size - 8;
	for(int i = 0; i < game_size; ++i)
	{
		glm::vec4 base0 = glm::mix(base00, base01, glm::vec4(i / (float)(game_size - 1)));
		glm::vec4 base1 = glm::mix(base10, base11, glm::vec4(i / (float)(game_size - 1)));
		ImGui::PushID(i);
		for(int j = 0; j < game_size; ++j)
		{
			glm::vec4 color = glm::mix(base0, base1, glm::vec4(j / (float)(game_size - 1)));
			Position pos = Position(i, j);
			const Cell &cell = board(pos, player);

			if(i == game_size - 1 && j == game_size - 1) color = glm::mix(color, enemys, interp);
			else if(i == 0 && j == 0)					 color = glm::mix(color, fridly, interp);

			bool is_neighbour = false;
			if(selected[player]) //a selection is active
			{
				Dir dir = lastclikk[player] - pos;
				if(norm1(dir) > 1)	color *= darken;
				if(norm1(dir) == 1)	color += neighb;
				if(norm1(dir) == 0)	color += selctd;
			}

			if(cell.unit && cell.unit->player == player)
				color = glm::mix(color, fridly, interp);
			else if(cell.unit && cell.unit->player != player)
				color = glm::mix(color, enemys, interp);
			else if(i == 0 && j == game_size - 1) //first outpost
			{
				int op = board.getPlayerAtOutpost(0, player);
				if(op == -1)			color = glm::mix(color, naturl, interp);
				else if(op == player)	color = glm::mix(color, fridly, interp);
				else					color = glm::mix(color, enemys, interp);
			}
			else if(j == 0 && i == game_size - 1) //second outpost
			{
				int op = board.getPlayerAtOutpost(1, player);
				if(op == -1)			color = glm::mix(color, naturl, interp);
				else if(op == player)	color = glm::mix(color, fridly, interp);
				else					color = glm::mix(color, enemys, interp);
			}

			ImGui::PushID(j);
			ImGui::PushStyleColor(ImGuiCol_Button, conv(color*normal));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, conv(color*hoover));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, conv(color*active));

			bool clicked = ImGui::Button((cell.unit ? RSP[cell.unit->type] : " "), ImVec2(button_size, button_size));
			if(cell.unit && ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("id= %d", cell.unit->id);
				ImGui::EndTooltip();
			}
			ImGui::PopStyleColor(3); //Pop 3 colors at once from the color stack
			ImGui::PopID();	//Have to pop button id
			ImGui::SameLine(); //Next Button appears on the same line

			if(clicked && !selected[player] &&		// nothing is selected, and the cell has a our unit inside:
			   board(pos, player).unit && board(pos, player).unit->player == player)
			{
				selected[player] = true;
				lastclikk[player] = pos;
			}
			else if(clicked && selected[player]) // a unit is selected
			{
				Dir dir = pos - lastclikk[player];
				if(norm1(dir) == 1)			//selecting a neighbour
				{
					Command cmd;	cmd.id = board(lastclikk[player], player).id;	cmd.dir = dir;
					queue.unitcmds.push_back(cmd);
					selected[player] = false;
				}
				else if(norm1(dir) > 1 &&	//selecting another unit
						board(pos, player).unit && board(pos, player).unit->player == player)
					lastclikk[player] = pos;
				else						// deselecting unit
					selected[player] = false;
			}
		}
		ImGui::PopID();		//Have to pop row id
		ImGui::NewLine();	   //Next row below
	}
}

void command_editor(CommandQueue &queue, const Units &units, const Board &board, int player)
{
	if(!ImGui::BeginChild(player, { 0,0 }, true))
	{
		ImGui::EndChild();
		return;
	}
	static bool show_unit_data[2] = { true,true };
	static bool show_cell_data[2] = {true,true};
	ImGui::Checkbox("Show Unit Data", &show_unit_data[player]);
	ImGui::SameLine();
	ImGui::Checkbox("Show Cell Data", &show_cell_data[player]);
	const char* names[3] = { "Rock", "Scizzor", "Paper" };
	int id = 0;
	ImGui::PushItemWidth(100); //small input ints
	for(int i = 0; i < queue.unitcmds.size(); ++i)
	{
		Command &command = queue.unitcmds[i];
		ImGui::PushID(id++);
		ImGui::Separator();
		if(ImGui::InputInt("##x", &command.dir.x, 1.0F,-1,1) && norm1(command.dir) > 1)
			command.dir = Dir(0);
		ImGui::SameLine();
		if(ImGui::SmallButton("^") && i > 0)
			std::swap(command, queue.unitcmds[i - 1]);
		if(show_unit_data[player])
		{
			ImGui::SameLine();
			const Unit& unit = units.at(command.id);
			ImGui::TextDisabled("%s at (%d,%d) id=%d", names[unit.type], unit.pos.x, unit.pos.y, unit.id);
		}
		//next line
		if(ImGui::InputInt("##y", &command.dir.y, 1.0F, -1, 1) && norm1(command.dir) > 1)
			command.dir = Dir(0);
		ImGui::SameLine();
		if(ImGui::SmallButton("v") && i < queue.unitcmds.size() - 1)
			std::swap(command, queue.unitcmds[i + 1]);
		if(show_unit_data[player] && show_cell_data[player])
		{
			ImGui::SameLine();
			const Unit& unit = units.at(command.id);
			const Cell cell = board(unit.pos, player);
			ImGui::TextDisabled("id=%d", cell.id);
		}
		ImGui::SameLine();
		if(ImGui::Button("Delete"))
			queue.unitcmds.erase(queue.unitcmds.begin() + i);
		ImGui::PopID();
	}
	ImGui::PopItemWidth();
	ImGui::EndChild();
}
