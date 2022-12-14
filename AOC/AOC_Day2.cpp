#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

	const char win[3] = { 1, 2, 0 };

	// By line parsing
	auto lines = GC_File::ReadAllLines(aFile);
	for (auto line : lines)
	{
		GC_StrSlice part;
		GC_Strtok(line, ' ', part);
		char a = part[0] - 'A';
		GC_Strtok(line, ' ', part);
		char c = part[0] - 'X';

		result += c + 1;
		if (a == c)
			result += 3;
		if (win[a] == c)
			result += 6;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day2)
{
	TEST_EQ(locPart1("AOC_Day2Test.txt"), 15);
	TEST_EQ(locPart1("AOC_Day2Part1.txt"), 10595);
}

static auto locPart2(char const* aFile)
{
	uint64 result = 0;

	const char win[3] = { 1, 2, 0 };

	// By line parsing
	auto lines = GC_File::ReadAllLines(aFile);
	for (auto line : lines)
	{
		GC_StrSlice part;
		GC_Strtok(line, ' ', part);
		char a = part[0] - 'A';
		GC_Strtok(line, ' ', part);
		char w = part[0] - 'X';
		char c = 0;

		switch (w)
		{
		case 0:
			c = (a + 2) % 3;
			break;
		case 1:
			c = a;
			break;
		case 2:
			c = win[a];
			break;
		}

		result += c + 1;
		if (a == c)
			result += 3;
		if (win[a] == c)
			result += 6;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day2)
{
	TEST_EQ(locPart2("AOC_Day2Test.txt"), 12);
	TEST_EQ(locPart2("AOC_Day2Part1.txt"), 9541);
}
