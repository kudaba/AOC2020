#include "AOC_Precompiled.h"

static void locUpdate(GC_ArrayRange2D<char> data, GC_Vector2u pos, uint64& result)
{
	if (data(pos) < 10) return;

	data(pos) = 0;
	result++;

	for (auto dir : GC_Cardinal8::Range())
	{
		auto n = pos + GC_CardinalDirection(dir);
		if (n.x < 10 && n.y < 10 && data(n))
		{
			data(n)++;
			locUpdate(data, n, result);
		}
	}
}

static auto locPart1(char const* aFile)
{
	auto data = GC_File::Parse2d<char>(aFile, [](char c) { return char(c - '0'); });

	uint64 result = 0;

	for_range(100)
	{
		for (char& c : data.AsRange()) ++c;
		for_range2d(10,10)
			locUpdate(data, { x, y }, result);
	}

	return result;
}

DEFINE_TEST_G(Part1, Day11)
{
	TEST_EQ(locPart1("AOC_Day11Test.txt"), 1656);
	TEST_EQ(locPart1("AOC_Day11Part1.txt"), 1702);
}

static auto locPart2(char const* aFile)
{
	auto data = GC_File::Parse2d<char>(aFile, [](char c) { return char(c - '0'); });

	uint64 result = 0;

	for (uint64 i = 1; ; ++i)
	{
		for (char& c : data.AsRange()) ++c;
		for_range2d(10,10)
			locUpdate(data, { x, y }, result);

		bool all = true;
		for_range2d(10,10)
			if (data(x,y) != 0)
			{
				all = false;
				break;
			}

		if (all)
			return i;
	}

	//return result;
}

DEFINE_TEST_G(Part2, Day11)
{
	TEST_EQ(locPart2("AOC_Day11Test.txt"), 195);
	TEST_EQ(locPart2("AOC_Day11Part1.txt"), 251);
}
