#pragma once
#include "abstract_strategy.h"
#include "genetic_game.h"

class GeneticStrategy : AbstractStrategy {
public:

	GeneticStrategy();
	void initiate_weights();
	void forward_prop();
	void cross_over(int scope);
	void mutate(int scope);

private:

	matrix weights;


};
