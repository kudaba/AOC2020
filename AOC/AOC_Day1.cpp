#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 result = 0;
	uint64 rolling = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		if (line.IsEmpty())
		{
			result = GC_Max(result, rolling);
			rolling = 0;
		}
		else
		{
			rolling += GC_Atoi(line);
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day1)
{
	TEST_EQ(locPart1("AOC_Day1Test.txt"), 24000);
	TEST_EQ(locPart1("AOC_Day1Part1.txt"), 67633);
}

static auto locPart2(char const* aFile)
{
	GC_HybridArray<uint64, 4> results;
	uint64 rolling = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		if (line.IsEmpty())
		{
			results.Add(rolling);
			if (results.Count() > 3)
				results.Remove(GC_Algorithm::Min(results));
			rolling = 0;
		}
		else
		{
			rolling += GC_Atoi(line);
		}
	}

	results.Add(rolling);
	if (results.Count() > 3)
		results.Remove(GC_Algorithm::Min(results));
	rolling = 0;

	return GC_Algorithm::Sum(results);
}

DEFINE_TEST_G(Part2, Day1)
{
	TEST_EQ(locPart2("AOC_Day1Test.txt"), 45000);
	TEST_EQ(locPart2("AOC_Day1Part1.txt"), 199628);
}
