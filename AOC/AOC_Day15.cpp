#include "AOC_Precompiled.h"

struct Tile
{
	char Cost : 6;
	char Visited : 2;
};

static uint locGetLowestCost(GC_DynamicArray2D<Tile>& data)
{
	return RunDijsktraShortStepV<GC_Vector2u>({ 0,0 }, data.Size() - GC_Vector2u(1),
		[&](GC_Vector2u const& candidate) { bool visited = data(candidate).Visited; data(candidate).Visited = true; return visited; },
		[&](GC_Vector2u const& candidate, auto const& anAddToQueue)
		{
			for (auto dir : GC_Cardinal::Range())
			{
				GC_Vector2u next = GC_Vector2u(candidate) + GC_CardinalDirection(dir);
				if (next < data.Size())
				{
					auto const& ntile = data(next);
					if (!ntile.Visited)
						anAddToQueue(next, ntile.Cost);
				}
			}
		}, data.Size().Area()).GetDefault(0);
}

static void locExpandBoard(GC_DynamicArray2D<Tile>& data, uint extraSize)
{
	if (extraSize)
	{
		auto size = data.Size();
		data.SetSize(size * extraSize);

		auto initArea = data.Area(GC_Vector2u(0), size);
		for_range2d(extraSize, extraSize)
			if (x || y)
				GC_ArrayOperations2D::Splat(data.Area(GC_Rectu::FromPointSize(size * GC_Vector2u(x, y), size)), initArea);

		for (uint i = size.x, e = data.Width(); i < e; ++i)
		{
			char add = char(i / size.x);
			for (auto& t : data.Row(i))
			{
				t.Cost += add;
				if (t.Cost > 9)
					t.Cost -= 9;
			}
			for (auto& t : data.Column(i))
			{
				t.Cost += add;
				if (t.Cost > 9)
					t.Cost -= 9;
			}
		}
	}
}

static auto locPart1(char const* aFile, uint extraSize = 0)
{
	auto data = GC_File::Parse2d<Tile>(aFile, [](char c) { return Tile{ c - '0', false }; });
	locExpandBoard(data, extraSize);
	return locGetLowestCost(data);
}

DEFINE_TEST_G(Part1, Day15)
{
	TEST_EQ(locPart1("AOC_Day15Test.txt"), 40);
	TEST_EQ(locPart1("AOC_Day15Part1.txt"), 423);
}

DEFINE_TEST_G(Part2, Day15)
{
	TEST_EQ(locPart1("AOC_Day15Test.txt", 5), 315);
	TEST_EQ(locPart1("AOC_Day15Part1.txt", 5), 2778);
}
