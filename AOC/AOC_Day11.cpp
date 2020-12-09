#include "AOC_Precompiled.h"
#include "AOC_Day11.h"

static uint locDay11Part1(char const* aFile)
{
	uint result = 0;

	for (auto line : GC_File::ReadAllLines(aFile))
	{

	}

	GC_String text;
	GC_File::ReadAllText(aFile, text);
	for (GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{

	}

	return result;
}

DEFINE_TEST_G(Part1, Day11)
{
	TEST_EQ(locDay11Part1("AOC_Day11Test.txt"), 0);
	TEST_EQ(locDay11Part1("AOC_Day11Part1.txt"), 0);
}

static uint locDay11Part2(char const* aFile)
{
	GC_UNUSED(aFile);
	return 0;
}

DEFINE_TEST_G(Part2, Day11)
{
	TEST_EQ(locDay11Part2("AOC_Day11Test.txt"), 0);
	TEST_EQ(locDay11Part2("AOC_Day11Part1.txt"), 0);
}
