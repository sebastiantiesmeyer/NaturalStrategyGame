#include "genetic_strategy.h"
#include "genetic_game_master.h"

UNIT_TYPE GeneticStrategy::train()
{
	return UNIT_TYPE(std::min(0,std::max(((int)output[4]),2))); //get the unit type, an int value between 0 and 2
}

void GeneticStrategy::changeOrders(AllOrders &orders)
{
	for (auto &o : orders)
	{
		o.second.resize(1);
		o.second[0].instruction = output.data();
	}
}

void GeneticStrategy::explore()
{

	input.resize(n_input);
	std::fill(input.begin(), input.end(), 0);

	//divided in quarters: [northwest-scissors, northwest-stones,northwest-papers, northeeast-scissors...]
	for (int i = 0; i < board->size(); i++) {
		for(int j = 0; j < board->size(); j++)
		{
			Unit* u = (*board)(Position(i, j), player).unit; //the following looks complicated
			// if u is nullptr then there is no unit there!
			// i/board.size() always zero here! integer division!, try 2*i/board.size(), this can be 0 or 1.
			if(u != nullptr)
			{
				int quarter = 2 * i / board->size() * 2 + 2 * j / board->size();
				input[3 * quarter + u->type] += (u->player == player ? 1 : -1);
			}
				//input[4 * (2 * i / board->size() + (2 * (j / (*board).size()))) + ((u -> type)*((u -> id)==player))]++;
		}
	}
	int index = 12;
	input[index] = (*board).getPlayerAtOutpost(0, player);
	index++;
	input[index] = (*board).getPlayerAtOutpost(1, player);
	index++;
	//is my homebase occupied?
	//Home is (0,0)
	//Enemy base is (board.size()-1, board.size()-1) !!
	if(((*board)(Position(0, 0), player)).unit != nullptr)
		input[index] = (int)((*board)(Position(0, 0), player).unit->player != player);
	index++;
	input[index] = 1;
}

GeneticStrategy::GeneticStrategy(int input, int output, float scope)
{
	n_input = input;
	n_output = output;
	initiate_weights(scope);
}

void GeneticStrategy::activate()
{
	explore();
	output = wpass(input);
}

//int * GeneticTactics::gpass(int input[]) {
//	forward_pass(gweig*hts, input);
//}

//Weight matrix forward pass
std::vector<float> GeneticStrategy::wpass(std::vector<int> input) {
	return forward_pass(weights, input);
}

//initiate weights with some randomness
void GeneticStrategy::initiate_weights(float scope)
{
	initiate_abst_weights(weights, n_input, n_output, scope);
}

//void GeneticTactics::initiate_gweights(float scope)
//{
//	initiate_abst_weights(gweights, scope);
//}

void GeneticStrategy::initiate_abst_weights(matrix &lweights, int height, int width, float scope)
{
	lweights.resize(n_output);
	for (int i = 0; i < lweights.size(); i++) {
		lweights[i].resize(n_input);
		for (int j = 0; j < lweights[i].size(); j++) {
			lweights[i][j] = get_rand(-scope, scope);
		}
	}
}

std::vector<float> GeneticStrategy::forward_pass(const matrix &lweights, const std::vector<int> &input)
{
	assert(lweights.size() == n_output && lweights[0].size() == input.size());
	std::vector<float> output(n_output, 0);
	for (int o = 0; o < output.size(); o++) {
		for (int a = 0; a < input.size(); a++) {
			output[o] += input[a] * lweights[o][a];
		}
	}
	return output;
}

//mutate weights
void GeneticStrategy::mutate(float scope)
{
	for (int i = 0; i < weights.size(); i++) {
		for (int j = 0; j < weights[i].size(); j++) {
			weights[i][j] += get_rand(-scope, scope);
		}
	}
}

//cross over with external weight matrix.
void GeneticStrategy::cross_over(matrix& genome, float ratio)
{
	for (int i = 0; i < n_output; i++) {
		float r = get_rand(0, 1);
		if (r > ratio) continue;
		genome[i].swap(weights[i]);
	}
}
void GeneticStrategy::Render()
{
	char buff[256];

	float current_training_time = (*unit_progress).current_train_time;

	sprintf_s(buff, "%d/%d", (*unit_progress).progress[ROCK], (int)current_training_time);
	ImGui::RadioButton("Rock", (int*)&whattotrain, (int)ROCK); ImGui::SameLine(100);
	if (whattotrain == ROCK) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
	ImGui::ProgressBar((*unit_progress).progress[ROCK] / current_training_time, { -1,0 }, buff);
	if (whattotrain == ROCK) ImGui::PopStyleColor();

	sprintf_s(buff, "%d/%d", (*unit_progress).progress[SCISSOR], (int)current_training_time);
	ImGui::RadioButton("Scissor", (int*)&whattotrain, (int)SCISSOR); ImGui::SameLine(100);
	if (whattotrain == SCISSOR) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
	ImGui::ProgressBar((*unit_progress).progress[SCISSOR] / current_training_time, { -1,0 }, buff);
	if (whattotrain == SCISSOR) ImGui::PopStyleColor();

	sprintf_s(buff, "%d/%d", (*unit_progress).progress[PAPER], (int)current_training_time);
	ImGui::RadioButton("Paper", (int*)&whattotrain, (int)PAPER); ImGui::SameLine(100);
	if (whattotrain == PAPER) ImGui::PushStyleColor(ImGuiCol_PlotHistogram, { 1,0.5,1,1 });
	ImGui::ProgressBar((*unit_progress).progress[PAPER] / current_training_time, { -1,0 }, buff);
	if (whattotrain == PAPER) ImGui::PopStyleColor();

	ImGui::Separator();

	const char* names[3] = { "ROCK", "SCIZZOR", "PAPER" };

	//give_orders(*board, *units, allorders, player);

	if (!ImGui::BeginChild("Order editor", { 0,300 }, true))
	{
		ImGui::EndChild();
		return;
	}

	ImGui::Separator();
	ImGui::SliderInt("Wait till next turn", &wait, 0, 5000);
	ImGui::ProgressBar((float)iterations / (float)wait, { -1,0 }, "Next turn");
	if (ImGui::Button("Next turn now")) iterations = wait;

	ImGui::Separator();
	for (auto& orderlist : allorders)
	{
		if (!units->count(orderlist.first)) continue;
		const Unit & unit = units->at(orderlist.first);
		orderlist.second.resize(1);		//ONLY ONE ORDER PER UNIT
		Order &order = orderlist.second[0];
		ImGui::PushID(unit.id);
		ImGui::Text("%s, pos = (%d,%d), id = %d",
			names[unit.type], unit.pos.x, unit.pos.y, unit.id);
		ImGui::SliderInt2("Target", &order.target.x, 0, board->size(), "%f");
		ImGui::Text("(%d, %d)", order.target.x, order.target.y);
		if (ImGui::SmallButton("Home base ")) order.target = Position{ 0,0 };
		ImGui::SameLine();
		if (ImGui::SmallButton("Enemy base")) order.target = Position{ board->size(),board->size() };
		ImGui::SameLine();
		if (ImGui::SmallButton("Outpost TR")) order.target = Position{ board->size(),0 };
		ImGui::SameLine();
		if (ImGui::SmallButton("Outpost BL")) order.target = Position{ 0, board->size() };
		ImGui::DragFloat("Sacrifice", &order.sacrifice, 0.05, 0, 1);
		ImGui::PopID();
	}
	ImGui::EndChild();
}
