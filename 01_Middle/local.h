#pragma once
#include "data_types.h"
#include <array>

struct Options
{
	union //so we can iterate through up, left, still, right down
	{
		struct { float up, left, still, right, down; };
		float data[5];
	};
	Dir choose(); //implemented in local.cpp
	inline void normalize()
	{
		float sum = 0;
		for(int i = 0; i < 5; ++i)	sum += data[i];
		for(int i = 0; i < 5; ++i)	data[i] /= sum;
	}
};

void localNxN(const Unit& unit, const Board &board, Options &options);
void localAvoid3x3(const Unit& unit, const Board &board, Options &options);
