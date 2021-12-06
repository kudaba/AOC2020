#include "AOC_Precompiled.h"

static uint64 locPart1(char const* aFile, uint days)
{
	GC_String text = GC_File::ReadAllText(aFile);

	uint64 numbers[9] = { 0 };
	for (auto n : GC_StrSplit(text, ","))
		numbers[GC_Atoi(n)]++;

	for (uint i = 0; i < days; ++i)
		numbers[(i + 7) % 9] += numbers[i % 9];

	return GC_Algorithm::Sum(GC_ArrayRange(numbers));
}

DEFINE_TEST_G(Part1, Day6)
{
	TEST_EQ(locPart1("AOC_Day6Test.txt", 80), 5934);
	TEST_EQ(locPart1("AOC_Day6Part1.txt", 80), 395627);
}

DEFINE_TEST_G(Part2, Day6)
{
	TEST_EQ(locPart1("AOC_Day6Test.txt", 256), 26984457539);
	TEST_EQ(locPart1("AOC_Day6Part1.txt", 256), 1767323539209);
}
