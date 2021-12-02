#include "AOC_Precompiled.h"

static auto locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<int>(aFile, [](auto aLine)
		{
			return GC_Atoi(aLine);
		});
}

static uint locPart1(char const* aFile)
{
	uint result = 0;

	for (auto item : locParseData(aFile))
	{
		(void)item;
	}

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

DEFINE_TEST_G(Part1, Day3)
{
	TEST_EQ(locPart1("AOC_Day3Test.txt"), 0);
	TEST_EQ(locPart1("AOC_Day3Part1.txt"), 0);
}

static uint locPart2(char const* aFile)
{
	uint result = 0;

	for (auto item : locParseData(aFile))
	{
		(void)item;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day3)
{
	TEST_EQ(locPart2("AOC_Day3Test.txt"), 0);
	TEST_EQ(locPart2("AOC_Day3Part1.txt"), 0);
}
