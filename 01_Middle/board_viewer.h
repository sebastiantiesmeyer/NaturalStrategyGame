#pragma once
#include "data_types.h"
#include "imgui\imgui.h"

template<typename T> T clamp(const T &x, const T &m, const T &M)
{	return (x < m ? m : (x > M ? M : x)); }

inline ImVec4 conv(const glm::vec4 &vec) { return{ vec.x,vec.y,vec.z,vec.w }; }

//four corners of the table
const glm::vec4 base00 = glm::vec4(0.12f, 0.38f, 0.91f, 1.00f);
const glm::vec4 base01 = glm::vec4(0.26f, 0.85f, 0.10f, 1.00f);
const glm::vec4 base10 = glm::vec4(0.79f, 0.12f, 0.34f, 1.00f);
const glm::vec4 base11 = glm::vec4(0.76f, 0.10f, 0.86f, 1.00f);
//button state (multiplicative)
const glm::vec4 normal = glm::vec4(0.85f, 0.85f, 0.85f, 0.55f);
const glm::vec4 hoover = glm::vec4(0.85f, 0.95f, 1.00f, 0.75f);
const glm::vec4 active = glm::vec4(1.00f, 0.95f, 1.00f, 1.00f);
//faction (additive)
const glm::vec4 fridly = glm::vec4(0.00f, 0.00f, 1.00f, 1.00f);
const glm::vec4 enemys = glm::vec4(1.00f, 0.00f, 0.00f, 0.00f)*0.5;
const glm::vec4 naturl = glm::vec4(0.00f, 1.00f, 0.00f, 0.00f)*0.5;
//Units
const char* RSP[3] = {"R", "S", "P" };

void view_board(const Board &board, int player)
{
	float w = clamp(ImGui::GetContentRegionAvail().x, 200.f, 600.f);
	float h = clamp(ImGui::GetContentRegionAvail().y + 110.f, 200.f, 600.f);

	float button_size = std::min(w,h) / 20 - 8;
	for(int i = 0; i < game_size; ++i)
	{
		glm::vec4 base0 = glm::mix(base00, base01, glm::vec4(i / (float)(game_size - 1)));
		glm::vec4 base1 = glm::mix(base10, base11, glm::vec4(i / (float)(game_size - 1)));
		ImGui::PushID(i);
		for(int j = 0; j < game_size; ++j)
		{
			glm::vec4 base = glm::mix(base0, base1, glm::vec4(j / (float)(game_size - 1)));
			const Cell &cell = board[Position(i,j)];

			glm::vec4 color = base;
			if(cell.unit && cell.unit->player == player || i == 0 && j == 0)
				color += fridly;
			else if(cell.unit && cell.unit->player != player || i == game_size-1 && j == game_size-1)
				color += enemys;
			else if(i == 0 && j == game_size-1 || i == game_size-1 && j == 0)
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
