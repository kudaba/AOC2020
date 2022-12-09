#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	GC_String data = GC_File::ReadAllText(aFile);

	GC_Vector2i pos(0);
	GC_HashSet<GC_Vector2i> delivered;
	delivered.Add(pos);

	uint64 result = 1;

	for (char c : data)
	{
		switch (c)
		{
		case '^': pos.y += 1; break;
		case 'v': pos.y -= 1; break;
		case '>': pos.x += 1; break;
		case '<': pos.x -= 1; break;
		}

		if (delivered.Add(pos))
			++result;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day3)
{
	TEST_EQ(locPart1("AOC_Day3Test.txt"), 2);
	TEST_EQ(locPart1("AOC_Day3Part1.txt"), 2592);
}

static auto locPart2(char const* aFile)
{
	GC_String data = GC_File::ReadAllText(aFile);

	GC_Vector2i pos1(0);
	GC_Vector2i pos2(0);
	GC_HashSet<GC_Vector2i> delivered;
	delivered.Add(pos1);

	uint64 result = 1;

	for_index(char c : data)
	{
		GC_Vector2i& pos = (i&1) ? pos1 : pos2;
		switch (c)
		{
		case '^': pos.y += 1; break;
		case 'v': pos.y -= 1; break;
		case '>': pos.x += 1; break;
		case '<': pos.x -= 1; break;
		}

		if (delivered.Add(pos))
			++result;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day3)
{
	TEST_EQ(locPart2("AOC_Day3Test.txt"), 11);
	TEST_EQ(locPart2("AOC_Day3Part1.txt"), 2360);
}
