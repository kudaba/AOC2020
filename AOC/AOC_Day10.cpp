#include "AOC_Precompiled.h"
#include "AOC_Day10.h"

static uint locDay10Part1(char const* aFile)
{
	GC_DynamicArray<int> jolts;
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		jolts.Add(GC_Atoi(line));
	}
	jolts.Sort();

	uint jolt = 0;
	uint one = 0;
	uint three = 0;
	for (uint i = 0; i < jolts.Count(); ++i)
	{
		if (jolts[i] - jolt == 1) ++one;
		if (jolts[i] - jolt == 3) ++three;
		jolt = jolts[i];
	}

	return one * (three + 1);
}

DEFINE_TEST_G(Part1, Day10)
{
	TEST_EQ(locDay10Part1("AOC_Day10Test.txt"), 35);
	TEST_EQ(locDay10Part1("AOC_Day10Part1.txt"), 2450);
}
//111111
//101111
//110111
//100111
//111011
//101011
//110011
//100011x
//111101
//101101
//110101
//100101
//111001
//101001
//110001x
//100001x


int comboes(int i)
{
	if (i < 2)
		return 1;
	if (i < 3)
		return 2;
	if (i < 4)
		return 4;
	if (i < 5)
		return 7;
	if (i < 6)
		return 13;

	GC_ASSERT(false);
	return 6;
}

static uint64 locDay10Part2(char const* aFile)
{
	GC_DynamicArray<int> jolts;
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		jolts.Add(GC_Atoi(line));
	}
	jolts.Sort();

	uint64 things = 1;
	uint i = 0;
	uint max = 0;

	for (uint j = 0; j < jolts.Count(); ++j)
		if (jolts[j] - (j ? jolts[j-1] : 0) == 3)
		{
			max = GC_Max(max, (j - i));
			things *= comboes((j - i));
			i = j;
			break;
		}

	while (i < jolts.Count() - 1)
	{
		bool found = false;
		for (uint j = i + 1; j < jolts.Count(); ++j)
			if (jolts[j] - jolts[j-1] == 3)
			{
				max = GC_Max(max, (j - i) - 1);
				things *= comboes((j - i) - 1);
				i = j;
				found = true;
				break;
			}

		if (!found)
		{
			max = GC_Max(max, (jolts.Count() - i));
			things *= comboes((jolts.Count() - i) - 1);
			break;
		}
	}

	return things;
}

DEFINE_TEST_G(Part2, Day10)
{
	TEST_EQ(locDay10Part2("AOC_Day10Test.txt"), 8);
	TEST_EQ(locDay10Part2("AOC_Day10Test2.txt"), 19208);
	TEST_EQ(locDay10Part2("AOC_Day10Part1.txt"), 32396521357312);
}
