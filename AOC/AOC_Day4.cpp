#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

	GC_DynamicArray<uint> winningNumbers;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		winningNumbers.Clear();

		auto parts = GC_StrSplit<2>(line, " | ");

		for (auto number : GC_StrSplit<32>(GC_StrSplit<2>(parts[0], ": ")[1], ' '))
			winningNumbers.Add(GC_Atoi(number));

		uint score = 0;
		for (auto number : GC_StrSplit<32>(parts[1], ' '))
		{
			if (winningNumbers.Contains(GC_Atou(number)))
			{
				score = score * 2 + !score;
			}
		}

		result += score;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day4)
{
	TEST_EQ(locPart1("AOC_Day4Test.txt"), 13);
	TEST_EQ(locPart1("AOC_Day4Part1.txt"), 27845);
}

static auto locPart2(char const* aFile)
{
	uint64 result = 0;

	GC_DynamicArray<uint> winningNumbers;
	GC_DynamicArray<uint> cards;

	auto tokens = GC_File::ReadAllLines(aFile);
	auto lines = tokens.GetAll();
	cards.Resize(lines.Count(), 1);


	// By line parsing
	for_index(auto line : lines)
	{
		result += cards[i];

		winningNumbers.Clear();

		auto parts = GC_StrSplit<2>(line, " | ");

		for (auto number : GC_StrSplit<32>(GC_StrSplit<2>(parts[0], ": ")[1], ' '))
			winningNumbers.Add(GC_Atoi(number));

		uint score = 0;
		for (auto number : GC_StrSplit<32>(parts[1], ' '))
		{
			if (winningNumbers.Contains(GC_Atou(number)))
			{
				++score;
			}
		}

		for_range_v(j, score)
		{
			if (i + j + 1 < cards.Count())
			{
				cards[i + j + 1] += cards[i];
			}
		}
	}

	return result;
}

DEFINE_TEST_G(Part2, Day4)
{
	TEST_EQ(locPart2("AOC_Day4Test.txt"), 30);
	TEST_EQ(locPart2("AOC_Day4Part1.txt"), 9496801);
}
