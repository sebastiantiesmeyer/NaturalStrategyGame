#pragma once
#include "abstract_strategy.h"
#include "genetic_game_master.h"

class GeneticStrategy : public AbstractStrategy {
public:
	GeneticStrategy() = default;

	void initiate_weights();
	void forward_prop();
	void cross_over(int scope);
	void mutate(int scope);
	UNIT_TYPE train();
	void changeOrders(AllOrders &orders);

private:

	matrix weights;


};
