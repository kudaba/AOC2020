#include "AOC_Precompiled.h"

struct Tile
{
	char Cost : 6;
	char Visited : 2;
};

static uint locGetLowestCost(GC_DynamicArray2D<Tile>& data)
{
	auto const end = data.Size() - GC_Vector2u(1);
	GC_HashMap<uint, GC_HybridArray<GC_Vector2u, 32>> queue;
	queue.Reserve(data.Size().Area());
	queue.Add(0, GC_HybridArray<GC_Vector2u, 32>(1, { 0,0 }));

	uint iterations = 0;
	uint cost = 0;

	while (queue.Count())
	{
		GC_HybridArray<GC_Vector2u, 32>* costQueue = queue.Find(cost);
		while (!costQueue)
		{
			++cost;
			costQueue = queue.Find(cost);
		}
		++iterations;

		auto const candidate = costQueue->Last();
		costQueue->PopBack();
		if (costQueue->IsEmpty())
			queue.Remove(cost);

		auto& tile = data(candidate);

		if (tile.Visited) continue;
		tile.Visited = true;

		if (end == candidate)
		{
			TestFixture::GetCurrentTest()->Printf("iters: %d\n", iterations);
			return cost;
		}

		for (auto dir : GC_Cardinal::Range())
		{
			GC_Vector2u next = GC_Vector2u(candidate) + GC_CardinalDirection(dir);
			if (next < data.Size())
			{
				auto const& ntile = data(next);
				if (!ntile.Visited)
					queue.GetOrAdd(cost + ntile.Cost).Add(next);
			}
		}
	}
	return 0;
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
