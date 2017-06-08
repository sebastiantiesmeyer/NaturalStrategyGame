#pragma once
#pragma once
#include "data_types.h"
#include "genetic_player.h"
#include <random>

class GeneMaster
{
public:
	GeneMaster();
	~GeneMaster();

	bool RenderUpdate();
	void Render();
	inline GAME_STATUS GetTrainStatus() { return game_status; }
private:
	void training_match();
	void simulate_board();

	void execute_command_for_player(const Command &command, int i);
	bool did_loose_player(int i);
	void train_for_player(UNIT_TYPE what_to_train, UnitProgress &unit_progress, int i);
	void set_outpost_ownership();

	void kill_unit(Unit &unit);
	void move_unit(Unit &unit, const Position &newpos);

private:
	int largest_id[2] = { 1,-1 };
	Units units;
	Board board;
	GeneticPlayer *player[2];
	UnitProgress unit_progress[2];

	std::default_random_engine rnd_engine;
	std::uniform_int_distribution<int> rnd_distribution;

	int cycle = 0;
	const int max_cycles = 5000;
	bool player_0_done = false;
	bool player_1_done = false;

	const CommandQueue *queue0 = nullptr;
	const CommandQueue *queue1 = nullptr;

	GAME_STATUS game_status = ONGOING;
};
