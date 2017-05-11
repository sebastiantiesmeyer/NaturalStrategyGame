#include "board_viewer.h"

void view_board(const Board &board, int player)
{
	float w = clamp(ImGui::GetContentRegionAvail().x, 200.f, 600.f);
	float h = clamp(ImGui::GetContentRegionAvail().y + 110.f, 200.f, 600.f);

	float button_size = std::min(w, h) / 20 - 8;
	for(int i = 0; i < game_size; ++i)
	{
		glm::vec4 base0 = glm::mix(base00, base01, glm::vec4(i / (float)(game_size - 1))); //linear interpolation
		glm::vec4 base1 = glm::mix(base10, base11, glm::vec4(i / (float)(game_size - 1))); //linear interpolation
		ImGui::PushID(i);
		for(int j = 0; j < game_size; ++j)
		{
			glm::vec4 base = glm::mix(base0, base1, glm::vec4(j / (float)(game_size - 1))); //(bi)linear interpolation
			const Cell &cell = board[Position(i, j)];

			glm::vec4 color = base;
			if(cell.unit && cell.unit->player == player || i == 0 && j == 0)
				color += fridly;
			else if(cell.unit && cell.unit->player != player || i == game_size - 1 && j == game_size - 1)
				color += enemys;
			else if(i == 0 && j == game_size - 1 || i == game_size - 1 && j == 0)
				color += naturl;

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

	float button_size = std::min(w, h) / 20 - 8;
	for(int i = 0; i < game_size; ++i)
	{
		glm::vec4 base0 = glm::mix(base00, base01, glm::vec4(i / (float)(game_size - 1)));
		glm::vec4 base1 = glm::mix(base10, base11, glm::vec4(i / (float)(game_size - 1)));
		ImGui::PushID(i);
		for(int j = 0; j < game_size; ++j)
		{
			glm::vec4 base = glm::mix(base0, base1, glm::vec4(j / (float)(game_size - 1)));
			Position pos = auto_rotate(Position(i, j), player);
			const Cell &cell = board[pos];

			glm::vec4 color = base;

			bool is_neighbour = false;
			if(selected[player]) //a selection is active
			{
				Dir dir = lastclikk[player] - pos;
				if(norm1(dir) > 1)	color *= darken;
				if(norm1(dir) == 1)	color += neighb;
				if(norm1(dir) == 0)	color += selctd;
			}

			if(cell.unit && cell.unit->player == player || pos.x == 0 && pos.y == 0)
				color += fridly;
			else if(cell.unit && cell.unit->player != player || pos.x == game_size - 1 && pos.y == game_size - 1)
				color += enemys;
			else if(pos.x == 0 && pos.y == game_size - 1 || pos.x == game_size - 1 && pos.y == 0)
				color += naturl;

			ImGui::PushID(j);
			ImGui::PushStyleColor(ImGuiCol_Button, conv(color*normal));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, conv(color*hoover));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, conv(color*active));

			bool clicked = ImGui::Button((cell.unit ? RSP[cell.unit->type] : " "), ImVec2(button_size, button_size));
			ImGui::PopStyleColor(3); //Pop 3 colors at once from the color stack
			ImGui::PopID();	//Have to pop button id
			ImGui::SameLine(); //Next Button appears on the same line

			if(clicked && !selected[player] &&		// nothing is selected, and the cell has a our unit inside:
			   board[pos].unit && board[pos].unit->player == player)
			{
				selected[player] = true;
				lastclikk[player] = pos;
			}
			else if(clicked && selected[player]) // a unit is selected
			{
				Dir dir = lastclikk[player] - pos;
				if(norm1(dir) == 1)			//selecting a neighbour
				{
					Command cmd;	cmd.id = board[lastclikk[player]].id;	cmd.dir = dir;
					queue.unitcmds.push_back(cmd);
					selected[player] = false;
				}
				else if(norm1(dir) > 1 &&	//selecting another unit
						board[pos].unit && board[pos].unit->player == player)
					lastclikk[player] = pos;
				else						// deselecting unit
					selected[player] = false;
			}
		}
		ImGui::PopID();		//Have to pop row id
		ImGui::NewLine();	   //Next row below
	}
}
