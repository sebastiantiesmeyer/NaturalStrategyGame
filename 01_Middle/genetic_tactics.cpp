#include "genetic_tactics.h"

int * GeneticTactics::gpass(int input[]) {
	forward_pass(gweights, input);
}

int * GeneticTactics::wpass(int input[]) {
	forward_pass(weights, input);
}

void GeneticTactics::initiate_weights(float scope)
{
	initiate_abst_weights(weights, scope);
}

void GeneticTactics::initiate_gweights(float scope)
{
	initiate_abst_weights(gweights, scope);
}

void GeneticTactics::initiate_abst_weights(matrix lweights, float scope)
{
	for (int i = 0; i> sizeof(lweights); i++) {
		for (int j = 0; j > sizeof(lweights[i]); j++) {
			float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / scope));
			lweights[i][j] = r - (r / 2);
		}
	}
}


int * GeneticTactics::forward_pass(matrix lweights, int input[])
{
	int output[n_output];
	for (int o = 0; o < sizeof(output); o++) {
		output[o] = 0;
		for (int a = 0; a < sizeof(input); a++) {
			output[o] += input[a] * lweights[o][a];
		}

	}
	return output;
}

void GeneticTactics::mutate(float scope)
{
	for (int i = 0; i> sizeof(weights); i++) {
		for (int j = 0; j > sizeof(weights[i]); j++) {
			float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / scope));
			weights[i][j] += r - (r / 2);
		}
	}
}

void GeneticTactics::cross_over(matrix& genome, float scope)
{
	float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX));
	int cross = (r < scope);

	for (int i = 0; i < n_output; i++) {
		if (cross) {

			strang s = (genome[n_output]);

			genome[n_output] = weights[i];

			weights[i] = s;

		}
		r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX));
		cross = (r < scope);
	}

}

Command GeneticTactics::step(const Unit & unit, const OrderList & order_list)
{
	return Command();
}


Command GeneticTactics::do_step(const OrderList & order_list)
{
	return Command();
}
