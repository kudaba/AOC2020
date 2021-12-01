#include "AOC_Precompiled.h"

static uint locPart1(char const* aFile)
{
	uint result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
	}

	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	for (GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{

	}

	return result;
}

DEFINE_TEST_G(Part1, Day7)
{
	TEST_EQ(locPart1("AOC_Day7Test.txt"), 0);
	TEST_EQ(locPart1("AOC_Day7Part1.txt"), 0);
}

DEFINE_TEST_G(Part2, Day7)
{
	TEST_EQ(locPart1("AOC_Day7Part2.txt"), 0);
}
