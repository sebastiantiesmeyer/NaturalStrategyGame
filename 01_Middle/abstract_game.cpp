#include "abstract_game.h"
#include "board_viewer.h"

bool AbstractGame::Update()
{
	if (score != glm::dvec2(0.0, 0.0)) return true;

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

bool AbstractGame::Render() const
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
	player[0]->RenderUpdate();
	player[1]->RenderUpdate();
}
AbstractGame::AbstractGame(AbstractPlayer * p0, AbstractPlayer * p1, int board_size)
{
	board.resize(board_size);
	player[0] = p0;
	player[1] = p1;
	player[0]->StartTurn();
	player[1]->StartTurn();
}

void AbstractGame::simulate_board()
{
	int i = 0, j = 0; //at each step, randomly choose a player who's next command will be executed
	while (i != queue0->unitcmds.size() && j != queue1->unitcmds.size())
	{
		if (rnd_distribution(rnd_engine) % 2)
			execute_command(queue0->unitcmds[i++], 0); //note the i++
		else
			execute_command(queue1->unitcmds[j++], 1); //note the j++
	} //the remaining commands has to be executed as well
	for (; i != queue0->unitcmds.size(); ++i)
		execute_command(queue0->unitcmds[i], 0);
	for (; j != queue1->unitcmds.size(); ++j)
		execute_command(queue1->unitcmds[j], 1);

	for (auto &id_unit : units) //we forgot to set the unit movements back to false!
		id_unit.second.moved = false;

	extra_rules();
}


void AbstractGame::execute_command(const Command & command, int player)
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
		&& 0 <= newpos.x && newpos.x < board.size()
		&& 0 <= newpos.y && newpos.y < board.size())
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

void AbstractGame::kill_unit(Unit & unit)
{
	board(unit.pos, unit.player).id = 0;
	board(unit.pos, unit.player).unit = nullptr;
	units.erase(unit.id);
}

void AbstractGame::move_unit(Unit & unit, const Position & newpos)
{
	if (board(newpos, unit.player).unit != nullptr) throw std::exception("Not an empty cell!");
	board(newpos, unit.player).id = board(unit.pos, unit.player).id;
	board(unit.pos, unit.player).id = 0;
	board(newpos, unit.player).unit = board(unit.pos, unit.player).unit;
	board(unit.pos, unit.player).unit = nullptr;
	unit.pos = newpos;
	unit.moved = true;
}
