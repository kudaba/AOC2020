#include "AOC_Precompiled.h"

static auto locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<int>(aFile, [](auto aLine)
		{
			return GC_Atoi(aLine);
		});
}

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

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

DEFINE_TEST_G(Part1, Day17)
{
	TEST_EQ(locPart1("AOC_Day17Test.txt"), 0);
	TEST_EQ(locPart1("AOC_Day17Part1.txt"), 0);
}

static auto locPart2(char const* aFile)
{
	(void)aFile;
	uint64 result = 0;
	return result;
}

DEFINE_TEST_G(Part2, Day17)
{
	TEST_EQ(locPart2("AOC_Day17Test.txt"), 0);
	TEST_EQ(locPart2("AOC_Day17Part1.txt"), 0);
}
