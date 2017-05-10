#pragma once
#include "imgui\imgui.h"
#include <vector>
#include <algorithm>

void drawBoard() //Example For Using imgui for our purposes
{
	ImGui::Text("This function works like printf. FPS = %f", ImGui::GetIO().Framerate);
	static std::vector<std::string> vec(20 * 20, "0"); //note the 'static' here
	float size = std::max(std::min(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y+110),400.f);
	float button_size = size /20-8;
	ImGui::ProgressBar(ImGui::GetIO().Framerate / 300.0);
	for(int i = 0; i < 20; ++i)
	{
		ImGui::PushID(i);		// If two objects have the same label, the imgui doesnt know which one you clicked on
		for(int j = 0; j < 20; ++j)
		{
			ImGui::PushID(j);	// For this reason, you can PushID-s onto the ID-Stack.
			char &c = vec[20 * j + i][0];
			float color = (c - '0') /40.0+0.1; //Colors have a different stack, and you can push colors onto it
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(color*i, color*5, color*j, 0.5)); // colors are in range [0,1]
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color*i, color*5, color*j, 0.75)); //RGBA colors
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color*i, color*5, color * j, 1.0));
			if(ImGui::Button(vec[20 * j + i].data(), ImVec2(button_size, button_size)))
				++c;		//Clickebales return true when clicked with left mouse button
			if(ImGui::IsItemClicked(1))
				--c;		//check if last object was right clicked (0=left,1=right,2=middle)
			ImGui::PopStyleColor(3); //Pop 3 colors at once from the color stack
			ImGui::PopID();	//Have to pop button id
			ImGui::SameLine(); //Next Button appears on the same line
		}
		ImGui::PopID();		//Have to pop row id
		ImGui::NewLine();	   //Next row below
	}
}