#include "AOC_Precompiled.h"
#include "AOC_Day24.h"

static uint locDay24(char const* aFile, uint numDays)
{
	GC_HashSet<GC_Vector2i> blackTiles;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		GC_Vector2i point(0);
		for (char const* b = line.begin(); b != line.end(); ++b)
		{
			switch (*b)
			{
			case 'e':
				point.x += 1;
				break;
			case 'w':
				point.x -= 1;
				break;
			case 's':
				if (b[1] == 'e')
					point += GC_Vector2i(1, -1);
				else
					point += GC_Vector2i(0, -1);
				++b;
				break;
			case 'n':
				if (b[1] == 'e')
					point += GC_Vector2i(0, 1);
				else
					point += GC_Vector2i(-1, 1);
				++b;
				break;
			}
		}

		if (blackTiles.Contains(point))
			blackTiles.Remove(point);
		else
			blackTiles.Add(point);
	}

	GC_Vector2i directions[] =
	{
		{1,0},
		{-1,0},
		{0,1},
		{-1,1},
		{0,-1},
		{1,-1},
	};

	GC_HashMap<GC_Vector2i, uint> counts;
	for_range(numDays)
	{
		counts.Clear();

		for (GC_Vector2i t : blackTiles)
		{
			for (auto v : directions)
			{
				auto count = counts.Add(t + v);
				if (count.myFirst)
					*count.mySecond = 1;
				else
					++(*count.mySecond);
			}

			auto count = counts.Add(t);
			if (count.myFirst)
				*count.mySecond = 0;
		}

		for (auto v : counts)
		{
			if (v.myValue == 2)
				blackTiles.Add(v.myKey);
			else if (v.myValue != 1)
				blackTiles.Remove(v.myKey);
		}
	}

	return blackTiles.Count();
}

DEFINE_TEST_G(Part1, Day24)
{
	TEST_EQ(locDay24("AOC_Day24Test.txt", 0), 10);
	TEST_EQ(locDay24("AOC_Day24Part1.txt", 0), 263);
}

DEFINE_TEST_G(Part2, Day24)
{
	TEST_EQ(locDay24("AOC_Day24Test.txt", 100), 2208);
	TEST_EQ(locDay24("AOC_Day24Part1.txt", 100), 3649);
}
