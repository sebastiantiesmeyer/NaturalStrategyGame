#pragma once
#include "data_types.h"
#include "abstract_player.h"
#include <random>

class GameMaster
{
public:
	GameMaster()
	{
		board.board.resize(game_size*game_size);
	}
	int game()
	{
		player[0] = new DummyPlayer(board, units);
		player[1] = new DummyPlayer(board, units);
		unit_progress[0].progress = { 0,0,0 };
		unit_progress[0].total_time = 10;
		unit_progress[1].progress = { 0,0,0 };
		unit_progress[1].total_time = 10;

		bool p1 = false;
		bool p2 = false;

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
	}

	bool did_loose_player(int i)
	{
		int id = board[auto_rotate(Position(0, 0), i)].id; //base of player i
		if(id == 0) return false;
		unit_progress[i].our_base_captured += (int)(units.at(id).player != i); //enemy standing on our base
		unit_progress[1-i].enemy_base_captured = unit_progress[i].our_base_captured;
		return unit_progress[i].our_base_captured > 20;
	}

	void train_for_player(UNIT_TYPE what_to_train, UnitProgress unit_progress, int i)
	{
		++unit_progress.progress[what_to_train];
		Position pos = auto_rotate(Position(0, 0), i);
		if(board[pos].id != 0) return; //cell not empty, do nothing else
		if(unit_progress.progress[what_to_train] >= unit_progress.total_time)
		{
			++largest_id; // new player at 0,0 (or 19,19)
			units[largest_id].id = largest_id;
			units[largest_id].moved = false;
			units[largest_id].player = i;
			units[largest_id].type = what_to_train;
			units[largest_id].pos = auto_rotate(pos, i); //rotate back
			board[pos].id != largest_id; //updating cell
			unit_progress.progress[what_to_train] = 0; // reset training
			++unit_progress.total_time; //incresing construction times
		}
	}

	void kill_unit(int id)
	{
		board[units[id].pos].id = 0;
		units.erase(id);
	}
	void move_unit(int id, Position newpos)
	{
		Unit &unit = units[id];
		board[unit.pos].id = 0;
		board[newpos].id = id;
		unit.pos = newpos;
	}

	void execute_command_for_player(const Command &command, int i)
	{
			Unit &unit = units[command.id];
			Dir dir = command.dir;
			dir.x = glm::clamp(dir.x, -1, 1);
			dir.y = glm::clamp(dir.y, -1, 1);
			dir = (i == 0 ? dir : -dir); //change direction for other player's perspective
			Position newpos = unit.pos + dir;
			if(!units[command.id].moved
			   && abs(dir.x) + abs(dir.y) == 1
			   && 0 <= newpos.x && newpos.x < game_size
			   && 0 <= newpos.y && newpos.y < game_size)
			{ //othervise we dont move
				if(board[unit.pos + dir].id == 0) //empty cell
				{
					move_unit(command.id, newpos);
				}
				else
				{
					Unit other_unit = units[board[newpos].id]; //the unit on the target cell
					if(other_unit.player != i) //enemy
					{
						switch((unit.type-other_unit.type) % 3)
						{
						case 0: //both die
							kill_unit(command.id);
							kill_unit(board[newpos].id);
						case 1: //we die
							kill_unit(command.id);
						case 2: //yeah
							kill_unit(board[newpos].id);
							move_unit(command.id, newpos);
						default:
							break;
						}
					}
				}
			}
	}

	AbstractPlayer *player[2];
	Units units;
	int largest_id = 0;
	Board board;
	UnitProgress unit_progress[2];
	std::default_random_engine rnd_engine;
	std::uniform_int_distribution<int> rnd_distribution;

	const int max_cycles = 1000;
};
