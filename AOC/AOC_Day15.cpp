#include "AOC_Precompiled.h"

struct Tile
{
	int Cost;
	uint MaxCost;
};

static void locGetLowestCost(GC_DynamicArray2D<Tile>& data, GC_Vector2u pos, GC_Vector2u lastPos, uint cost, uint& maxcost, uint& iterations)
{
	++iterations;
	Tile& t = data(pos);
	if (t.MaxCost <= cost) return;
	t.MaxCost = cost;

	auto distance = data.Size() - pos - GC_Vector2u(1);
	if (maxcost > cost + (distance.x + distance.y) * 2)
	{
		if (pos == data.Size() - GC_Vector2u(1))
		{
			if (cost < maxcost)
			{
				maxcost = cost;
			}
		}
		else
		{
			for (auto dir : GC_Cardinal::Range())
			{
				GC_Vector2u next = pos + GC_CardinalDirection(dir);
				if (next != lastPos && next < data.Size())
					locGetLowestCost(data, next, pos, cost + data(next).Cost, maxcost, iterations);
			}
		}
	}
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
	auto data = GC_File::Parse2d<Tile>(aFile, [](char c) { return Tile{ c - '0', UINT_MAX }; });
	locExpandBoard(data, extraSize);

	// prime data with a max path equivalent.
	// including the cost of tile 0,0 allows the algorithm to run through paths at least once without failing
	uint ycost = 0;
	for_range_v(y, data.Height())
	{
		uint xcost = 0;
		for_range_v(x, data.Width())
		{
			auto& t = data(x, y);
			xcost += t.Cost;
			t.MaxCost = GC_Min(t.MaxCost, ycost + xcost);
		}
		ycost += data(0, y).Cost;
	}

	uint maxCost = GC_Algorithm::Sum(data.Row(0), [](auto& c) { return c.Cost; }) + GC_Algorithm::Sum(data.Column(data.Width() - 1), [](auto& c) { return c.Cost; });
	uint iterations = 0;
	locGetLowestCost(data, GC_Vector2u(0), GC_Vector2u(0), 0, maxCost, iterations);
	TestFixture::GetCurrentTest()->Printf("iters: %d\n", iterations);
	return maxCost;
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
