#include "official_game.h"
#include <iostream>

void OfficialGame::extra_rules()
{
	train_for_player(queue0->train, *unit_progress[0], 0);
	train_for_player(queue1->train, *unit_progress[1], 1);
	set_outpost_ownership();

	bool p1 = did_loose_player(0), p2 = did_loose_player(1);
	if(p1 && p2) score = glm::dvec2(0.4, 0.4); //Draw
	else if(p2) score = glm::dvec2(1.0, 0.0); //p2 lost p1 won
	else if(p1) score = glm::dvec2(0.0, 1.0); //vice versa
	if(++cycle > max_cycles) score = glm::dvec2(0.9, 0.9);
}

glm::dvec2 OfficialGame::getPlayerScore() const
{
	return score;
}

double OfficialGame::get_secondary_score(int player) const
{
	Units::const_iterator mid = units->lower_bound(0); // negative = player 1 AND positive player 0

	int typecnt[3] = { 0,0,0 };
	Position sumpos = Position(0);
	for(auto it = (player == 0 ? mid : units->cbegin());
		it != (player == 0 ? units->end() : mid); ++it)
	{
		++typecnt[it->second.type];
		sumpos += it->second.pos;
	}
	float unitcnt = typecnt[0] + typecnt[1] + typecnt[2] + 0.5f;
	const float S_unit_ratio = unitcnt / (float)(units->size() + 0.5f);
	
	float udf0 = ((float)typecnt[0] / unitcnt - 1.f / 3.f);
	float udf1 = ((float)typecnt[1] / unitcnt - 1.f / 3.f);
	float udf2 = ((float)typecnt[2] / unitcnt - 1.f / 3.f);
	const float S_unit_type_div = (udf0*udf0 + udf1*udf1 + udf2*udf2)*3.f / 2.f; //0-1

	int numoops = 0;
	numoops += (int)(board->getPlayerAtOutpost(0, player) == player);
	numoops += (int)(board->getPlayerAtOutpost(1, player) == player);
	const float S_outpost_cnt = numoops/2.f;

	const float S_avg_dist = ((float) sumpos.x / unitcnt + (float)sumpos.y / unitcnt) / (float)board->size()/2.f;
	const float S_base_captured = unit_progress[player]->enemy_base_captured;

/*PARAMETERS TO SET*/
	const float W_unit_ratio	= 3.0f;
	const float W_unit_type_div	= 1.1f;
	const float W_outpost_cnt	= 4.0f;
	const float W_avg_dist		= 0.2f;
	const float W_base_captured	= 5.0f;

	const double W_final		= 0.16;
	const double M_max_final	= 0.13;
/*=================*/

	double weighted_score_sum = W_unit_ratio * S_unit_ratio + W_unit_type_div * S_unit_type_div
		+ W_outpost_cnt * S_outpost_cnt + W_avg_dist * S_avg_dist + W_base_captured * S_base_captured;
	double sum_of_weights = W_unit_ratio + W_unit_type_div + W_outpost_cnt + W_avg_dist + W_base_captured;

	const double W_score = weighted_score_sum / sum_of_weights; //0-1

	return std::min(W_score*W_final,M_max_final);
}

bool OfficialGame::did_loose_player(int player)
{
	//int id = board[auto_rotate(Position(0, 0), i)].id; //base of player i
	Unit * unitptr = board->operator()(Position(0, 0), player).unit; //pointer to the unit on that position
	if (unitptr == nullptr || unitptr->player == player)
	{
		unit_progress[player]->our_base_captured = 0; //enemy standing on our base
		unit_progress[1 - player]->enemy_base_captured = 0;
		return false;
	}
	++unit_progress[player]->our_base_captured; //enemy standing on our base
	unit_progress[1 - player]->enemy_base_captured = unit_progress[player]->our_base_captured;
	return unit_progress[player]->our_base_captured > board->size();
}

void OfficialGame::train_for_player(UNIT_TYPE what_to_train, UnitProgress &unit_progress, int player)
{
	const float multipiers[3] = { 1.0, 0.75, 0.5 }; //training time modifiers

	++unit_progress.progress[what_to_train];
	Position pos = Position(0, 0);
	if (board->operator()(pos, player).unit != nullptr) return; //cell not empty, do nothing else
	int numoops = 0;
	numoops += (int)(board->getPlayerAtOutpost(0, player) == player);
	numoops += (int)(board->getPlayerAtOutpost(1, player) == player);
	//int numoops = (int)(board.outposts[0] == player) + (int)(board.outposts[1] == player);
	unit_progress.current_train_time = (int)(unit_progress.total_time * multipiers[numoops]);

	if (unit_progress.progress[what_to_train] >= unit_progress.current_train_time)
	{
		/*largest_id[player] += (player == 0 ? 1 : -1); // new player at 0,0 (or 19,19)
		Unit new_unit;
		new_unit.id = largest_id[player];
		new_unit.moved = false;
		new_unit.player = player;
		new_unit.type = what_to_train;
		new_unit.pos = pos;
		units.emplace(std::make_pair(largest_id[player], new_unit));
		board(pos, player).id = largest_id[player]; //updating cell
		board(pos, player).unit = &units.at(largest_id[player]);*/
		create_unit(pos, player, what_to_train); //NEW
		//std::cout << "You created a " << what_to_train << ".\n";
		unit_progress.progress[what_to_train] = 0; // reset training
		++unit_progress.total_time; //incresing construction times
	}
}

void OfficialGame::set_outpost_ownership()
{
	Position p01 = Position(0, board->size() - 1);
	if (board->at(p01).unit != nullptr)
		board->outposts[0] = board->at(p01).unit->player;
	Position p10 = Position(board->size() - 1, 0);
	if (board->at(p10).unit != nullptr)
		board->outposts[1] = board->at(p10).unit->player;
}