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
	for (int i = 0; i < (*board).size(); i++) {
		for (int j = 0; j < (*board).size(); j++) {
			Unit* u = (*board)(Position(i,j),player).unit; //the following looks complicated
			// if u is nullptr then there is no unit there!
			// i/board.size() always zero here! integer division!, try 2*i/board.size(), this can be 0 or 1.
			if (u != nullptr)
				input[4 * (2 * i / (*board).size() + (2 * (j / (*board).size()))) + ((u -> type)*((u -> id)==player))]++;
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
	initiate_weights(0.2);
}

void GeneticStrategy::activate()
{
	explore();
	output = wpass(input);
}

//int * GeneticTactics::gpass(int input[]) {
//	forward_pass(gweights, input);
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
void GeneticStrategy::cross_over(matrix& genome, float scope)
{
	for (int i = 0; i < n_output; i++) {
		float r = get_rand(0, 1);
		if (r < scope) continue;
		genome[i].swap(weights[i]);
	}
}


