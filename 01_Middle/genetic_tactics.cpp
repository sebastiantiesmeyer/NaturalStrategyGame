#include "genetic_tactics.h"
#include "local.h"
#include <iostream>

GeneticTactics::GeneticTactics()
	: weights0(n_input, strang(n_inter)), weights1(n_inter, strang(n_output))
{
	initiate_weights(scope);
}

/*
//Weight matrix forward pass
std::vector<float> GeneticTactics::w0pass(const std::vector<int> &input) {
	std::vector<float> converted_input(input.begin(), input.end());
	return wpass(weights0, converted_input);
}

//Weight matrix forward pass
std::vector<float> GeneticTactics::w1pass(const std::vector<float> &input) {
	return wpass(weights1, input);
}*/

inline strang matvecmul(const matrix &M, const strang &x)
{
	strang y(M.size(), 0.f);
	assert(M[0].size() == x.size());
	for(auto i = 0; i < y.size(); ++i)
	{
		for(auto j = 0; j < M[i].size(); ++j)
		{
			y[i] += M[i][j] * x[j];
		}
	}
	return y;
}

std::vector<float> GeneticTactics::forward_pass(const std::vector<int> &input)
{
	//std::vector<float> inter = w0pass(input);
	std::vector<float> float_input(input.begin(), input.end());
	return matvecmul(weights1, matvecmul(weights0, float_input));
}

void GeneticTactics::initiate_weights(float scope) {

	for(auto &row : weights0)for(auto &elm : row)
			elm = get_rand(-scope, scope);
	for(auto &row : weights1)for(auto &elm : row)
			elm = get_rand(-scope, scope);
}


/*
std::vector<float> GeneticTactics::wpass(const matrix &lweights, const std::vector<float> &input)
{
	//assert(lweights.size() == n_output && lweights[0].size() == input.size());
	std::vector<float> output(lweights.size(), 0);
	for (int o = 0; o < output.size(); o++) {
		for (int a = 0; a < input.size(); a++) {
			output[o] += input[a] * lweights[o][a];
		}
	}
	return output;
}*/

//mutate weights
void GeneticTactics::mutate(float scope)
{
	for (int i = 0; i < weights0.size(); i++) {
		for (int j = 0; j < weights0[i].size(); j++) {
			weights0[i][j] += get_rand(-scope, scope);
		}
	}
	for (int i = 0; i < weights1.size(); i++) {
		for (int j = 0; j < weights1[i].size(); j++) {
			weights1[i][j] += get_rand(-scope, scope);
		}
	}

}

//cross over with external weight matrix.
void GeneticTactics::cross_over(matrix& genome0, matrix& genome1, float ratio)
{
	for (int i = 0; i < n_inter; i++) {
		float r = get_rand(0, 1);
		if (r > ratio) continue;
		genome0[i].swap(weights0[i]);
	}
	for (int i = 0; i < n_output; i++) {
		float r = get_rand(0, 1);
		if (r > ratio) continue;
		genome1[i].swap(weights1[i]);
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
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) {
				//own position
				input[index] = unit.pos.x;
				index++;
				input[index] = unit.pos.y;
				index++;
			}
			else {
				//rule out off-board commands
				Position pos = Position(unit.pos.x - i, unit.pos.y - j);
				if(pos.x < 0 || pos.y < 0 || pos.x >= board->size() || pos.y >= board->size())
				{
					input[index] = -2;
				}
				else {
					Unit *other_unit = (board-> at(Position(unit.pos.x - i, unit.pos.y - j))).unit;
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
	for (auto order : order_list) {
		input[index] = order_list[0].instruction[0];
	}
	index++;
	input[index] = 1;
	index++;



	std::vector<float> output = forward_pass(input);

	//Options options = output;

	int maxpos = 0;
	float max = output[0];
	for (int i = 0; i <output.size(); i++) {
		if (output[i] > max) maxpos = i;
	}
	int cmd_int = maxpos;

	Command cmd;
	if (cmd_int == 4) cmd.dir = Dir{ 0,0 };
	else if (cmd_int == 0) cmd.dir = Dir{ 1,0 };
	else if (cmd_int == 1) cmd.dir = Dir{ -1,0 };
	else if (cmd_int == 2) cmd.dir = Dir{ 0,-1 };
	else if (cmd_int == 3) cmd.dir = Dir{ 0,1 };

		//[0] = (cmd_int % 2) * 2 - 1;
		//cmd.dir[1] = (cmd_int / 2) * 2 - 1;
	
	cmd.id = unit.id;
	return cmd;
}
