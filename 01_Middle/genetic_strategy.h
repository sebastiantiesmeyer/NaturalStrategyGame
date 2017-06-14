#pragma once
#include "abstract_strategy.h"
#include "genetic_game_master.h"

class GeneticStrategy : public AbstractStrategy {
public:

	GeneticStrategy(int n_input, int n_output, int scope = 0.2);

	void mutate(float scope);

	void cross_over(matrix & genome, float scope);

	UNIT_TYPE train();
	void changeOrders(AllOrders &orders);



	int * wpass(std::vector<int> input);

	void initiate_weights(float scope);

	void initiate_abst_weights(matrix &lweights, float scope);

	std::vector<int> forward_pass(const matrix & lweights, const std::vector<int>& input);


	matrix weights;
	matrix gweights;

private:
	static const int n_output = 5;
	int n_input;

};
