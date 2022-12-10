#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	bool grid[1000][1000] = {0};

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<5>(line, ' ');
		int off = parts.Count() == 5;
		auto tlp = GC_StrSplit<2>(parts[1 + off], ',');
		auto brp = GC_StrSplit<2>(parts[3 + off], ',');

		GC_Vector2u tl(GC_Atoi(tlp[0]), GC_Atoi(tlp[1]));
		GC_Vector2u br(GC_Atoi(brp[0]), GC_Atoi(brp[1]));

		if (line[1] == 'o')
		{
			for (uint x = tl.x; x <= br.x; ++x)
				for (uint y = tl.y; y <= br.y; ++y)
					grid[x][y] = !grid[x][y];
		}
		else if (parts[1][1] == 'n')
		{
			for (uint x = tl.x; x <= br.x; ++x)
				for (uint y = tl.y; y <= br.y; ++y)
					grid[x][y] = true;
		}
		else
		{
			for (uint x = tl.x; x <= br.x; ++x)
				for (uint y = tl.y; y <= br.y; ++y)
					grid[x][y] = false;
		}
	}

	uint64 result = 0;

	for_range2d(1000, 1000)
		result += grid[x][y];

	return result;
}

DEFINE_TEST_G(Part1, Day6)
{
	TEST_EQ(locPart1("AOC_Day6Test.txt"), 1000*1000 - 1004);
	TEST_EQ(locPart1("AOC_Day6Part1.txt"), 543903);
}

static auto locPart2(char const* aFile)
{
	GC_DynamicArray2D<uint> grid;
	grid.SetSize(1000, 1000);
	GC_MemZero(grid.Buffer(), grid.SizeInBytes());

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<5>(line, ' ');
		int off = parts.Count() == 5;
		auto tlp = GC_StrSplit<2>(parts[1 + off], ',');
		auto brp = GC_StrSplit<2>(parts[3 + off], ',');

		GC_Vector2u tl(GC_Atoi(tlp[0]), GC_Atoi(tlp[1]));
		GC_Vector2u br(GC_Atoi(brp[0]), GC_Atoi(brp[1]));

		if (line[1] == 'o')
		{
			for (uint x = tl.x; x <= br.x; ++x)
				for (uint y = tl.y; y <= br.y; ++y)
					grid(x,y) += 2;
		}
		else if (parts[1][1] == 'n')
		{
			for (uint x = tl.x; x <= br.x; ++x)
				for (uint y = tl.y; y <= br.y; ++y)
					grid(x, y) += 1;
		}
		else
		{
			for (uint x = tl.x; x <= br.x; ++x)
				for (uint y = tl.y; y <= br.y; ++y)
					grid(x, y) = grid(x, y) > 0 ? grid(x, y) - 1 : 0;
		}
	}

	uint64 result = 0;

	for (uint i : grid)
		result += i;

	return result;
}

DEFINE_TEST_G(Part2, Day6)
{
	TEST_EQ(locPart2("AOC_Day6Test.txt"), 1000*1000 + 2000 - 4);
	TEST_EQ(locPart2("AOC_Day6Part1.txt"), 0);
}
