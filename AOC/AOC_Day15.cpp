#include "AOC_Precompiled.h"
#include "AOC_Day15.h"

static uint locDay15(char const* aFile, uint count)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);

	GC_DynamicArray<uint> spokenMap;
	spokenMap.Resize(count);

	uint i = 1;
	for (GC_StrSlice chunk; GC_Strtok(text, ",", chunk); i++)
	{
		spokenMap[GC_Atoi(chunk)] = i;
	}

	uint last = 0;

	for (; i < count; ++i)
	{
		uint index = spokenMap[last];
		spokenMap[last] = i;
		last = (i - index) * !!index;
	}

	return last;
}

DEFINE_TEST_G(Part1, Day15)
{
	TEST_EQ(locDay15("AOC_Day15Test.txt", 2020), 436);
	TEST_EQ(locDay15("AOC_Day15Part1.txt", 2020), 257);
}

DEFINE_TEST_G(Part2, Day15)
{
	TEST_EQ(locDay15("AOC_Day15Test.txt", 30000000), 175594);
	TEST_EQ(locDay15("AOC_Day15Part1.txt", 30000000), 8546398);
}
