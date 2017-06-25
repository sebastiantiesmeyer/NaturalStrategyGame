#pragma once
#include "data_types.h"

UNIT_TYPE train(const Units &units, int player)
{	//build stats		R>S>P
	int p0_units[3] = { 0,0,0 };
	int p1_units[3] = { 0,0,0 };
	for(const auto & id_unit : units)
	{
		if(id_unit.first > 0)	++p0_units[id_unit.second.type];
		else					++p1_units[id_unit.second.type];
	}

	float ps[3] = { 1,1,1 };
	if(p0_units[0] * p0_units[1] * p0_units[2] == 0)
	{
		ps[0] = (p0_units[0] == 0 ? 1 : 0);
	}
}