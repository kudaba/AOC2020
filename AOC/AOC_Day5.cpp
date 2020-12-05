#include "AOC_Precompiled.h"
#include "AOC_Day5.h"

static uint locToUint(GC_StrSlice s)
{
	uint val = 0;
	for (char c : s)
	{
		val <<= 1;
		val += c == 'B' || c == 'R';
	}
	return val;
}

static uint locPart1(char const* file)
{
	uint max = 0;
	for (auto line : GC_File::ReadAllLines(file))
		max = GC_Max(max, locToUint(line));
	return max;
}

DEFINE_TEST_G(Part1, Day5)
{
	TEST_EQ(locPart1("AOC_Day5Test.txt"), 820);
	TEST_EQ(locPart1("AOC_Day5Part1.txt"), 878);
}

static uint locPart2(char const* file)
{
	bool b[1024] = { false };
	uint min = 1024, max = 0;
	for (auto line : GC_File::ReadAllLines(file))
	{
		uint val = locToUint(line);
		b[val] = true;
		min = GC_Min(min, val);
		max = GC_Max(max, val);
	}

	for (uint i = min; i < max; ++i)
		if (!b[i])
			return i;
	return 0;
}

DEFINE_TEST_G(Part2, Day5)
{
	TEST_EQ(locPart2("AOC_Day5Part1.txt"), 504);
}
