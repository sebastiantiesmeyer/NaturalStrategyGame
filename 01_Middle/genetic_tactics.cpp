#include "genetic_tactics.h"
#include "local.h"

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
std::vector<int> GeneticTactics::wpass(std::vector<int> input) {
	return forward_pass(weights,  input);
}

//initiate weightts with some randomness
void GeneticTactics::initiate_weights(float scope)
{

	initiate_abst_weights(weights, n_input, n_output, scope);
}

//void GeneticTactics::initiate_gweights(float scope)
//{
//	initiate_abst_weights(gweights, scope);
//}

void GeneticTactics::initiate_abst_weights(matrix &lweights, int height, int width, float scope)
{
	lweights.resize(n_output);
	for (int i = 0; i < lweights.size(); i++) {
		lweights[i].resize(n_input);
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
	Options options;
	std::vector<int>input(n_input);
	//collect information into the input vector;

	int index = 0;
	//surrounding squares:
	for (int i = -1; i < 1; i++) {
		for (int j = -1; j < 1; j++) {
			if (i == 0 && j == 0) {
				//own position
				input[index] = unit.pos.x;
				index++;
				input[index] = unit.pos.y;
				index++;
			}
			else {
				//rule out off-board commands
				if ((std::min(unit.pos.x - i, unit.pos.y - j) < 0) ||
					(std::max(unit.pos.x - +i, unit.pos.y + j) == ((*board).size()))) {
					input[index] = -2;
				}
				else {
					Unit *other_unit = (*board)(glm::ivec2(unit.pos.x - i, unit.pos.y - j), unit.player).unit;
					if (!other_unit)   input[index] = 0;
					else if (other_unit && other_unit->player == unit.player) input[index] = -1;
					else input[index] = (unit.type - other_unit->type) % 3 + 2;
					index++;
				}

			}

		}

	}

	input[index] = unit.type;
	index++;
	input[index] = 1;
	index++;
	input[index] = 1;
	index++;



	std::vector<int> output = wpass(input);

	//Options options = output;

	int maxpos = 0;
	int max = output[0];
	for (int i = 0; i < sizeof(output); i++) {
		if (output[i] < max) maxpos = i;
	}
	int cmd_int = output[max];

	Command cmd;
	if (cmd_int == 4) cmd.dir = Dir{ 0,0 };
	else {
		cmd.dir[0] = (cmd_int % 2) * 2 - 1;
		cmd.dir[1] = (cmd_int / 2) * 2 - 1;
		
	}

	return Command();
}
