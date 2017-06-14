#pragma once
#include "abstract_strategy.h"
#include "genetic_game_master.h"

class GeneticStrategy : public AbstractStrategy {
public:

	void mutate(float scope);
	int * forward_pass(matrix lweights, int input[]);

	void cross_over(matrix & genome, float scope);

	UNIT_TYPE train();
	void changeOrders(AllOrders &orders);

	void initiate_weights(float scope);

	void initiate_gweights(float scope);

	void initiate_abst_weights(matrix lweights, float scope);

	int * gpass(int input[]);

	int * wpass(int input[]);

	matrix weights;
	matrix gweights;

private:
	static const int n_output = 5;


};