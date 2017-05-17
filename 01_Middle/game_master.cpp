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
		view_board(board, 0);
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

bool GameMaster::did_loose_player(int i)
{
	//int id = board[auto_rotate(Position(0, 0), i)].id; //base of player i
	Unit * unitptr = board[auto_rotate(Position(0, 0), i)].unit; //pointer to the unit on that position
	if(unitptr == nullptr) return false;
	unit_progress[i].our_base_captured += (int)(unitptr->player != i); //enemy standing on our base
	unit_progress[1 - i].enemy_base_captured = unit_progress[i].our_base_captured;
	return unit_progress[i].our_base_captured > 20; //after 20 cycles player i looses
}

void GameMaster::train_for_player(UNIT_TYPE what_to_train, UnitProgress &unit_progress, int i)
{
	++unit_progress.progress[what_to_train];
	Position pos = auto_rotate(Position(0, 0), i);
	if(board[pos].unit != nullptr) return; //cell not empty, do nothing else

	int numoops = (int)(board.op1 == i) + (int)(board.op2 == i);

	if(unit_progress.progress[what_to_train] >= unit_progress.total_time * (4-numoops)/4)
	{
		++largest_id; // new player at 0,0 (or 19,19)
		Unit new_unit;
		new_unit.id = largest_id;
		new_unit.moved = false;
		new_unit.player = i;
		new_unit.type = what_to_train;
		new_unit.pos = auto_rotate(pos, i); //rotate back
		units.emplace(std::make_pair(largest_id, new_unit));
		board[pos].id = largest_id; //updating cell
		board[pos].unit = &units.at(largest_id);
		unit_progress.progress[what_to_train] = 0; // reset training
		++unit_progress.total_time; //incresing construction times
	}
}

void GameMaster::kill_unit(Unit &unit)
{
	board[unit.pos].id = 0;
	board[unit.pos].unit = nullptr;
	units.erase(unit.id);
}
void GameMaster::move_unit(Unit &unit, const Position &newpos)
{
	board[newpos].id = board[unit.pos].id;
	board[unit.pos].id = 0;
	board[newpos].unit = board[unit.pos].unit;
	board[unit.pos].unit = nullptr;
	unit.pos = newpos;
	unit.moved = true; //we forgot this! :)
}

void GameMaster::execute_command_for_player(const Command &command, int i)
{
	Unit &unit = units.at(command.id); //the command commands this unit
	Dir dir = command.dir;
	dir.x = glm::clamp(dir.x, -1, 1); //no cheating
	dir.y = glm::clamp(dir.y, -1, 1);
	dir = (i == 0 ? dir : -dir); //change direction for other player's perspective
	Position newpos = unit.pos + dir;
	if(!unit.moved						//unit did not move in this cycle
	   && unit.player == i				//cannot move enemy's player!! we forgot this as well
	   && abs(dir.x) + abs(dir.y) == 1	//no diagonal move
	   && 0 <= newpos.x && newpos.x < game_size
	   && 0 <= newpos.y && newpos.y < game_size)
	{ //othervise we dont move
		if(board[unit.pos + dir].unit == nullptr) //empty cell
		{
			move_unit(unit, newpos);
		}
		else
		{
			//Unit &other_unit = units.at(board[newpos].id); //the unit on the target cell
			Unit &other_unit = *board[newpos].unit; //the unit on the target cell
			if(other_unit.player != i) //enemy
			{
				switch((unit.type - other_unit.type) % 3)
				{
				case 0: //both die
					kill_unit(unit);
					kill_unit(other_unit);
				case 1: //we die
					kill_unit(unit);
				case 2: //yeah
					kill_unit(other_unit);
					move_unit(unit, newpos);
				default:
					break;
				}
			}
		}
	}
}

/* OLD GAME
int game()
{
player[0] = new DummyPlayer(board, units, 0);
player[1] = new DummyPlayer(board, units, 1);
unit_progress[0].progress = { 0,0,0 };
unit_progress[0].total_time = 10;
unit_progress[0].our_base_captured = 0;
unit_progress[0].enemy_base_captured = 0;
unit_progress[1] = unit_progress[0];

bool p1 = false, p2 = false;

for(int cycle = 0; cycle < max_cycles; ++cycle)
{
const CommandQueue &queue0 = player[0]->move(unit_progress[0]);
const CommandQueue &queue1 = player[1]->move(unit_progress[1]);
int i = 0, j = 0;
while(i != queue0.unitcmds.size() && j != queue1.unitcmds.size())
{
if(rnd_distribution(rnd_engine) % 2)
{
execute_command_for_player(queue0.unitcmds[i], 0);
++i;
}
else
{
execute_command_for_player(queue1.unitcmds[j], 1);
++j;
}
}
for(; i != queue0.unitcmds.size(); ++i)
execute_command_for_player(queue0.unitcmds[i], 0);
for(; j != queue1.unitcmds.size(); ++j)
execute_command_for_player(queue1.unitcmds[j], 1);

train_for_player(queue0.train, unit_progress[0], 0);
train_for_player(queue1.train, unit_progress[1], 1);

p1 = did_loose_player(0);
p2 = did_loose_player(1);
if(p1 || p2) break;
}

if(p1 && p2 || !p1 && !p2) return 0; //Draw
if(p2) return 1; //p2 lost p1 won
if(p1) return 2; //vice versa
}*/
