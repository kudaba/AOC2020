#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	auto lines = GC_File::ReadAllLines(aFile);
	GC_Vector2u end(lines[0].Count() - 1, lines.Count() - 1);

	uint total = 0;

	// By line parsing
	for_index_v(y, auto line : lines)
	{
		for_index_v(x, auto c : line)
		{
			if (y && c >= lines[y-1][x])
				continue;
			if (x && c >= lines[y][x-1])
				continue;
			if (y < end.y && c >= lines[y+1][x])
				continue;
			if (x < end.x && c >= lines[y][x+1])
				continue;

			total += c - '0' + 1;
		}
	}

	return total;
}

DEFINE_TEST_G(Part1, Day9)
{
	TEST_EQ(locPart1("AOC_Day9Test.txt"), 15);
	TEST_EQ(locPart1("AOC_Day9Part1.txt"), 480);
}

static uint locGetBasinSize(GC_File::FileLines const& lines, GC_DynamicArray2D<bool>& visited, GC_Vector2i pos)
{
	if (visited(pos) || lines[pos.y][pos.x] == '9')
		return 0;

	visited(pos) = true;

	uint neighbors = 1;

	for (auto dir : GC_Cardinal::Range())
	{
		GC_Vector2i np = pos + GC_CardinalDirection(dir);
		if (!(np.x < 0 || np.y < 0 || np.x == (int)visited.Size().x || np.y == (int)visited.Size().y))
		{
			neighbors += locGetBasinSize(lines, visited, np);
		}
	}

	return neighbors;
}

static auto locPart2(char const* aFile)
{
	auto lines = GC_File::ReadAllLines(aFile);
	auto data = GC_ArrayRange2D<char const>(lines[0].begin(), { lines[0].Count(), lines.Count() }, lines[0].Count() + 1);

	uint biggest[3] = {};

	// By line parsing
	for_index_v(y, auto line : lines)
	{
		for_index_v(x, auto c : line)
		{
			if ((x && c >= data(x - 1, y)) ||
				(y && c >= data(x, y - 1)) ||
				(x < data.Width() - 1 && c >= data(x + 1, y)) ||
				(y < data.Height() - 1 && c >= data(x, y + 1)))
				continue;

			uint basinSize = 0;
			GC_Algorithm::FloodFill(data, GC_Vector2i(x, y), [&](char c) { if (c == '9') return false; ++basinSize; return true; });

			uint& min1 = biggest[0] < biggest[1] ? biggest[0] : biggest[1];
			uint& min = min1 < biggest[2] ? min1 : biggest[2];

			if (basinSize > min)
				min = basinSize;
		}
	}


	return biggest[0] * biggest[1] * biggest[2];
}

DEFINE_TEST_G(Part2, Day9)
{
	TEST_EQ(locPart2("AOC_Day9Test.txt"), 1134);
	TEST_EQ(locPart2("AOC_Day9Part1.txt"), 1045660);
}
