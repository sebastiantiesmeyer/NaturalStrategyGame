#pragma once
#include "abstract_tactics.h"


typedef std::vector<float> strang;
typedef std::vector<strang> matrix;

class GeneticTactics : public AbstractTactic
{
public:

	GeneticTactics(int n_input=13, int n_output = 5, float scope = 0.2);
	~GeneticTactics()
	{
		std::cout << "~GeneticTactics()" << std::endl;
	}
	typedef std::vector<float> strang;
	typedef std::vector<strang> matrix;

	std::vector<float> wpass(std::vector<int> input);

	void initiate_weights(float scope);
	void GeneticTactics::initiate_w0(float scope);
	void GeneticTactics::initiate_w1(float scope);

	std::vector<float>  forward_pass(const matrix &lweights, const std::vector<int> &input);

	void mutate(float scope);
	void cross_over(matrix& genome0, matrix& genome1, float scope);

	Command step(const Unit &unit, const OrderList &order_list);

	matrix weights0;
	matrix weights1;

private:


	int n_input;
	int n_inter;
	int n_output;
	void initiate_abst_weights(matrix &lweights, int height, int width, float scope);

};

