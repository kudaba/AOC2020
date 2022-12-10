#include "AOC_Precompiled.h"
#include "GC_StringBuilder.h"

static auto locPart1(char const* aFile)
{
	int64 result = 0;
	int64 cycle = 0;
	int64 X = 1;
	int64 next = 20;
	int64 nextLine = 40;

	GC_StringBuilderLarge output;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		int64 nextCycle = cycle + 1;
		int64 nextX = X;

		if (line[0] != 'n')
		{
			++nextCycle;
			nextX += GC_Atoi(line.Range(5));
		}

		while (cycle < nextCycle)
		{
			output.Append((GC_Abs((cycle % 40) - X) <= 1) ? "#" : ".");

			++cycle;

			if (cycle == next)
			{
				result += X * next;
				next += 40;
			}

			if (cycle == nextLine)
			{
				output.Append("\n");
				nextLine += 40;
			}
		}

		X = nextX;
	}

	output.Append("\n");
	printf(output.ToString());

	return result;
}

DEFINE_TEST_G(Part1, Day10)
{
	TEST_EQ(locPart1("AOC_Day10Test.txt"), 13140);
	TEST_EQ(locPart1("AOC_Day10Part1.txt"), 15680);
}
