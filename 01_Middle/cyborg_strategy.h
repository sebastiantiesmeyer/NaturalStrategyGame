#pragma once
#include "abstract_strategy.h"
#include "super_player.h"
#include "imgui\imgui.h"

//A human-controlled global strategy, ideal for testing tactics
// Behaviour is similar to real-time strategy games.
class CyborgStrategy : public AbstractStrategy
{
public:
	/*virtual*/ void changeOrders(AllOrders &newallorders)
	{ //just like startTurn;
		allorders = newallorders;
		endturn = false;
		iterations = 0;
	}
	/*virtual*/ bool Update() //return true when the turn is ended
	{
		++iterations;
		return iterations > wait;
	}
	/*virtual*/ void Render(); //implemented in cpp
public:
	AllOrders &allorders;
	bool endturn = false;
	int wait = 100;
	int iterations = 0;
};