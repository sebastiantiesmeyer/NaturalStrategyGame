#pragma once
#include "abstract_tactics.h"

class GeneticTactics : public AbstractTactic
{
public:
	typedef std::vector<float> strang;
	typedef std::vector<strang> matrix;

	GeneticTactics(Board &const board) : AbstractTactic::AbstractTactic(board) {}

	void mutate(float scope);
	void cross_over(matrix& genome, float scope);
	void init_weights(float scope);
	void init_gweights(float scope);
	Command step(const Unit &unit, const OrderList &order_list);
	Command do_step(const OrderList &order_list);
};

