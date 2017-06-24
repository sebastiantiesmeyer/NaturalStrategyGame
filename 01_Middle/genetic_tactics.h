#pragma once
#include "abstract_tactics.h"


typedef std::vector<float> strang;
typedef std::vector<strang> matrix;

class GeneticTactics : public AbstractTactic
{
public:

	GeneticTactics(int n_input=13, int n_output = 5, int scope = 0.2);

	typedef std::vector<float> strang;
	typedef std::vector<strang> matrix;

	std::vector<int> wpass(std::vector<int> input);

	void initiate_weights(float scope);

	std::vector<int>  forward_pass(const matrix &lweights, const std::vector<int> &input);

	void mutate(float scope);
	void cross_over(matrix& genome, float scope);

	Command step(const Unit &unit, const OrderList &order_list);

	matrix weights;

private:

	int n_input;
	int n_output;
	void initiate_abst_weights(matrix &lweights, int height, int width, float scope);

};

