#include "genetic_tactics.h"

GeneticTactics::GeneticTactics(int input, int output, int scope)
{
	n_input = input;
	n_output = output;
	initiate_weights(0.2);
}


//int * GeneticTactics::gpass(int input[]) {
//	forward_pass(gweights, input);
//}

//Weight matrix forward pass
int * GeneticTactics::wpass(int input[]) {
	forward_pass(weights, input);
}

//initiate weightts with some randomness
void GeneticTactics::initiate_weights(float scope)
{
	initiate_abst_weights(weights, scope);
}

//void GeneticTactics::initiate_gweights(float scope)
//{
//	initiate_abst_weights(gweights, scope);
//}

float get_rand(float m, float M)
{
	float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX);
	return r*(M - m) + m;
}

void GeneticTactics::initiate_abst_weights(matrix &lweights, float scope)
{
	for (int i = 0; i < lweights.size(); i++) {
		for (int j = 0; j < lweights[i].size(); j++) {
			lweights[i][j] = get_rand(-scope,scope);
		}
	}
}


std::vector<int> GeneticTactics::forward_pass(const matrix &lweights, const std::vector<int> &input)
{
	assert(lweights.size() == n_output && lweights[0].size() == input.size());
	std::vector<int> output(n_output, 0);
	for (int o = 0; o < output.size(); o++) {
		for (int a = 0; a < input.size(); a++) {
			output[o] += input[a] * lweights[o][a];
		}
	}
	return output;
}

//mutate weights
void GeneticTactics::mutate(float scope)
{
	for (int i = 0; i < weights.size(); i++) {
		for (int j = 0; j < weights[i].size(); j++) {
			weights[i][j] += get_rand(-scope, scope);
		}
	}
}

//cross over with external weight matrix.
void GeneticTactics::cross_over(matrix& genome, float scope)
{
	for (int i = 0; i < n_output; i++) {
		float r = get_rand(0, 1);
		if(r < scope) continue;
		genome[i].swap(weights[i]);
	}
}

//virtual functions:
Command GeneticTactics::step(const Unit & unit, const OrderList & order_list)
{
	//TODO
	return Command();
}
