#include "AOC_Precompiled.h"

static auto locParseData2(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<int64>(aFile, [](auto aLine)
		{
			return GC_Atoi(aLine);
		});
}

static auto locPart2(char const* aFile, int64 mult, int rounds)
{
	auto data = locParseData2(aFile);

	GC_DynamicArray<uint16> workingSet;
	workingSet.Resize(data.Count());

	uint Zero = 0;
	for_index(auto& d : data)
	{
		d *= mult;
		workingSet[i] = (uint16)i;
		if (!d)
			Zero = i;
	}

	for_range_v(r, rounds)
	{
		for_index_v(di, auto& d : data)
		{
			if (!d) continue;

			int i = workingSet.Find(di);
			int j = i;
			if (d > 0)
			{
				j = (int)GC_LoopClamp<int64>(i + d - 1, 1, data.Count()) + 1;
				if (j == (int)data.Count())
					j = 1;
			}
			else
			{
				j = (int)GC_LoopClamp<int64>(i + d + 1, 0, data.Count() - 1) - 1;
				if (j <= 0)
					j = (int)data.Count() - 1 + j;
			}

			if (i < j)
			{
				GC_MemMove(&workingSet[i], &workingSet[i + 1], (j - i) * sizeof(uint16));
				workingSet[j] = (uint16)di;
			}
			else if (i > j)
			{
				GC_MemMove(&workingSet[j + 1], &workingSet[j], (i - j) * sizeof(uint16));
				workingSet[j] = (uint16)di;
			}
		}
	}

	int i = workingSet.Find(Zero);
	int64 result = 0;
	for_range_v(t, 3)
		result += data[workingSet[((t+1) * 1000 + i) % data.Count()]];
	return result;
}

DEFINE_TEST_G(Part1, Day20)
{
	TEST_EQ(locPart2("AOC_Day20Test.txt", 1, 1), 3);
	TEST_EQ(locPart2("AOC_Day20Part1.txt", 1, 1), 11616);
}

DEFINE_TEST_G(Part2, Day20)
{
	TEST_EQ(locPart2("AOC_Day20Test.txt", 811589153, 10), 1623178306);
	TEST_EQ(locPart2("AOC_Day20Part1.txt", 811589153, 10), 9937909178485);
}
