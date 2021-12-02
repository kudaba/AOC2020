#include "AOC_Precompiled.h"

static auto locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<GC_Vector2i>(aFile, [](auto aLine, auto& anItem)
		{
			GC_StrSlice part;
			GC_Strtok(aLine, " ", part);
			if (GC_Strneq(part.begin(), "forward", part.Count()))
				anItem = { 1, 0 };
			else if (GC_Strneq(part.begin(), "down", part.Count()))
				anItem = { 0, 1 };
			else if (GC_Strneq(part.begin(), "up", part.Count()))
				anItem = { 0, -1 };
			else
				anItem = { -1, 0 };

			GC_Strtok(aLine, " ", part);
			anItem *= GC_Atoi(part);
			return true;
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
