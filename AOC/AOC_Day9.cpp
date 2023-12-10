#include "AOC_Precompiled.h"

static int64 NextItemInSequence(GC_DynamicArray<int64> const& aSequence)
{
	GC_DynamicArray<int64> nextSequence;
	nextSequence.Resize(aSequence.Count() - 1);

	bool allZeros = true;

	for_range(nextSequence.Count())
	{
		const int64 next = aSequence[i + 1] - aSequence[i];
		allZeros &= !next;
		nextSequence[i] = next;
	}

	if (allZeros)
	{
		return aSequence.Last();
	}

	return aSequence.Last() + NextItemInSequence(nextSequence);
}

static auto locPart1(char const* aFile)
{
	int64 result = 0;

	GC_DynamicArray<int64> sequence;
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		sequence.Clear();

		for (auto n : GC_StrSplit<32>(line, ' '))
		{
			sequence.Add(GC_Atoi(n));
		}

		result += NextItemInSequence(sequence);
	}

	return result;
}

DEFINE_TEST_G(Part1, Day9)
{
	TEST_EQ(locPart1("AOC_Day9Test.txt"), 114);
	TEST_EQ(locPart1("AOC_Day9Part1.txt"), 1884768153);
}

static auto locPart2(char const* aFile)
{
	int64 result = 0;

	GC_DynamicArray<int64> sequence;
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		sequence.Clear();

		for (auto n : GC_StrSplit<32>(line, ' '))
		{
			sequence.Insert(0, GC_Atoi(n));
		}

		result += NextItemInSequence(sequence);
	}

	return result;
}

DEFINE_TEST_G(Part2, Day9)
{
	TEST_EQ(locPart2("AOC_Day9Test.txt"), 2);
	TEST_EQ(locPart2("AOC_Day9Part1.txt"), 1031);
}
