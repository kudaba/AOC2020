#include "AOC_Precompiled.h"
#include "AOC_Day15.h"
#include "GC_BitVector.h"

static uint locDay15(char const* aFile, uint const count)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);

	GC_DynamicArray<uint> spokenArray;
	spokenArray.Resize(count);

	GC_BitVector set;
	set.Resize(count);

	uint i = 1;
	for (GC_StrSlice chunk; GC_Strtok(text, ",", chunk); i++)
	{
		int const v = GC_Atoi(chunk);
		spokenArray[v] = i;
		set.SetValue(v, true);
	}

	uint last = 0;

	for (; i < count; ++i)
	{
		uint& arr = spokenArray[last];
		if (set.GetValue(last))
		{
			last = i - arr;
		}
		else
		{
			set.SetValue(last, true);
			last = 0;
		}
		arr = i;
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
#if RUN_TESTS
	TEST_EQ(locDay15("AOC_Day15Test.txt", 30000000), 175594);
#endif
	TEST_EQ(locDay15("AOC_Day15Part1.txt", 30000000), 8546398);
}
