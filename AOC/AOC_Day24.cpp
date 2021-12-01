#include "AOC_Precompiled.h"

static uint locPart1(char const* aFile)
{
	uint result = 0;

	// By line with parse function
	auto items = GC_File::Parse<int>(aFile, [](auto aLine, auto& anItem)
		{
			anItem = GC_Atoi(aLine);
			return true;
		});
	for (auto item : items)
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

DEFINE_TEST_G(Part1, Day24)
{
	TEST_EQ(locPart1("AOC_Day24Test.txt"), 0);
	TEST_EQ(locPart1("AOC_Day24Part1.txt"), 0);
}

DEFINE_TEST_G(Part2, Day24)
{
	TEST_EQ(locPart1("AOC_Day24Part2.txt"), 0);
}
