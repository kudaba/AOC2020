#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, bool aSecond)
{
	GC_String text = GC_File::ReadAllText(aFile);

	GC_DynamicArray<uint64> numbers;
	for (auto n : GC_StrSplit(text, ","))
	{
		uint i = GC_Atoi(n);
		numbers.Resize(GC_Max(i + 1, numbers.Count()), 0);
		numbers[i]++;
	}

	uint64 minCost = UINT64_MAX;

	GC_DynamicArray<uint64> costs;
	costs.Resize(numbers.Count());

	uint totalCosts = 0;
	for (uint i = 0; i < costs.Count(); ++i)
	{
		totalCosts += i;
		costs[i] = aSecond ? totalCosts : i;
	}

	for (uint i = 0; i < numbers.Count(); ++i)
	{
		uint64 cost = 0;
		for (uint j = 0; j < numbers.Count(); ++j)
			cost += costs[GC_Abs((int)j - (int)i)] * numbers[j];

		if (cost < minCost)
			minCost = cost;
	}

	return minCost;
}

DEFINE_TEST_G(Part1, Day7)
{
	TEST_EQ(locPart1("AOC_Day7Test.txt", false), 37);
	TEST_EQ(locPart1("AOC_Day7Part1.txt", false), 335271);
}

DEFINE_TEST_G(Part2, Day7)
{
	TEST_EQ(locPart1("AOC_Day7Test.txt", true), 168);
	TEST_EQ(locPart1("AOC_Day7Part1.txt", true), 95851339);
}
