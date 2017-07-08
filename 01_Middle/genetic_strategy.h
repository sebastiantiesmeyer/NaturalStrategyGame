#pragma once
#include "abstract_strategy.h"
//#include "genetic_game_master.h"

class GeneticStrategy : public AbstractStrategy
{
private:
	int n_output = 6;
	int n_input = 16;
	float scope = 4.f;


public:

	GeneticStrategy();
	GeneticStrategy(const GeneticStrategy& other) = default;
	~GeneticStrategy() {}
	void activate();

	void mutate(float scope);

	void cross_over(matrix & genome, float ratio);

	void Render();

	virtual UNIT_TYPE train();
	virtual void changeOrders(AllOrders &orders);

	void explore();

	std::vector<int> input;
	std::vector<float> output = std::vector<float>(5, 0.0);

	AllOrders allorders;
	bool endturn = false;
	int wait = 100;
	int iterations = 0;
	UNIT_TYPE whattotrain = ROCK;

	void initiate_weights(float scope);
	matrix weights;

private:
	//void initiate_abst_weights(matrix &lweights, int n_input, int n_output, float scope);

	//std::vector<float> forward_pass(const matrix & lweights, const std::vector<int>& input);
	//std::vector<float> wpass(std::vector<int> input);
};
