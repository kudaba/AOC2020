#include "AOC_Precompiled.h"

static uint locPart1(char const* aFile)
{
	auto vals = GC_File::Parse<int>(aFile, [](GC_StrSlice aLine, int& anItem) { anItem = GC_Atoi(aLine); return true; });

	int result = 0;
	for (uint i = 1; i < vals.Count(); ++i)
	{ 
		result += vals[i] > vals[i-1];
	}

	return result;
}

DEFINE_TEST_G(Part1, Day1)
{
	TEST_EQ(locPart1("AOC_Day1Test.txt"), 7);
	TEST_EQ(locPart1("AOC_Day1Part1.txt"), 1466);
}

static uint locPart2(char const* aFile)
{
	auto vals = GC_File::Parse<int>(aFile, [](GC_StrSlice aLine, int& anItem) { anItem = GC_Atoi(aLine); return true; });

	int result = 0;

	int sum = vals[0] + vals[1] + vals[2];
	for (uint i = 3; i < vals.Count(); ++i)
	{
		int newSum = sum - vals[i - 3] + vals[i];
		if (newSum > sum) ++result;
		sum = newSum;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day1)
{
	TEST_EQ(locPart2("AOC_Day1Test.txt"), 5);
	TEST_EQ(locPart2("AOC_Day1Part1.txt"), 1491);
}
