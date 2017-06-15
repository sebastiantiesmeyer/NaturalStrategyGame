#include "cyborg_strategy.h"
#include "board_viewer.h"

void give_orders(const Board &board, const Units &units, AllOrders &allorders, int player)
{
	float w = clamp(ImGui::GetContentRegionAvail().x, 200.f, 600.f);
	float h = clamp(ImGui::GetContentRegionAvail().y + 110.f, 200.f, 600.f);
	static Position lastclikk[2] = { Position(0,0), Position(0, 0) };	// last click is saved per-player
	//static bool selected[2] = { false,false };		// player i selected a player
	static int selected[2] = {0, 0}; //player i selected a unit id
	int game_size = board.size();
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
			if(selected[player] != 0) //a selection is active
			{
				Dir dir = lastclikk[player] - pos;
				if(norm1(dir) == 0)	color += selctd;
				else				color += neighb;
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
			ImGui::PushStyleColor(ImGuiCol_Button, color*normal);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color*hoover);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, color*active);

			bool clicked = ImGui::Button((cell.unit ? RSP[cell.unit->type] : " "), ImVec2(button_size, button_size));
			if(cell.unit && ImGui::IsItemHovered()) draw_unit_tooltip(cell, player);
			ImGui::PopStyleColor(3);
			ImGui::PopID();
			ImGui::SameLine();
			const Unit * unitptr = board(pos, player).unit;
			if(clicked && selected[player] == 0 &&		// nothing is selected and
			   unitptr && unitptr->player == player)	//the cell has a our unit inside
			{
				selected[player] = unitptr->id;
				lastclikk[player] = pos;
			}
			else if(clicked && selected[player]) // a unit is selected
			{
				Dir dir = pos - lastclikk[player];
				if(dir.x == 0 && dir.y == 0) //deselecting unit
					selected[player] = false;
				else
				{
					Order order; order.target = pos; //this order is issued
					allorders[&units.at(selected[player])].assign(1, order); // explanation:
					//the unit may have moved, thus we look it up thorugh its id
				}
			}
		}
		ImGui::PopID();
		ImGui::NewLine();
	}
}

void CyborgStrategy::Render()
{
	if(!ImGui::BeginChild("Order editor", { 0,300 }, true))
	{
		ImGui::EndChild();
		return;
	}
	char buff[256];

	float current_training_time = (*unit_progress).current_train_time;

	sprintf_s(buff, "%d/%d", (*unit_progress).progress[ROCK], (int)current_training_time);
	ImGui::RadioButton("Rock", (int*)(*queue).train, (int)ROCK); ImGui::SameLine(100);
	if(whattotrain == ROCK) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
	ImGui::ProgressBar((*unit_progress).progress[ROCK] / current_training_time, { -1,0 }, buff);
	if(whattotrain == ROCK) ImGui::PopStyleColor();

	sprintf_s(buff, "%d/%d", (*unit_progress).progress[SCISSOR], (int)current_training_time);
	ImGui::RadioButton("Scissor", (int*)(*queue).train, (int)SCISSOR); ImGui::SameLine(100);
	if(whattotrain == SCISSOR) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
	ImGui::ProgressBar((*unit_progress).progress[SCISSOR] / current_training_time, { -1,0 }, buff);
	if(whattotrain == SCISSOR) ImGui::PopStyleColor();

	sprintf_s(buff, "%d/%d", (*unit_progress).progress[PAPER], (int)current_training_time);
	ImGui::RadioButton("Paper", (int*)(*queue).train, (int)PAPER); ImGui::SameLine(100);
	if(whattotrain == PAPER) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
	ImGui::ProgressBar((*unit_progress).progress[PAPER] / current_training_time, { -1,0 }, buff);
	if(whattotrain == PAPER) ImGui::PopStyleColor();

	ImGui::Separator();

	const char* names[3] = { "ROCK", "SCIZZOR", "PAPER" };
	
	give_orders(*board, *units, allorders, player);
	ImGui::Separator();
	ImGui::SliderInt("Wait till next turn", &wait, 0, 5000);
	ImGui::ProgressBar((float)iterations / (float)wait, {-1,0}, "Next turn");
	if(ImGui::Button("Next turn now")) iterations = wait;
	
	ImGui::Separator();
	for(auto& orderlist : allorders)
	{
		const Unit &unit = *orderlist.first;
		orderlist.second.resize(1); //ONLY ONE ORDER PER UNIT
		Order &order = orderlist.second[0];
		ImGui::PushID(unit.id);
		ImGui::Text("%s, pos = (%d,%d), id = %d",
					names[unit.type], unit.pos.x, unit.pos.y, unit.id);
		ImGui::SliderInt2("Target", &order.target.x, 0, (*board).size(), "%d");
		if(ImGui::SmallButton("Home base ")) order.target = Position{ 0,0 };
		ImGui::SameLine();
		if(ImGui::SmallButton("Enemy base")) order.target = Position{ (*board).size(),(*board).size() };
		ImGui::SameLine();
		if(ImGui::SmallButton("Outpost TR")) order.target = Position{ (*board).size(),0 };
		ImGui::SameLine();
		if(ImGui::SmallButton("Outpost BL")) order.target = Position{ 0,(*board).size() };
		ImGui::DragFloat("Sacrifice", &order.sacrifice, 0.05, 0, 1);
		ImGui::PopID();
	}
	ImGui::EndChild();
}