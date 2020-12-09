#include "AOC_Precompiled.h"
#include "AOC_Day9.h"

static uint64 locDay9Part1(char const* aFile, uint preamble)
{
	auto lines = GC_File::ReadAllLines(aFile);

	GC_DynamicArray<uint64> numbers;
	numbers.Reserve(lines.Count());

	for (auto line : lines)
		numbers.Add(GC_Atoi64(line));

	for (uint i = preamble; i < lines.Count(); ++i)
	{
		bool found = false;
		for (uint x = i - preamble; x < i - 1 && !found; ++x)
			for (uint y = x + 1; y < i && !found; ++y)
				found = numbers[i] == numbers[x] + numbers[y];

		if (!found)
			return numbers[i];
	}

	return 0;
}

DEFINE_TEST_G(Part1, Day9)
{
	TEST_EQ(locDay9Part1("AOC_Day9Test.txt", 5), 127);
	TEST_EQ(locDay9Part1("AOC_Day9Part1.txt", 25), 31161678);
}

static uint64 locDay9Part2(char const* aFile, uint value)
{
	auto lines = GC_File::ReadAllLines(aFile);

	GC_DynamicArray<uint> numbers;
	numbers.Reserve(lines.Count());

	for (auto line : lines)
		numbers.Add(GC_Atoi(line));

	for (uint x = 0; x < lines.Count(); ++x)
	{
		uint v = numbers[x];
		uint mn = v, mx = v;
		uint sum = v;
		for (uint y = x + 1; y < lines.Count(); ++y)
		{
			v = numbers[y];
			mn = GC_Min(mn, v);
			mx = GC_Max(mx, v);
			sum += v;
			if (sum == value)
				return mn + mx;
			if (sum > value)
				break;
		}
	}

	return 0;
}
DEFINE_TEST_G(Part2, Day9)
{
	TEST_EQ(locDay9Part2("AOC_Day9Test.txt", 127), 62);
	TEST_EQ(locDay9Part2("AOC_Day9Part1.txt", 31161678), 5453868);
}
