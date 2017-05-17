#include "game_master.h"
#include "board_viewer.h"
GameMaster::GameMaster()
{
	board.board.resize(game_size*game_size);
	player[0] = new DummyPlayer({ board, units, unit_progress[0], 0 });
	player[1] = new DummyPlayer({ board, units, unit_progress[1], 1 });
	unit_progress[0].progress = { 0,0,0 };
	unit_progress[0].total_time = 10;
	unit_progress[0].our_base_captured = 0;
	unit_progress[0].enemy_base_captured = 0;
	unit_progress[1] = unit_progress[0];
	player[0]->StartTurn();
	player[1]->StartTurn();
}
GameMaster::~GameMaster() { delete player[0]; delete player[1]; }

bool GameMaster::RenderUpdate() //return true when the game is ended
{								// REPLACES MAIN LOOP
	if(game_status != ONGOING) return true;

	if(player_0_done && player_1_done)
	{	//we can apply the rules
		simulate_board();
		player[0]->StartTurn();
		player[1]->StartTurn();
		player_0_done = player_1_done = false;
	}
	if(player[0]->RenderUpdate())
	{
		queue0 = &player[0]->GetCommandQueue();
		player_0_done = true;
	}
	if(player[1]->RenderUpdate())
	{
		queue1 = &player[1]->GetCommandQueue();
		player_1_done = true;
	}
	return false;
}

void GameMaster::Render()
{
	if(ImGui::Begin("Game Master"))
	{
		ImGui::TextColored({ 1,0,0,1 }, "TODO:");
		ImGui::Bullet(); ImGui::TextColored({ 1,0,0,1 }, "Outpost dynamics!");
		ImGui::Bullet(); ImGui::TextColored({ 1,0,0,1 }, "This Render function");
		ImGui::Bullet(); ImGui::TextColored({ 1,0,0,1 }, "Human player");
		ImGui::Bullet(); ImGui::TextColored({ 0,0,1,1 }, "Time measurement");
		view_board(board);
	}
	ImGui::End();
}

void GameMaster::simulate_board()
{
	int i = 0, j = 0; //at each step, randomly choose a player who's next command will be executed
	while(i != queue0->unitcmds.size() && j != queue1->unitcmds.size())
	{
		if(rnd_distribution(rnd_engine) % 2)
			execute_command_for_player(queue0->unitcmds[i++], 0); //note the i++
		else
			execute_command_for_player(queue1->unitcmds[j++], 1); //note the j++
	} //the remaining commands has to be executed as well
	for(; i != queue0->unitcmds.size(); ++i)
		execute_command_for_player(queue0->unitcmds[i], 0);
	for(; j != queue1->unitcmds.size(); ++j)
		execute_command_for_player(queue1->unitcmds[j], 1);

	for(auto &id_unit : units) //we forgot to set the unit movements back to false!
		id_unit.second.moved = false;

	train_for_player(queue0->train, unit_progress[0], 0);
	train_for_player(queue1->train, unit_progress[1], 1);

	bool p1 = did_loose_player(0), p2 = did_loose_player(1);
	if(p1 && p2) game_status = DRAW; //Draw
	else if(p2) game_status = PLAYER1WON; //p2 lost p1 won
	else if(p1) game_status = PLAYER2WON; //vice versa
	if(++cycle > max_cycles) game_status = DRAW;
}

bool GameMaster::did_loose_player(int player)
{
	//int id = board[auto_rotate(Position(0, 0), i)].id; //base of player i
	Unit * unitptr = board(Position(0, 0), player).unit; //pointer to the unit on that position
	if(unitptr == nullptr) return false;
	unit_progress[player].our_base_captured += (int)(unitptr->player != player); //enemy standing on our base
	unit_progress[1 - player].enemy_base_captured = unit_progress[player].our_base_captured;
	return unit_progress[player].our_base_captured > 20; //after 20 cycles player i looses
}

void GameMaster::train_for_player(UNIT_TYPE what_to_train, UnitProgress &unit_progress, int player)
{
	++unit_progress.progress[what_to_train];
	Position pos = Position(0, 0);
	if(board(pos, player).unit != nullptr) return; //cell not empty, do nothing else

	int numoops = (int)(board.op1 == player) + (int)(board.op2 == player);

	if(unit_progress.progress[what_to_train] >= unit_progress.total_time * (4-numoops)/4)
	{
		++largest_id; // new player at 0,0 (or 19,19)
		Unit new_unit;
		new_unit.id = largest_id;
		new_unit.moved = false;
		new_unit.player = player;
		new_unit.type = what_to_train;
		new_unit.pos = pos;
		units.emplace(std::make_pair(largest_id, new_unit));
		board(pos, player).id = largest_id; //updating cell
		board(pos, player).unit = &units.at(largest_id);
		unit_progress.progress[what_to_train] = 0; // reset training
		++unit_progress.total_time; //incresing construction times
	}
}

void GameMaster::kill_unit(Unit &unit, int player)
{
	board(unit.pos, player).id = 0;
	board(unit.pos, player).unit = nullptr;
	units.erase(unit.id);
}
void GameMaster::move_unit(Unit &unit, const Position &newpos, int player)
{
	board(newpos, player).id = board(unit.pos, player).id;
	board(unit.pos, player).id = 0;
	board(newpos, player).unit = board(unit.pos, player).unit;
	board(unit.pos, player).unit = nullptr;
	unit.pos = newpos;
	unit.moved = true; //we forgot this! :)
}

void GameMaster::execute_command_for_player(const Command &command, int player)
{
	Unit &unit = units.at(command.id); //the command commands this unit
	Dir dir = command.dir;
	dir.x = glm::clamp(dir.x, -1, 1); //no cheating
	dir.y = glm::clamp(dir.y, -1, 1);
	//dir = (player == 0 ? dir : -dir); //change direction for other player's perspective
	Position newpos = unit.pos + dir;
	if(!unit.moved						//unit did not move in this cycle
	   && unit.player == player				//cannot move enemy's player!! we forgot this as well
	   && abs(dir.x) + abs(dir.y) == 1	//no diagonal move
	   && 0 <= newpos.x && newpos.x < game_size
	   && 0 <= newpos.y && newpos.y < game_size)
	{ //othervise we dont move
		if(board(unit.pos + dir, player).unit == nullptr) //empty cell
		{
			move_unit(unit, newpos, player);
		}
		else
		{
			//Unit &other_unit = units.at(board[newpos].id); //the unit on the target cell
			Unit &other_unit = *board(newpos, player).unit; //the unit on the target cell
			if(other_unit.player != player) //enemy
			{
				switch((unit.type - other_unit.type) % 3)
				{
				case 0: //both die
					kill_unit(unit, player);
					kill_unit(other_unit, player);
				case 1: //we die
					kill_unit(unit, player);
				case 2: //yeah
					kill_unit(other_unit, player);
					move_unit(unit, newpos, player);
				default:
					break;
				}
			}
		}
	}
}