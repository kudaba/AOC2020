#include "AOC_Precompiled.h"
#include "AOC_Day12.h"
#include "GC_CardinalDirections.h"

static uint locDay12Part1(char const* aFile)
{
	GC_Cardinal dir = GC_Cardinal::East;
	GC_Vector2i pos(0);

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		switch (line[0])
		{
		case 'N':
			pos.y += GC_Atoi(line.Range(1));
			break;
		case 'E':
			pos.x += GC_Atoi(line.Range(1));
			break;
		case 'W':
			pos.x -= GC_Atoi(line.Range(1));
			break;
		case 'S':
			pos.y -= GC_Atoi(line.Range(1));
			break;
		case 'F':
			pos += GC_CardinalDirection(dir) * GC_Atoi(line.Range(1));
			break;
		case 'R':
		case 'L':
		{
			switch (GC_Atoi(line.Range(1)))
			{
			case 90:
				if (line[0] == 'R')
					dir = GC_CardinalRotateRight(dir);
				else
					dir = GC_CardinalRotateLeft(dir);
				break;
			case 180:
				dir = GC_CardinalOpposite(dir);
				break;
			case 270:
				if (line[0] == 'L')
					dir = GC_CardinalRotateRight(dir);
				else
					dir = GC_CardinalRotateLeft(dir);
				break;
			}
		}
		}
	}

	return GC_Abs(pos.x) + GC_Abs(pos.y);
}

DEFINE_TEST_G(Part1, Day12)
{
	TEST_EQ(locDay12Part1("AOC_Day12Test.txt"), 25);
	TEST_EQ(locDay12Part1("AOC_Day12Part1.txt"), 636);
}

static uint locDay12Part2(char const* aFile)
{
	GC_Cardinal dir = GC_Cardinal::East;
	GC_Vector2i pos(0);
	GC_Vector2i wpt(10, 1);

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		switch (line[0])
		{
		case 'N':
			wpt.y += GC_Atoi(line.Range(1));
			break;
		case 'E':
			wpt.x += GC_Atoi(line.Range(1));
			break;
		case 'W':
			wpt.x -= GC_Atoi(line.Range(1));
			break;
		case 'S':
			wpt.y -= GC_Atoi(line.Range(1));
			break;
		case 'F':
			pos += wpt * GC_Atoi(line.Range(1));
			break;
		case 'R':
		case 'L':
		{
			switch (GC_Atoi(line.Range(1)))
			{
			case 90:
				if (line[0] == 'R')
					wpt.Set(wpt.y, -wpt.x);
				else
					wpt.Set(-wpt.y, wpt.x);
				break;
			case 180:
				wpt = -wpt;
				break;
			case 270:
				if (line[0] == 'L')
					wpt.Set(wpt.y, -wpt.x);
				else
					wpt.Set(-wpt.y, wpt.x);
				break;
			}
		}
		}
	}

	return GC_Abs(pos.x) + GC_Abs(pos.y);
}

DEFINE_TEST_G(Part2, Day12)
{
	TEST_EQ(locDay12Part2("AOC_Day12Test.txt"), 286);
	TEST_EQ(locDay12Part2("AOC_Day12Part1.txt"), 26841);
}
