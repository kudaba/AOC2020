#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, uint aRange) -> uint
{
	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);

	for (uint x = 0; text[x + aRange]; ++x)
	{
		uint bits = 0;

		for_range(aRange)
			bits |= (1 << (text[x + i] - 'a'));

		if (GC_GetNumBitsSet(bits) == aRange)
			return x + aRange;
	}

	return 0;
}

DEFINE_TEST_G(Part1, Day6)
{
	TEST_EQ(locPart1("AOC_Day6Test.txt", 4), 7);
	TEST_EQ(locPart1("AOC_Day6Part1.txt", 4), 1909);
}

DEFINE_TEST_G(Part2, Day6)
{
	TEST_EQ(locPart1("AOC_Day6Test.txt", 14), 19);
	TEST_EQ(locPart1("AOC_Day6Part1.txt", 14), 3380);
}
