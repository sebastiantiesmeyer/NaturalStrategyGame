#include "train.h"
#include <random>

static std::default_random_engine rnd_engine(42);
static std::uniform_real_distribution<float> rnd_distribution;

inline float train_weights(int n_pred, int n_same, int n_pray, int n_current, float progress)
{
	return std::max((2.0f * n_pray - 0.5f * n_pred + 10.f*progress) / (0.1f + 0.5f*n_same + 3.f*n_current), 0.f);
}

UNIT_TYPE train(const Units &units, const UnitProgress &progess, int player)
{	//statsistics		R>S>P
	int p0_units[3] = { 0,0,0 };
	int p1_units[3] = { 0,0,0 };
	int sign = player == 0 ? 1 : -1;
	for(const auto & id_unit : units)
	{
		assert(id_unit.first*sign != 0);
		if(id_unit.first*sign > 0)	++p0_units[id_unit.second.type];
		else						++p1_units[id_unit.second.type];
	}
	glm::vec3 ps = glm::vec3(1); //weights
	//current weight		its predater,		enemy equal,	its prey,			current unit count
	ps[ROCK]	= train_weights(p1_units[PAPER], p1_units[ROCK], p1_units[SCISSOR], p0_units[ROCK], progess.progress[ROCK]/(float)progess.total_time);
	ps[SCISSOR] = train_weights(p1_units[ROCK], p1_units[SCISSOR], p1_units[PAPER], p0_units[SCISSOR], progess.progress[SCISSOR] / (float)progess.total_time);
	ps[PAPER]	= train_weights(p1_units[SCISSOR], p1_units[PAPER], p1_units[ROCK], p0_units[PAPER], progess.progress[PAPER] / (float)progess.total_time);

	glm::vec3 sums = glm::vec3(ps.x); sums.y += ps.y; sums.z = sums.y + ps.z;
	float rnd = rnd_distribution(rnd_engine)*sums.z;
	if(rnd < sums.x) return ROCK;
	if(rnd < sums.y) return SCISSOR;
	return PAPER;
}