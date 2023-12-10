#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 result = 1;

	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	auto parts = GC_StrSplit<2>(text, '\n');
	auto timeParts = GC_StrSplit<4>(parts[0], ' ', false);
	auto distParts = GC_StrSplit<4>(parts[1], ' ', false);

	for_range(timeParts.Count())
	{
		if (i)
		{
			uint time = GC_Atoi(timeParts[i]);
			uint dist = GC_Atoi(distParts[i]);

			uint ways = 0;
			for_range_v(j, time)
			{
				if (j * (time - j) > dist)
					++ways;
			}

			result *= ways;
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day6)
{
	TEST_EQ(locPart1("AOC_Day6Test.txt"), 288);
	TEST_EQ(locPart1("AOC_Day6Part1.txt"), 303600);
}

static auto locPart2(char const* aFile)
{
	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	auto parts = GC_StrSplit<2>(text, '\n');
	auto timeParts = GC_StrSplit<4>(parts[0], ' ', false);
	auto distParts = GC_StrSplit<4>(parts[1], ' ', false);

	uint64 time = 0;
	uint64 dist = 0;

	for_range(timeParts.Count())
	{
		if (i)
		{
			for (char c : timeParts[i])
				time = time * 10 + (c - '0');

			for (char c : distParts[i])
				dist = dist * 10 + (c - '0');
		}
	}

	uint64 lower = 0;
	{
		uint64 min = 0, max = time, cur = time / 2;
		while (min < max - 1)
		{
			if (cur * (time - cur) > dist)
				max = cur;
			else
				min = cur;

			cur = min + (max - min) / 2;
		}
		lower = min;
	}

	uint64 upper = 0;
	{
		uint64 min = 0, max = time, cur = time / 2;
		while (min < max - 1)
		{
			if (cur * (time - cur) > dist)
				min = cur;
			else
				max = cur;

			cur = min + (max - min) / 2;
		}
		upper = min;
	}

	return upper - lower;
}

DEFINE_TEST_G(Part2, Day6)
{
	TEST_EQ(locPart2("AOC_Day6Test.txt"), 71503);
	TEST_EQ(locPart2("AOC_Day6Part1.txt"), 23654842);
}
