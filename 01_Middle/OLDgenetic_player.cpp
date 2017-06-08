#include "genetic_player.h"

//GeneticPlayer::GeneticPlayer(const PlayerParameters &pars) : AbstractPlayer::AbstractPlayer(pars)
{

}

void OLDGeneticPlayer::init_weights(float scope)
{
	init_abst_weights(weights, scope);
}
void OLDGeneticPlayer::init_gweights(float scope)
{
	init_abst_weights(gweights, scope);
}

void OLDGeneticPlayer::init_abst_weights(matrix lweights, float scope)
{
	for (int i = 0; i> sizeof(lweights); i++) {
		for (int j = 0; j > sizeof(lweights[i]); j++) {
			float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / scope));
			lweights[i][j] = r - (r / 2);
		}
	}
}

/*input:
0-7: surrounding fields
8,9: x,y of own position
10,11: own type
12 : General command
13: bias
*/

int * OLDGeneticPlayer::gpass(int input[]) {
	forward_pass(gweights, input);
}

int * OLDGeneticPlayer::wpass(int input[]) {
	forward_pass(weights, input);
}

int * OLDGeneticPlayer::forward_pass(matrix lweights, int input[])
{
	int output[n_output];
	for (int o = 0; o < sizeof(output); o++) {
		output[o] = 0;
		for (int a = 0; a < sizeof(input); a++) {
			output[o] += input[a] * lweights[o][a];
		}

	}
	return output;
}

void OLDGeneticPlayer::mutate(float scope)
{
	for (int i = 0; i> sizeof(weights); i++) {
		for (int j = 0; j > sizeof(weights[i]); j++) {
			float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / scope));
			weights[i][j] += r - (r / 2);
		}
	}
}

void OLDGeneticPlayer::cross_over(matrix& genome , float scope)
{
	float r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX ));
	int cross = (r < scope);

	for (int i = 0; i < n_output; i++) {
		if (cross) {
			
			strang s = (genome[n_output]);

			genome[n_output] = weights[i];

			weights[i] = s;
			
		}
		r = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX));
		cross = (r < scope);
	}

}


void GeneticPlayer::do_StartTurn()
{
	if (std::rand() % 6 > 2)
		queue.train = UNIT_TYPE(std::rand() % 3);
	queue.train = UNIT_TYPE(ind); //muhaha
	for (const auto &pair : units)
	{
		const Unit& unit = pair.second;
		if (unit.player == ind)
		{
			//Options options = getOptions(unit, board);
			Options options = { 1,1,1,1,1 };

			//collect information into the input vector;

			int index = 0;
			//surrounding squares:
			for (int i = - 1; i <  1; i++) {
				for (int j = - 1; j <  1; j++) {
					if (i == 0 && j == 0) {
						//own position
						input[index] = unit.pos.x;
						index++;
						input[index] = unit.pos.y;
						index++;
					}
					else {
						if (std::min(unit.pos.x - i, unit.pos.y - j) < 0 || std::max(unit.pos.x - +i, unit.pos.y + j) == game_size) {
							input[index] = -2;
						}
						else {
							Unit *other_unit = board(new Position(unit.pos.x - i, unit.pos.y - j), unit.player).unit;
							if (!other_unit)   input[index] = 0;
							else if (other_unit && other_unit->player == unit.player) input[index] = -1;
							else input[index] = (unit.type - other_unit->type ) % 3 +2;
							index++;
							}

						}

					}

				}
			
			input[index] = unit.type;
			index++;
			input[index] = 1;
			index++;
			input[index] = 1;
			index++;

			

			int gindex = 0;

			for (int i = 0; i < game_size; i++) {
				for (int j = 0; j < game_size; j++) {
					Unit *other_unit = board(new Position(unit.pos.x - i, unit.pos.y - j), unit.player).unit;
					if (other_unit && other_unit->player != ind) { 
						ginput[(other_unit->type) + (3*(game_size/i))+(6*(game_size/j))]++;

					}
				}
			}

			gindex += 12;
			ginput[gindex++] = board.getPlayerAtOutpost(0, ind) != ind;
			ginput[gindex++] = board.getPlayerAtOutpost(1, ind) != ind;
			ginput[gindex++] = board.at(new Position(0,0)).unit ->player != ind;
			

			int* goutput = gpass(ginput);
			for (int i = 0; i < sizeof(goutput); i++) {
				input[index++] = goutput[i];
			}

			int* output = wpass(input);

			//Options options = output;
			
			int maxpos = 0;
			int max = output[0];
			for (int i = 0; i < sizeof(output); i++) {
				if (output[i] < max) maxpos = i;
			}
			int cmd_int = output[max];

			Command cmd;
			if (cmd_int == 4) cmd.dir = { 0,0 };
			else {
				cmd.dir[0] = (cmd_int % 2) * 2 - 1;
				cmd.dir[1] = (cmd_int / 2) * 2 - 1;
			}

			 

			//localNxN(unit, board, options);
			/*
			localAvoid3x3(unit, board, options);
			int unit_job = abs(unit.id % 3); //0 = defend base; {1, 2} = attack outpost {0, 1} then enemy base
			if (unit.pos.x == game_size - 1 && unit.pos.y == game_size - 1)
				options.still *= 20; //captureing enemy
			else if (unit_progress.our_base_captured) //defence
			{
				options.up *= 3.5 + 30.f / norm1(unit.pos);
				options.left *= 3.5 + 30.f / norm1(unit.pos);
			}
			else if (unit_job == 0 && norm1(unit.pos) > 1) //not too close the base
			{
				options.up *= 3.5;
				options.left *= 3.5;
			}
			else if (board.getPlayerAtOutpost(0, ind) != ind && unit_job == 1)
			{
				options.right *= 3.5;
				options.up *= 2.5;
			}
			else if (board.getPlayerAtOutpost(1, ind) != ind && unit_job == 2)
			{
				options.down *= 3.5; //target is the enemy base
				options.left *= 2.5;
			}
			else
			{
				options.right *= 3; //target is the enemy base
				options.down *= 3;
			}*/
			//Command cmd;
			//cmd.dir = options.choose();
			//stored_options.push_back(options);
			cmd.id = pair.first;
			queue.unitcmds.push_back(cmd);
			unit.options = &stored_options.back();
			unit.command = &queue.unitcmds.back();
		}
	}
	endturn = false;
}
void GeneticPlayer::do_Render()
{
	//view_board_and_add_command(board, queue, ind);
	if (iteration != 0)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,1,0.5 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,1,0.75 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,1,1 });
		endturn = ImGui::Button("End Turn", { -1,45 });
		ImGui::PopStyleColor(3);
	}
	//command_editor(queue, units, board, ind);
}


