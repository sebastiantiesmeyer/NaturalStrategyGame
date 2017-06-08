#include "official_game.h"

bool OfficialGame::did_loose_player(int player)
{
	//int id = board[auto_rotate(Position(0, 0), i)].id; //base of player i
	Unit * unitptr = board(Position(0, 0), player).unit; //pointer to the unit on that position
	if (unitptr == nullptr || unitptr->player == player)
	{
		unit_progress[player].our_base_captured = 0; //enemy standing on our base
		unit_progress[1 - player].enemy_base_captured = 0;
		return false;
	}
	++unit_progress[player].our_base_captured; //enemy standing on our base
	unit_progress[1 - player].enemy_base_captured = unit_progress[player].our_base_captured;
	return unit_progress[player].our_base_captured > game_size;
}

void OfficialGame::train_for_player(UNIT_TYPE what_to_train, UnitProgress &unit_progress, int player)
{
	const float multipiers[3] = { 1.0, 0.75, 0.5 }; //training time modifiers

	++unit_progress.progress[what_to_train];
	Position pos = Position(0, 0);
	if (board(pos, player).unit != nullptr) return; //cell not empty, do nothing else

	int numoops = (int)(board.outposts[0] == player) + (int)(board.outposts[1] == player);
	unit_progress.current_train_time = (int)(unit_progress.total_time * multipiers[numoops]);

	if (unit_progress.progress[what_to_train] >= unit_progress.current_train_time)
	{
		largest_id[player] += (player == 0 ? 1 : -1); // new player at 0,0 (or 19,19)
		Unit new_unit;
		new_unit.id = largest_id[player];
		new_unit.moved = false;
		new_unit.player = player;
		new_unit.type = what_to_train;
		new_unit.pos = pos;
		units.emplace(std::make_pair(largest_id[player], new_unit));
		board(pos, player).id = largest_id[player]; //updating cell
		board(pos, player).unit = &units.at(largest_id[player]);
		unit_progress.progress[what_to_train] = 0; // reset training
		++unit_progress.total_time; //incresing construction times
	}
}

void OfficialGame::set_outpost_ownership()
{
	Position p01 = Position(0, game_size - 1);
	if (board.at(p01).unit != nullptr)
		board.outposts[0] = board.at(p01).unit->player;
	Position p10 = Position(game_size - 1, 0);
	if (board.at(p10).unit != nullptr)
		board.outposts[1] = board.at(p10).unit->player;
}

void OfficialGame::extra_rules()
{
	largest_id[0] += 1; 
	largest_id[1] += -1;
	Unit new_unit0, new_unit1;
	new_unit0.id = largest_id[0];
	new_unit1.id = largest_id[1];
	new_unit0.moved = false;
	new_unit1.moved = false;
	new_unit0.player = 0;
	new_unit1.player = 1;
	new_unit0.type = ROCK;
	new_unit1.type = SCISSOR;
	new_unit0.pos = Position(0, 0);
	new_unit1.pos = Position(0, 0);
	units.emplace(std::make_pair(largest_id[0], new_unit0));
	units.emplace(std::make_pair(largest_id[1], new_unit1));
	board(Position(0, 0), 0).id = largest_id[0]; //updating cell
	board(Position(0,0), 1).id = largest_id[1]; //updating cell
	board(Position(0, 0), 0).unit = &units.at(largest_id[0]);
	board(Position(0,0), 1).unit = &units.at(largest_id[1]);
	unit_progress.progress[what_to_train] = 0; // reset training
	++unit_progress.total_time; //incresing construction times
}