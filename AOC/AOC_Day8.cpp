#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 length = 0;
	uint64 result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		length += line.Count();

		auto sub = line.Range(1, line.Count() - 2);
		for_range(sub.Count())
		{
			if (sub[i] == '\\')
			{
				if (sub[i + 1] == 'x')
					i += 3;
				else
					++i;
			}
			++result;
		}
	}

	return length - result;
}

DEFINE_TEST_G(Part1, Day8)
{
	TEST_EQ(locPart1("AOC_Day8Test.txt"), 12);
	TEST_EQ(locPart1("AOC_Day8Part1.txt"), 1371);
}

static auto locPart2(char const* aFile)
{
	uint64 length = 0;
	uint64 result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		length += line.Count();
		result += 6;

		auto sub = line.Range(1, line.Count() - 2);
		for (char c : sub)
		{
			++result;

			if (c == '\"')
				++result;
			else if (c == '\\')
				++result;
		}
	}

	return result - length;
}

DEFINE_TEST_G(Part2, Day8)
{
	TEST_EQ(locPart2("AOC_Day8Test.txt"), 19);
	TEST_EQ(locPart2("AOC_Day8Part1.txt"), 2117);
}
