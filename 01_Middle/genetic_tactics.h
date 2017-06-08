#pragma once
#include "abstract_tactics.h"

class GeneticTactics : public AbstractTactic
{
	typedef std::vector<float> strang;
	typedef std::vector<strang> matrix;

	//GeneticTactics() : AbstractTactic::AbstractTactic() {}

	void mutate(float scope);
	void cross_over(matrix& genome, float scope);
	void init_weights(float scope);
	void init_gweights(float scope);
	Command do_step(const OrderList &order_list);
};

