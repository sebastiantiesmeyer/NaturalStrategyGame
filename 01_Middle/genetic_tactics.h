#pragma once
#include "abstract_tactics.h"

typedef std::vector<float> strang;
typedef std::vector<strang> matrix;

class GeneticTactics : public AbstractTactic
{
public:
	//GeneticTactics() = default;
	GeneticTactics(Board  &const b, int n_input, int n_output);//AbstractTactic(Board  &const b);
	typedef std::vector<float> strang;
	typedef std::vector<strang> matrix;

	int * gpass(int input[]);

	int * wpass(int input[]);

	void initiate_weights(float scope);

	void initiate_gweights(float scope);


	int * forward_pass(matrix lweights, int input[]);

	void mutate(float scope);
	void cross_over(matrix& genome, float scope);

	Command step(const Unit &unit, const OrderList &order_list);
	Command do_step(const OrderList &order_list);
	matrix weights;
	matrix gweights;

private:


	void initiate_abst_weights(matrix lweights, float scope);

};

