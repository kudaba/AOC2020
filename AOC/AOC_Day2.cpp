#include "AOC_Precompiled.h"

static auto locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<GC_Vector2i>(aFile, [](auto aLine)
		{
			auto parts = GC_StrSplit(aLine, " ");
			return GC_Atoi(parts[1]) *
				// Totally stole this from someone elses solutionand cleaned mine up
				(parts[0][0] == 'f' ? GC_Vector2i { 1, 0 } :
				parts[0][0] == 'd' ? GC_Vector2i { 0, 1 } : GC_Vector2i { 0, -1 });
		});
}

static uint locPart1(char const* aFile)
{
	GC_Vector2i sum (0);
	for (auto item : locParseData(aFile))
	{
		sum += item;
	}
	return sum.x * sum.y;
}

DEFINE_TEST_G(Part1, Day2)
{
	TEST_EQ(locPart1("AOC_Day2Test.txt"), 150);
	TEST_EQ(locPart1("AOC_Day2Part1.txt"), 1893605);
}

static uint locPart2(char const* aFile)
{
	GC_Vector3i sum (0);

	for (auto item : locParseData(aFile))
	{
		if (item.y)
			sum.z += item.y;
		else
		{
			sum.x += item.x;
			sum.y += sum.z * item.x;
		}
	}

	return sum.x * sum.y;
}

DEFINE_TEST_G(Part2, Day2)
{
	TEST_EQ(locPart2("AOC_Day2Test.txt"), 900);
	TEST_EQ(locPart2("AOC_Day2Part1.txt"), 2120734350);
}
