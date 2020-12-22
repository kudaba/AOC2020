#include "AOC_Precompiled.h"
#include "AOC_Day22.h"

static uint locDay22Part1(char const* aFile)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_DynamicArray<GC_DynamicArray<uint>> players;
	for (GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{
		auto& player = players.Add();
		GC_StrSlice line;
		GC_StrLine(chunk, line);
		while (GC_StrLine(chunk, line))
		{
			player.Add(GC_Atoi(line));
		}
	}

	auto& player1 = players[0];
	auto& player2 = players[1];

	while (player1.Count() && player2.Count())
	{
		if (player1[0] > player2[0])
		{
			player1.PushBack(player1[0]);
			player1.PushBack(player2[0]);
		}
		else
		{
			player2.PushBack(player2[0]);
			player2.PushBack(player1[0]);
		}
		player1.PopFront();
		player2.PopFront();
	}

	auto& winner = player1.Count() ? player1 : player2;

	uint result = 0;

	uint mult = winner.Count();
	for (uint i : winner)
	{
		result += i * mult--;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day22)
{
	TEST_EQ(locDay22Part1("AOC_Day22Test.txt"), 306);
	TEST_EQ(locDay22Part1("AOC_Day22Part1.txt"), 31314);
}

struct Round
{
	GC_HybridArray<uint, 50> P1;
	GC_HybridArray<uint, 50> P2;

	bool operator==(Round const& other) const
	{
		return GC_Algorithm::Equal(P1, other.P1) && GC_Algorithm::Equal(P2, other.P2);
	}
};

inline uint32 GC_GetHash(Round const& anItem, uint32 aSeed = 0)
{
	bool player = true;
	aSeed = GC_FastHash(&player, 1, aSeed);
	aSeed = GC_FastHash(anItem.P1.Buffer(), anItem.P1.SizeInBytes(), aSeed);
	player = false;
	aSeed = GC_FastHash(&player, 1, aSeed);
	aSeed = GC_FastHash(anItem.P2.Buffer(), anItem.P2.SizeInBytes(), aSeed);
	return aSeed;
}

using Rounds = GC_HashSet<Round>;

static bool locRecursiveCombat(Round& round)
{
	auto& player1 = round.P1;
	auto& player2 = round.P2;

	Rounds rounds;

	while (player1.Count() && player2.Count())
	{
		if (!rounds.Add(round))
			return true;

		uint p1 = player1[0];
		player1.PopFront();
		uint p2 = player2[0];
		player2.PopFront();

		bool p1Win = false;
		if (p1 <= player1.Count() && p2 <= player2.Count())
		{
			Round r{ round.P1.Left(p1), round.P2.Left(p2) };
			p1Win = locRecursiveCombat(r);
		}
		else
		{
			p1Win = p1 > p2;
		}

		if (p1Win)
		{
			player1.PushBack(p1);
			player1.PushBack(p2);
		}
		else
		{
			player2.PushBack(p2);
			player2.PushBack(p1);
		}
	}

	return player1.Count();
}

static uint locDay22Part2(char const* aFile)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_DynamicArray<GC_DynamicArray<uint>> players;
	for (GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{
		auto& player = players.Add();
		GC_StrSlice line;
		GC_StrLine(chunk, line);
		while (GC_StrLine(chunk, line))
		{
			player.Add(GC_Atoi(line));
		}
	}

	auto& player1 = players[0];
	auto& player2 = players[1];
	GC_Optional<Round> instantWin;

	Round r = { player1, player2 };
	locRecursiveCombat(r);

	auto& winner = instantWin.IsSet() ? r.P1 : (r.P1.Count() ? r.P1 : r.P2);

	uint result = 0;

	uint mult = winner.Count();
	for (uint i : winner)
	{
		result += i * mult--;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day22)
{
	TEST_EQ(locDay22Part2("AOC_Day22Test.txt"), 291);
	TEST_EQ(locDay22Part2("AOC_Day22Part1.txt"), 32760);
}
