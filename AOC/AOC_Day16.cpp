#include "AOC_Precompiled.h"

struct Tile
{
	char c;
	uint energized = 0;
};

static uint64 locCalcEnergy(GC_DynamicArray2D<Tile>& map, GC_Vector2u start = { 0,0 }, GC_Cardinal startDir = GC_Cardinal::East)
{
	GC_DynamicArray<GC_Pair<GC_Vector2i, GC_Cardinal>> Beams;
	Beams.Add({ start - GC_CardinalDirection(startDir) * GC_Vector2i(1, -1), startDir });

	while (Beams.Count())
	{
		GC_Pair<GC_Vector2i, GC_Cardinal> beam = Beams.Last();
		Beams.PopBack();

		GC_Vector2i pos = beam.First;
		GC_Cardinal dir = beam.Second;

		while (1)
		{
			pos += GC_CardinalDirection(dir) * GC_Vector2i(1, -1);

			if (pos.x < 0 || pos.y < 0 || pos.x == (int)map.Width() || pos.y == (int)map.Height())
				break;

			Tile& t = map(pos);
			if (t.energized & (1 << dir))
				break;

			t.energized |= (1 << dir);

			switch (t.c)
			{
			case '-':
				if (dir == GC_Cardinal::North || dir == GC_Cardinal::South)
				{
					dir = GC_Cardinal::West;
					Beams.Add({ pos, GC_Cardinal::East });
				}
				break;
			case '|':
				if (dir == GC_Cardinal::West || dir == GC_Cardinal::East)
				{
					dir = GC_Cardinal::North;
					Beams.Add({ pos, GC_Cardinal::South });
				}
				break;
			case '/':
				if (dir == GC_Cardinal::North || dir == GC_Cardinal::South)
					dir = GC_CardinalRotateRight(dir);
				else
					dir = GC_CardinalRotateLeft(dir);
				break;
			case '\\':
				if (dir == GC_Cardinal::North || dir == GC_Cardinal::South)
					dir = GC_CardinalRotateLeft(dir);
				else
					dir = GC_CardinalRotateRight(dir);
				break;
			}
		}
	}

	uint64 result = 0;

	for (Tile const& t : map)
	{
		if (t.energized)
			++result;
	}

	return result;
}

static auto locPart1(char const* aFile)
{
	GC_DynamicArray2D<Tile> map = GC_File::Parse2d<Tile>(aFile, [](char c) -> Tile { return { c, 0 }; });
	return locCalcEnergy(map);
}

DEFINE_TEST_G(Part1, Day16)
{
	TEST_EQ(locPart1("AOC_Day16Test.txt"), 46);
	TEST_EQ(locPart1("AOC_Day16Part1.txt"), 7210);
}

static uint64 locClearAndCalcEnergy(GC_DynamicArray2D<Tile>& map, GC_Vector2u start, GC_Cardinal startDir)
{
	for (Tile& t : map)
		t.energized = 0;

	return locCalcEnergy(map, start, startDir);
}

static auto locPart2(char const* aFile)
{
	GC_DynamicArray2D<Tile> map = GC_File::Parse2d<Tile>(aFile, [](char c) -> Tile { return { c, 0 }; });

	uint64 result = 0;

	for_range(map.Width())
	{
		result = GC_Max(result, locClearAndCalcEnergy(map, { i, 0 }, GC_Cardinal::South));
		result = GC_Max(result, locClearAndCalcEnergy(map, { i, map.Height() - 1}, GC_Cardinal::North));
	}

	for_range(map.Height())
	{
		result = GC_Max(result, locClearAndCalcEnergy(map, { 0, i }, GC_Cardinal::East));
		result = GC_Max(result, locClearAndCalcEnergy(map, { map.Width() - 1, i}, GC_Cardinal::West));
	}

	return result;
}

DEFINE_TEST_G(Part2, Day16)
{
	TEST_EQ(locPart2("AOC_Day16Test.txt"), 51);
	TEST_EQ(locPart2("AOC_Day16Part1.txt"), 7673);
}
