#include "gene_master.h"
#include "board_viewer.h"
#include "genetic_player.h"
GeneMaster::GeneMaster()
{
	board.board.resize(game_size*game_size);
	player[0] = new GeneticPlayer({ board, units, unit_progress[0], 0 });
	player[1] = new GeneticPlayer({ board, units, unit_progress[1], 1 });
	player[0]->init_gweights;
	player[1]->init_weights;
	player[0]->init_gweights;
	player[1]->init_weights;
	player[0]->StartTurn();
	player[1]->StartTurn();
}
GeneMaster::~GeneMaster() { delete player[0]; delete player[1]; }

bool GeneMaster::RenderUpdate() //return true when the game is ended
{								// REPLACES MAIN LOOP
	if (game_status != ONGOING) return true;

	if (player_0_done && player_1_done)
	{	//we can apply the rules
		simulate_board();
		player[0]->StartTurn();
		player[1]->StartTurn();
		player_0_done = player_1_done = false;
	}
	if (player[0]->RenderUpdate())
	{
		queue0 = &player[0]->GetCommandQueue();
		player_0_done = true;
	}
	if (player[1]->RenderUpdate())
	{
		queue1 = &player[1]->GetCommandQueue();
		player_1_done = true;
	}
	return false;
}

void GeneMaster::Render()
{
	if (ImGui::Begin("Game Master"))
	{
		ImGui::TextColored({ 1,0,0,1 }, "TODO:");
		ImGui::Bullet(); ImGui::TextColored({ 1,0,0,1 }, "Fast end game when no more players!");
		ImGui::Bullet(); ImGui::TextColored({ 1,0,0,1 }, "This Render function");
		ImGui::Bullet(); ImGui::TextColored({ 0,0,1,1 }, "Time measurement");
		view_board(board);
	}
	ImGui::End();
}

void GeneMaster::training_match()
{
	
}

void GeneMaster::simulate_board()
	{
	int i = 0, j = 0; //at each step, randomly choose a player who's next command will be executed
	while (i != queue0->unitcmds.size() && j != queue1->unitcmds.size())
	{
		if (rnd_distribution(rnd_engine) % 2)
			execute_command_for_player(queue0->unitcmds[i++], 0); //note the i++
		else
			execute_command_for_player(queue1->unitcmds[j++], 1); //note the j++
	} //the remaining commands has to be executed as well
	for (; i != queue0->unitcmds.size(); ++i)
		execute_command_for_player(queue0->unitcmds[i], 0);
	for (; j != queue1->unitcmds.size(); ++j)
		execute_command_for_player(queue1->unitcmds[j], 1);

	for (auto &id_unit : units) //we forgot to set the unit movements back to false!
		id_unit.second.moved = false;

	//train_for_player(queue0->train, unit_progress[0], 0);
	//train_for_player(queue1->train, unit_progress[1], 1);
	set_outpost_ownership();

	int loseint = 2;
	bool p1 = did_loose_player(0), p2 = did_loose_player(1);
	if (p1 && p2) game_status = DRAW; //Draw
	else if (p2) {
		game_status = PLAYER1WON; loseint = 0;
	} //p2 lost p1 won
	else if (p1) { game_status = PLAYER2WON; loseint = 1; }//vice versa
	if (++cycle > max_cycles) game_status = DRAW;
	
	//do the genetic mutations, cross-overs...
	if (loseint<2) {
		player[loseint] -> mutate(0.2);
	}
}

bool GeneMaster::did_loose_player(int player)
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
/*
void GeneMaster::train_for_player(UNIT_TYPE what_to_train, UnitProgress &unit_progress, int player)
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
}*/

void GeneMaster::set_outpost_ownership()
{
	Position p01 = Position(0, game_size - 1);
	if (board.at(p01).unit != nullptr)
		board.outposts[0] = board.at(p01).unit->player;
	Position p10 = Position(game_size - 1, 0);
	if (board.at(p10).unit != nullptr)
		board.outposts[1] = board.at(p10).unit->player;
}

void GeneMaster::kill_unit(Unit &unit)
{
	board(unit.pos, unit.player).id = 0;
	board(unit.pos, unit.player).unit = nullptr;
	units.erase(unit.id);
}
void GeneMaster::move_unit(Unit &unit, const Position &newpos)
{
	if (board(newpos, unit.player).unit != nullptr) throw std::exception("Not an empty cell!");
	board(newpos, unit.player).id = board(unit.pos, unit.player).id;
	board(unit.pos, unit.player).id = 0;
	board(newpos, unit.player).unit = board(unit.pos, unit.player).unit;
	board(unit.pos, unit.player).unit = nullptr;
	unit.pos = newpos;
	unit.moved = true;
}

void GeneMaster::execute_command_for_player(const Command &command, int player)
{
	if (units.count(command.id) == 0) return;
	Unit &unit = units.at(command.id); //the command commands this unit
	Dir dir = command.dir;
	dir.x = glm::clamp(dir.x, -1, 1); //no cheating
	dir.y = glm::clamp(dir.y, -1, 1);
	//dir = (player == 0 ? dir : -dir); //change direction for other player's perspective
	Position newpos = unit.pos + dir;
	if (!unit.moved						//unit did not move in this cycle
		&& unit.player == player				//cannot move enemy's player!! we forgot this as well
		&& norm1(dir) == 1	//no diagonal move
		&& 0 <= newpos.x && newpos.x < game_size
		&& 0 <= newpos.y && newpos.y < game_size)
	{ //othervise we dont move
		if (board(newpos, player).unit == nullptr) //empty cell
		{
			move_unit(unit, newpos);
		}
		else
		{
			Unit &other_unit = *board(newpos, player).unit; //the unit on the target cell
			if (other_unit.player != player) //enemy
			{
				switch ((unit.type - other_unit.type + 3) % 3)
				{
				case 0: //both die
					kill_unit(unit);
					kill_unit(other_unit);
					break;
				case 1: //we die
					kill_unit(unit);
					break;
				case 2: //yeah
					kill_unit(other_unit);
					move_unit(unit, newpos);
					break;
				default:					break;
				}
			}
		}
	}
}