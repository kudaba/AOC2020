#include "AOC_Precompiled.h"

GC_DECLARE_ENUM(CubeColor, red, green, blue);

static auto locPart1(char const* aFile)
{
	int Counts[] =
	{
		12, 13, 14
	};

	uint64 result = 0;

	// By line parsing
	for_index(auto line : GC_File::ReadAllLines(aFile))
	{
		auto game = GC_StrSplit<6>(line, ": ");
		bool valid = true;

		for (auto pair : GC_StrSplit<6>(game[1], "; "))
		{
			for (auto part : GC_StrSplit<2>(pair, ", "))
			{
				auto cube = GC_StrSplit<2>(part, " ");
				if (GC_Atoi(cube[0]) > Counts[CubeColor::FromString(GC_String(cube[1]))])
				{
					valid = false;
					break;
				}
			}
		}

		if (valid)
		{
			result += i + 1;
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day2)
{
	TEST_EQ(locPart1("AOC_Day2Test.txt"), 8);
	TEST_EQ(locPart1("AOC_Day2Part1.txt"), 2913);
}

static auto locPart2(char const* aFile)
{
	uint64 result = 0;

	// By line parsing
	for(auto line : GC_File::ReadAllLines(aFile))
	{
		int Counts[] =
		{
			0, 0, 0
		};

		auto game = GC_StrSplit<6>(line, ": ");

		for (auto pair : GC_StrSplit<6>(game[1], "; "))
		{
			for (auto part : GC_StrSplit<2>(pair, ", "))
			{
				auto cube = GC_StrSplit<2>(part, " ");
				CubeColor color = CubeColor::FromString(GC_String(cube[1]));
				Counts[color] = GC_Max(Counts[color], GC_Atoi(cube[0]));
			}
		}

		result += Counts[0] * Counts[1] * Counts[2];
	}

	return result;
}

DEFINE_TEST_G(Part2, Day2)
{
	TEST_EQ(locPart2("AOC_Day2Test.txt"), 2286);
	TEST_EQ(locPart2("AOC_Day2Part1.txt"), 55593);
}
