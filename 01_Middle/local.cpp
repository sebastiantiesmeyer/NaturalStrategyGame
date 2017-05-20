#include "local.h"
#include <random>

static std::default_random_engine rnd_engine(0);
static std::uniform_real_distribution<float> rnd_distribution;

Dir Options::choose()
{
	float sums[5] = { data[0] };
	for(int i = 1; i < 5; ++i)
		sums[i] = sums[i - 1] + data[i];
	int ind = 0;	float rnd = rnd_distribution(rnd_engine);
	while(rnd*sums[4] > sums[ind]) ++ind;
	switch(ind)
	{
	case 0: return Dir(-1, 0); break; //up
	case 1: return Dir(0, -1); break; //left
	case 2: return Dir(0, 0); break; //still
	case 3: return Dir(0, 1); break; //right
	case 4: return Dir(1, 0); break; //down
	default: throw std::exception("Switch error"); break;
	}
}

void localAvoid3x3(const Unit& unit, const Board &board, Options &options)
{
	if(unit.pos.x > 0)
	{
		Unit *other_unit = board(unit.pos + Dir(-1, 0), unit.player).unit;
		if(other_unit && (unit.type - other_unit->type + 3) % 3 == 1)
			options.up = options.still = 0;
	}
	else options.up = 0;
	if(unit.pos.x < game_size - 1)
	{
		Unit *other_unit = board(unit.pos + Dir(+1, 0), unit.player).unit;
		if(other_unit && (unit.type - other_unit->type + 3) % 3 == 1)
			options.down = options.still = 0;
	}
	else options.down = 0;
	if(unit.pos.y > 0)
	{
		Unit *other_unit = board(unit.pos + Dir(0, -1), unit.player).unit;
		if(other_unit && (unit.type - other_unit->type + 3) % 3 == 1)
			options.left = options.still = 0;
	}
	else options.left = 0;
	if(unit.pos.y < game_size - 1)
	{
		Unit *other_unit = board(unit.pos + Dir(0, +1), unit.player).unit;
		if(other_unit && (unit.type - other_unit->type + 3) % 3 == 1)
			options.right = options.still = 0;
	}
	else options.right = 0;
}

void localNxN(const Unit& unit, const Board &board, Options &options)
{
	const int window_size = 3;
	int xlb = std::max(unit.pos.x - window_size, 0);
	int xub = std::min(unit.pos.x + window_size, game_size - 1);
	int ylb = std::max(unit.pos.y - window_size, 0);
	int yub = std::min(unit.pos.y + window_size, game_size - 1);
	const float reaction[3] = { -1, -19.0, +19.0 }; //{both die, we die, we win}
	const float panicing[3] = { +1.2, +4.0, +0.2 }; //{both die, we die, we win}
	glm::vec2 vec = glm::vec2(0);
	float panic = 0.5; int num = 0;
	for(int x = xlb; x < xub; ++x) for(int y = ylb; y < yub; ++y)
	{
		Position pos = Position(x,y);
		Dir dir = pos - unit.pos;
		Unit *other_unit = board(pos, unit.player).unit;
		if(other_unit && other_unit->player != unit.player) //enemy
		{
			int outcome = (unit.type - other_unit->type + 3) % 3;
			vec += glm::vec2((float)dir.x, (float)dir.y) / ((float)norm1(dir) + 0.1f) * reaction[outcome];
			panic *= panicing[outcome];
			++num;
		}
	}
	Options ret = { panic, panic, 1.f/(panic + 1.0), panic, panic };
	options.down *= panic;
	options.up *= panic;
	options.right *= panic;
	options.left *= panic;
	options.still *= 0.7 * panic / (panic*panic + 1.0);

	options.down	+= std::max(0.f, +vec.y);
	options.up		+= std::max(0.f, -vec.y);
	options.right	+= std::max(0.f, +vec.x);
	options.left	+= std::max(0.f, -vec.x);
	options.still	+= 0.5/(abs(vec.x) + abs(vec.y) + 1.0);
	unit.movementvec = vec;
	unit.numberofenemys = num;
}

//Ellen : 0=R, 1=P, 2=S
//Mienk : 3=r, 4=p, 5=s
/*
void stepOptions(Table &T, const size_t i, const size_t j)
{
	Options ret = { 1,
				1,	1,	1,
					1 };
	const float p0 = 0.f, p1 = 0.f;
	switch(T.m[i][j])
	{
	case 3: // 0
	{
		if(i != 19) switch(T.m[i + 1][j])
		{
		case 0:	ret[2] *= p0;	break;
		case 1:	ret[3] *= p0; ret[2] *= p0;	break;
		case 2:	ret[0] *= p0; ret[1] *= p0; ret[2] *= p0; ret[4] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(i != 0) switch(T.m[i - 1][j])
		{
		case 0:	ret[2] *= p0;	break;
		case 1:	ret[1] *= p0;	ret[2] *= p0; break;
		case 2:	ret[0] *= p0; ret[2] *= p0; ret[3] *= p0; ret[4] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 19) switch(T.m[i][j + 1])
		{
		case 0: ret[2] *= p0; break;
		case 1: ret[4] *= p0; ret[2] *= p0; break;
		case 2:	ret[0] *= p0; ret[1] *= p0; ret[2] *= p0; ret[3] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 0) switch(T.m[i][j - 1])
		{
		case 0: ret[2] *= p0; break;
		case 1: ret[0] *= p0; ret[2] *= p0; break;
		case 2:	ret[1] *= p0; ret[2] *= p0; ret[3] *= p0; ret[4] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		//SARKOK
		if(j != 0 && i != 0) switch(T.m[i - 1][j - 1])
		{
		case 0: break;
		case 1: ret[0] *= p1; ret[1] *= p1; break;
		case 2:	ret[3] *= p0; ret[4] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 19 && i != 19) switch(T.m[i + 1][j + 1])
		{
		case 0: break;
		case 1: ret[3] *= p1; ret[4] *= p1; break;
		case 2:	ret[0] *= p0; ret[1] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 0 && i != 19) switch(T.m[i + 1][j - 1])
		{
		case 0: break;
		case 1: ret[0] *= p1; ret[3] *= p1; break;
		case 2:	ret[1] *= p0; ret[4] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 19 && i != 0) switch(T.m[i - 1][j + 1])
		{
		case 0: break;
		case 1: ret[1] *= p1; ret[4] *= p1;	break;
		case 2:	ret[0] *= p0; ret[3] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
	}	break;
	case 4: //1
	{

		if(i != 19) switch(T.m[i + 1][j])
		{
		case 1:	ret[2] *= p0;	break;
		case 2:	ret[3] *= p0; ret[2] *= p0;	break;
		case 0:	ret[0] *= p0; ret[1] *= p0; ret[2] *= p0; ret[4] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(i != 0) switch(T.m[i - 1][j])
		{
		case 1:	ret[2] *= p0;	break;
		case 2:	ret[1] *= p0;	ret[2] *= p0; break;
		case 0:	ret[0] *= p0; ret[2] *= p0; ret[3] *= p0; ret[4] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 19) switch(T.m[i][j + 1])
		{
		case 1: ret[2] *= p0; break;
		case 2: ret[4] *= p0; ret[2] *= p0; break;
		case 0:	ret[0] *= p0; ret[1] *= p0; ret[2] *= p0; ret[3] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 0) switch(T.m[i][j - 1])
		{
		case 1: ret[2] *= p0; break;
		case 2: ret[0] *= p0; ret[2] *= p0; break;
		case 0:	ret[1] *= p0; ret[2] *= p0; ret[3] *= p0; ret[4] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		//SARKOK
		if(j != 0 && i != 0) switch(T.m[i - 1][j - 1])
		{
		case 1: break;
		case 2: ret[0] *= p1; ret[1] *= p1; break;
		case 0:	ret[3] *= p0; ret[4] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 19 && i != 19) switch(T.m[i + 1][j + 1])
		{
		case 1: break;
		case 2: ret[3] *= p1; ret[4] *= p1; break;
		case 0:	ret[0] *= p0; ret[1] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 0 && i != 19) switch(T.m[i + 1][j - 1])
		{
		case 1: break;
		case 2: ret[0] *= p1; ret[3] *= p1; break;
		case 0:	ret[1] *= p0; ret[4] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 19 && i != 0) switch(T.m[i - 1][j + 1])
		{
		case 1: break;
		case 2: ret[1] *= p1; ret[4] *= p1;	break;
		case 0:	ret[0] *= p0; ret[3] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
	}break;
	case 5: //1
	{

		if(i != 19) switch(T.m[i + 1][j])
		{
		case 2:	ret[2] *= p0;	break;
		case 0:	ret[3] *= p0; ret[2] *= p0;	break;
		case 1:	ret[0] *= p0; ret[1] *= p0; ret[2] *= p0; ret[4] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(i != 0) switch(T.m[i - 1][j])
		{
		case 2:	ret[2] *= p0;	break;
		case 0:	ret[1] *= p0;	ret[2] *= p0; break;
		case 1:	ret[0] *= p0; ret[2] *= p0; ret[3] *= p0; ret[4] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 19) switch(T.m[i][j + 1])
		{
		case 2: ret[2] *= p0; break;
		case 0: ret[4] *= p0; ret[2] *= p0; break;
		case 1:	ret[0] *= p0; ret[1] *= p0; ret[2] *= p0; ret[3] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 0) switch(T.m[i][j - 1])
		{
		case 2: ret[2] *= p0; break;
		case 0: ret[0] *= p0; ret[2] *= p0; break;
		case 1:	ret[1] *= p0; ret[2] *= p0; ret[3] *= p0; ret[4] *= p0;	break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		//SARKOK
		if(j != 0 && i != 0) switch(T.m[i - 1][j - 1])
		{
		case 2: break;
		case 0: ret[0] *= p1; ret[1] *= p1; break;
		case 1:	ret[3] *= p0; ret[4] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 19 && i != 19) switch(T.m[i + 1][j + 1])
		{
		case 2: break;
		case 0: ret[3] *= p1; ret[4] *= p1; break;
		case 1:	ret[0] *= p0; ret[1] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 0 && i != 19) switch(T.m[i + 1][j - 1])
		{
		case 2: break;
		case 0: ret[0] *= p1; ret[3] *= p1; break;
		case 1:	ret[1] *= p0; ret[4] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
		if(j != 19 && i != 0) switch(T.m[i - 1][j + 1])
		{
		case 2: break;
		case 0: ret[1] *= p1; ret[4] *= p1;	break;
		case 1:	ret[0] *= p0; ret[3] *= p0; break;
		case -1: case 3: case 4: case 5: break;
		default: cerr << __LINE__ << " csabixkodjaban\n"; break;
		}
	}break;
	default: cerr << __LINE__ << " csabixkodjaban\n"; break;
	}
	T.o[i][j] = ret;
}
*/