#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		GC_StrSlice left = line.Left(line.Count() / 2);
		GC_StrSlice right = line.Right(line.Count() / 2);

		for (char c : left)
		{
			if (GC_Algorithm::Contains(right, c))
			{
				if (GC_IsLower(c))
				{
					result += c - 'a' + 1;
				}
				else
				{
					result += c - 'A' + 27;
				}
				break;
			}
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day3)
{
	TEST_EQ(locPart1("AOC_Day3Test.txt"), 157);
	TEST_EQ(locPart1("AOC_Day3Part1.txt"), 7428);
}

static auto locPart2(char const* aFile)
{
	uint64 result = 0;

	GC_StrSlice group[3];

	auto lines = GC_File::ReadAllLines(aFile);
	// By line parsing
	for_index(auto line : lines)
	{
		if (i && (i % 3) == 0)
		{
			for (char c : group[0])
			{
				if (GC_Algorithm::Contains(group[1], c) && GC_Algorithm::Contains(group[2], c))
				{
					if (GC_IsLower(c))
					{
						result += c - 'a' + 1;
					}
					else
					{
						result += c - 'A' + 27;
					}
					break;
				}
			}
		}

		group[i % 3] = line;
	}

	for (char c : group[0])
	{
		if (GC_Algorithm::Contains(group[1], c) && GC_Algorithm::Contains(group[2], c))
		{
			if (GC_IsLower(c))
			{
				result += c - 'a' + 1;
			}
			else
			{
				result += c - 'A' + 27;
			}
			break;
		}
	}

	return result;
}

DEFINE_TEST_G(Part2, Day3)
{
	TEST_EQ(locPart2("AOC_Day3Test.txt"), 70);
	TEST_EQ(locPart2("AOC_Day3Part1.txt"), 2650);
}
