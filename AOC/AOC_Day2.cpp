#include "AOC_Precompiled.h"
#include "AOC_Day2.h"

static uint locGetValidPW(char const* data)
{
	uint valid = 0;

	GC_StrSlice line;
	while (GC_StrLine(data, line))
	{
		int min, max;
		char c;
		char pw[32];
		_snscanf(line.Buffer(), line.Count(), "%d-%d %c: %31s", &min, &max, &c, pw);

		int count = 0;
		for (char ch : pw)
		{
			if (!ch) break;
			if (ch == c)
				++count;
		}

		if (GC_InRange(count, min, max))
			++valid;
	}
	return valid;
}

DEFINE_TEST_G(Part1, Day2)
{
	TEST_EQ(locGetValidPW(testData), 2);
	GC_String text;
	GC_File::ReadAllText("AOC_Day2Part1.txt", text);
	TEST_EQ(locGetValidPW(text), 396);
}

static uint locGetValidPW2(char const* data)
{
	uint valid = 0;

	GC_StrSlice line;
	while (GC_StrLine(data, line))
	{
		int min, max;
		char c;
		char pw[32];
		_snscanf(line.Buffer(), line.Count(), "%d-%d %c: %31s", &min, &max, &c, pw);

		if ((pw[min - 1] == c) != (pw[max - 1] == c))
			++valid;
	}
	return valid;
}

DEFINE_TEST_G(Part2, Day2)
{
	TEST_EQ(locGetValidPW2(testData), 1);
	GC_String text;
	Printf("%s\n", GC_Path::GetAbsolutePath("Day2Part1.txt").ToString());
	GC_File::ReadAllText("AOC_Day2Part1.txt", text);
	TEST_EQ(locGetValidPW2(text), 428);
}
