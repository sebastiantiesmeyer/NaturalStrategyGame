#pragma once
#include "abstract_tactics.h"
#include <Python.h>

typedef std::vector<float> strang;
typedef std::vector<strang> matrix;
typedef std::vector < std::vector<std::vector<int>>> d_mat;

class QTactics : public AbstractTactic
{
private:
	int n_input = 15;
	int n_output = 5;
	float alpha = 0.6f;
	int n_inter = (int)(n_input*(1 - alpha) + n_output * alpha);
	float scope = 4.f;
	std::string templ;
	d_mat collect_data(const Unit & unit);

public:
	PyObject Q_Net;
	QTactics();
	
	void initiate_keras();
	void close_keras();
	QTactics(const QTactics& other) = default;
	~QTactics() {}
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

