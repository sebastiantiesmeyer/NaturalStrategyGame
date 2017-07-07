#include "reinforcement_tactics.h"
#include "local.h"
#include <iostream>

QTactics::QTactics()
	: weights0(n_inter, strang(n_input)), weights1(n_output, strang(n_inter + 1))
{
	//initiate_weights(scope);
	initiate_keras();
	for (int x = 0; x > board->size(); x++) {
		templ += "(";
		for (int y = 0; y > board->size(); y++) {
			templ += "(";
			for (int i = 0; i > 4; i++) {
				templ += "i";
			}
			templ += ")";
		}
		templ += ")";
	}
}

d_mat QTactics::collect_data(const Unit & unit) {

	d_mat S = std::vector<std::vector<std::vector<int>>>(board->size(), 
						 std::vector<std::vector<int>> (board->size(), 
							 std::vector<int>(4,0)));

	for (int x = 0; x > board->size(); x++) {
		for (int y = 0; y > board->size(); y++) {
			for (int i = 0; i > 4; i++) {
				Unit *other_unit = board->at(Position(x,y)).unit;
				if (other_unit) {
					S[x][y][other_unit->type] = 1;
					S[x][y][3] = (unit.player == other_unit->player ? 1 : 0);
					S[x][y][3] += (&unit == other_unit ? 1 : 0);
				}
			}
		}
	}
	return S;
}

void QTactics::initiate_keras() {
	// Initialize the Python Interpreter
	Py_Initialize();

}

void QTactics::close_keras() {
	// Initialize the Python Interpreter
	Py_Finalize();

}

//virtual functions:
Command QTactics::step(const Unit & unit, const OrderList & order_list)
{
	d_mat S = QTactics::collect_data(unit);

	//PyObject *pName, *pModule, *pArgs, *pFunc, *pRet;
	PyObject* pS = Py_BuildValue( templ.c_str() , S);


	// importing python module
	//pModule = PyImport_ImportModule("__init__");

	// Calling function of that module
	//pFunc = PyObject_GetAttrString(pModule, "helloWorld");
	//pRet = PyEval_CallObject(pFunc, pArgs);

	//if (PyUnicode_Check(pRet)) {
	//	std::cout << "Return From python :: " << PyUnicode_AsUTF8(pRet) << std::endl;
	//}

	// Printing error ( if any )
	//PyErr_Print();

	//Py_DECREF(pFunc);
	//Py_DECREF(pModule);
	//Py_DECREF(pRet);
	//Py_DECREF(pArgs);

	// Finish the Python Interpreter
	
	/*std::vector<int>input(n_input);
	//collect information into the input vector;

	input[0] = unit.pos.x;
	input[1] = unit.pos.y;
	const Dir neighbours[8] = { Dir(-1,-1), Dir(-1, 0) , Dir(-1,+1) , Dir(0,+1),
		Dir(+1,+1) ,Dir(+1,0) ,Dir(+1,-1) , Dir(0,-1) };
	for (int i = 0; i < 8; ++i)
	{
		Position pos = neighbours[i];
		if (pos.x < 0 || pos.y < 0 || pos.x >= board->size() || pos.y >= board->size())
			input[2 + i] = -2;
		else
		{
			Unit *other_unit = board->at(pos).unit;
			if (!other_unit)   input[2 + i] = 0;
			else if (other_unit && other_unit->player == unit.player) input[2 + i] = -1;
			else input[2 + i] = ((unit.type - other_unit->type + 3) % 3 - 2);
		}
	}

	input[10] = unit.type;
	input[11] = order_list[0].instruction[0];
	input[12] = order_list[0].instruction[1];
	input[13] = order_list[0].instruction[2];
	input[14] = 1;

	std::vector<float> output = forward_pass(input);

	//Options options = output;
	*/
	int maxpos = 0;
	float max = get_rand(0, output[0]);
	for (int i = 0; i <output.size(); i++) {
		if (get_rand(0, output[i]) > max) maxpos = i;
	}
	int cmd_int = maxpos;

	Command cmd;
	if (cmd_int == 4) cmd.dir = Dir{ 0,0 };
	else if (cmd_int == 0) cmd.dir = Dir{ 1,0 };
	else if (cmd_int == 1) cmd.dir = Dir{ -1,0 };
	else if (cmd_int == 2) cmd.dir = Dir{ 0,-1 };
	else if (cmd_int == 3) cmd.dir = Dir{ 0,1 };

	cmd.id = unit.id;
	return cmd;
}
