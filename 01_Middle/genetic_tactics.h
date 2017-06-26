#pragma once
#include "abstract_tactics.h"


typedef std::vector<float> strang;
typedef std::vector<strang> matrix;

class GeneticTactics : public AbstractTactic
{
private:
	int n_input = 13;
	int n_inter = 5;
	int n_output = 5;
	float scope = 6.f;
public:

	GeneticTactics();
	~GeneticTactics() {}
	typedef std::vector<float> strang;
	typedef std::vector<strang> matrix;

	std::vector<float> forward_pass(const std::vector<int> &input);

	void initiate_weights(float scope);

	//std::vector<float>  wpass(const matrix &lweights, const std::vector<float> &input);

	void mutate(float scope);
	void cross_over(matrix& genome0, matrix& genome1, float ratio);

	Command step(const Unit &unit, const OrderList &order_list);

	matrix weights0;
	matrix weights1;

private:

	//std::vector<float> w0pass(const std::vector<int> &input);
	//std::vector<float> w1pass(const std::vector<float> &input);

};

