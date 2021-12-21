#include "AOC_Precompiled.h"

static auto locPart1(uint p1, uint p2)
{
	uint positions[2] = { p1-1, p2-1 };
	uint scores[2] = {};
	uint rolls = 0;

	auto next = [&rolls]()
	{
		return ((rolls++) % 100) + 1;
	};

	while (scores[0] < 1000 && scores[1] < 1000)
	{
		for_range(2)
		{
			positions[i] = (positions[i] + next() + next() + next()) % 10;
			scores[i] += positions[i] + 1;
			if (scores[i] >= 1000)
				break;
		}
	}

	return GC_Min(scores[0], scores[1]) * (rolls);
}

DEFINE_TEST_G(Part1, Day21)
{
	TEST_EQ(locPart1(4, 8), 739785);
	TEST_EQ(locPart1(7, 5), 798147);
}

struct Game
{
	union
	{
		struct
		{
			char scores[2];
			char positions[2];
			char turn;
		};
		GC_Vector2u State;
	};

	bool operator==(Game const& anOther) const { return State == anOther.State; }
};

uint32 GC_GetHash(Game const& aGame) { return aGame.State.x ^ aGame.State.y; }
using Wins = GC_Vector2<u64>;

static Wins locCalcWins(Game game, uint64 ngames, char const (&rolls)[10], GC_HashMap<Game, Wins>& cachedWins)
{
	if (Wins const* wins = cachedWins.Find(game))
		return *wins * ngames;
	
	Wins wins(0);

	for (uint r = 3; r <= 9; ++r)
	{
		Game g = game;
		g.positions[g.turn] = (g.positions[g.turn] + r) % 10;
		g.scores[g.turn] += g.positions[g.turn] + 1;
		if (g.scores[g.turn] > 20)
			wins[g.turn] += ngames*rolls[r];
		else
		{
			g.turn = !g.turn;
			wins += locCalcWins(g, ngames*rolls[r], rolls, cachedWins);
		}
	}

	cachedWins.GetOrAdd(game) = wins / ngames;
	return wins;
}

static uint64 locPart2(char p1, char p2)
{
	char rolls[10] = {};
	{
		for_range2d(3, 3)
			for_range(3)
				rolls[x + y + i + 3]++;
	}

	Game initGame = {};
	initGame.positions[0] = p1 - 1;
	initGame.positions[1] = p2 - 1;

	GC_HashMap<Game, Wins> cachedWins;
	cachedWins.Reserve(50000);
	Wins wins = locCalcWins(initGame, 1, rolls, cachedWins);

	return GC_Max(wins[0], wins[1]);
}

DEFINE_TEST_G(Part2, Day21)
{
	TEST_EQ(locPart2(4, 8), 444356092776315);
	TEST_EQ(locPart2(7, 5), 809953813657517);
}
