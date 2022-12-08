#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	int64 result = 0;

	GC_String data = GC_File::ReadAllText(aFile);

	for (char c : data)
	{
		if (c == '(') ++result;
		else if (c == ')') --result;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day1)
{
	TEST_EQ(locPart1("AOC_Day1Test.txt"), -3);
	TEST_EQ(locPart1("AOC_Day1Part1.txt"), 280);
}

static int64 locPart2(char const* aFile)
{
	int64 result = 0;

	GC_String data = GC_File::ReadAllText(aFile);

	for_index(char c : data)
	{
		if (c == '(') ++result;
		else if (c == ')') --result;

		if (result < 0)
			return i + 1;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day1)
{
	TEST_EQ(locPart2("AOC_Day1Test.txt"), 1);
	TEST_EQ(locPart2("AOC_Day1Part1.txt"), 1797);
}
