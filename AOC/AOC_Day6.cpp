#include "AOC_Precompiled.h"
#include "AOC_Day6.h"
#include "GC_Integer.h"

static uint countUnique(char const* aFile)
{
	uint sum = 0;
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	for (GC_StrSlice blocks; GC_Strtok(text, "\n\n", blocks);)
	{
		uint v = 0;
		for (GC_StrSlice line; GC_StrLine(blocks, line);)
		{
			for (char c : line)
			{
				v |= 1 << (c - 'a');
			}
		}

		sum += GC_GetNumBitsSet(v);
	}
	return sum;
}

DEFINE_TEST_G(Part1, Day6)
{
	TEST_EQ(countUnique("AOC_Day6Test.txt"), 11);
	TEST_EQ(countUnique("AOC_Day6Part1.txt"), 6885);
}


static uint countAll(char const* aFile)
{
	uint sum = 0;
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	for (GC_StrSlice blocks; GC_Strtok(text, "\n\n", blocks);)
	{
		uint v[26] = { 0 };
		uint count = 0;
		for (GC_StrSlice line; GC_StrLine(blocks, line);)
		{
			for (char c : line)
			{
				v[(c - 'a')]++;
			}
			++count;
		}

		for (uint vv : v)
			if (vv == count)
				++sum;
	}
	return sum;
}

DEFINE_TEST_G(Part2, Day6)
{
	TEST_EQ(countAll("AOC_Day6Test.txt"), 6);
	TEST_EQ(countAll("AOC_Day6Part1.txt"), 3550);
}
