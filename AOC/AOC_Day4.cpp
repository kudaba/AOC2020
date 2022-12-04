#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<2>(line, ',');

		int min1 = 0, max1 = 0;
		{
			auto minmax = GC_StrSplit<2>(parts[0], '-');
			min1 = GC_Atoi(minmax[0]);
			max1 = GC_Atoi(minmax[1]);
		}

		int min2 = 0, max2 = 0;
		{
			auto minmax = GC_StrSplit<2>(parts[1], '-');
			min2 = GC_Atoi(minmax[0]);
			max2 = GC_Atoi(minmax[1]);
		}

		if ((min1 >= min2 && max1 <= max2) ||
			(min2 >= min1 && max2 <= max1))
			++result;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day4)
{
	TEST_EQ(locPart1("AOC_Day4Test.txt"), 2);
	TEST_EQ(locPart1("AOC_Day4Part1.txt"), 518);
}

static auto locPart2(char const* aFile)
{
	uint64 result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<2>(line, ',');

		int min1 = 0, max1 = 0;
		{
			auto minmax = GC_StrSplit<2>(parts[0], '-');
			min1 = GC_Atoi(minmax[0]);
			max1 = GC_Atoi(minmax[1]);
		}

		int min2 = 0, max2 = 0;
		{
			auto minmax = GC_StrSplit<2>(parts[1], '-');
			min2 = GC_Atoi(minmax[0]);
			max2 = GC_Atoi(minmax[1]);
		}

		if (!(max1 < min2 || min1 > max2))
			++result;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day4)
{
	TEST_EQ(locPart2("AOC_Day4Test.txt"), 4);
	TEST_EQ(locPart2("AOC_Day4Part1.txt"), 909);
}
