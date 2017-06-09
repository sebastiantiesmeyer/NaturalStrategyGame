#include "cyborg_strategy.h"

void CyborgStrategy::Render()
{
	if(!ImGui::BeginChild("Order editor", { 0,300 }, true))
	{
		ImGui::EndChild();
		return;
	}
	const char* names[3] = { "ROCK", "SCIZZOR", "PAPER" };

	for(auto& orderlist : allorders)
	{
		const Unit &unit = *orderlist.first;
		orderlist.second.resize(1); //ONLY ONE ORDER PER UNIT
		Order &order = orderlist.second[0];
		ImGui::PushID(unit.id);
		ImGui::Text("%s, pos = (%d,%d), id = %d",
					names[unit.type], unit.pos.x, unit.pos.y, unit.id);
		ImGui::SliderInt2("Target", &order.target.x, 0, board.size(), "%d");
		if(ImGui::SmallButton("Home base ")) order.target = Position{ 0,0 };
		ImGui::SameLine();
		if(ImGui::SmallButton("Enemy base")) order.target = Position{ board.size(),board.size() };
		ImGui::SameLine();
		if(ImGui::SmallButton("Outpost TR")) order.target = Position{ board.size(),0 };
		ImGui::SameLine();
		if(ImGui::SmallButton("Outpost BL")) order.target = Position{ 0,board.size() };
		ImGui::DragFloat("Sacrifice", &order.sacrifice, 0.05, 0, 1);
		ImGui::PopID();
	}
	ImGui::EndChild();
}