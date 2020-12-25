#include "AOC_Precompiled.h"
#include "AOC_Day25.h"

static uint64 locDay25Part1(char const* aFile)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_StrSlice line;
	GC_StrLine(text, line);
	uint64 key1 = GC_Atoi(line);
	GC_StrLine(text, line);
	uint64 key2 = GC_Atoi(line);

	uint64 value = 1;
	uint64 const subject = 7;
	uint64 const divisor = 20201227;
	uint loops = 0;
	while (value != key1)
	{
		value = (value * subject) % divisor;
		++loops;
	}

	uint64 result = 1;
	for_range(loops)
	{
		result = (result * key2) % divisor;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day25)
{
	TEST_EQ(locDay25Part1("AOC_Day25Test.txt"), 14897079);
	TEST_EQ(locDay25Part1("AOC_Day25Part1.txt"), 16457981);
}

