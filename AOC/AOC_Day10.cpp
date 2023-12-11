#include "AOC_Precompiled.h"

struct Dirs
{
	GC_Cardinal D[2];

	static GC_HashMap<char, Dirs> GetValidDirs()
	{
		GC_HashMap<char, Dirs> ValidDirs;
		ValidDirs.Add('|', { GC_Cardinal::North, GC_Cardinal::South });
		ValidDirs.Add('-', { GC_Cardinal::East, GC_Cardinal::West });
		ValidDirs.Add('J', { GC_Cardinal::South, GC_Cardinal::West });
		ValidDirs.Add('L', { GC_Cardinal::South, GC_Cardinal::East });
		ValidDirs.Add('7', { GC_Cardinal::North, GC_Cardinal::West });
		ValidDirs.Add('F', { GC_Cardinal::North, GC_Cardinal::East });
		return ValidDirs;
	}
};

static auto locCalculateLoop(char const* aFile, GC_DynamicArray2D<int32>& costs, GC_DynamicArray2D<char>* mapOut = nullptr)
{
	GC_DynamicArray2D<char> mapData;
	GC_DynamicArray2D<char>& map = mapOut ? *mapOut : mapData;
	map = GC_File::Parse2d<char>(aFile, [](char c) { return c; });

	costs.SetSize(map.Size());
	GC_MemSet(costs.Buffer(), -1, costs.SizeInBytes());

	for_range2d(map.Width(), map.Height())
	{
		if (map(x, y) == 'S')
		{
			costs(x, y) = 0;

			char dirMap[GC_Cardinal::Count][3] =
			{
				{ '|', 'J', 'L' },
				{ '-', '7', 'J' },
				{ '|', 'F', '7' },
				{ '-', 'L', 'F' },
			};

			Dirs Start = { GC_Cardinal::Count, GC_Cardinal::Count };
			for (GC_Cardinal dirType : GC_Cardinal::Range())
			{
				GC_Vector2i dir = GC_Vector2i(x, y) + GC_CardinalVector(dirType);
				if (dir >= GC_Vector2i(0) && dir < map.Size())
				{
					const char pipe = map(dir);
					if (GC_Algorithm::Contains(dirMap[dirType], pipe))
					{
						if (Start.D[0] == GC_Cardinal::Count)
							Start.D[0] = dirType;
						else
							Start.D[1] = dirType;
					}
				}
			}

			GC_HashMap<char, Dirs> ValidDirs = Dirs::GetValidDirs();
			ValidDirs.Add('S', { Start });

			int max = 0;

			GC_DynamicArray<GC_Vector2u> searches;
			searches.Add({ x, y });

			while (searches.Count())
			{
				uint index = 0;
				int32 min = INT_MAX;

				for_index(GC_Vector2u p : searches)
				{
					if (costs(p) < min)
					{
						min = costs(p);
						index = i;
					}
				}

				const GC_Vector2i p = searches[index];
				searches.RemoveAtFast(index);

				const int32 curCost = costs(p);
				const char curPipe = map(p);
				for (GC_Cardinal dirType : ValidDirs[curPipe].D)
				{
					GC_Vector2i dir = p + GC_CardinalVector(dirType);
					if (dir >= GC_Vector2i(0) && dir < map.Size())
					{
						int32& nextCost = costs(dir);
						const char pipe = map(dir);
						if (nextCost == -1 && GC_Algorithm::Contains(dirMap[dirType], pipe))
						{
							nextCost = curCost + 1;
							max = GC_Max(max, curCost + 1);
							searches.Add(dir);
						}
					}
				}
			}

			return max;
		}
	}

	return 0;
}

static auto locPart1(char const* aFile)
{
	GC_DynamicArray2D<int32> costs;
	return locCalculateLoop(aFile, costs);
}

DEFINE_TEST_G(Part1, Day10)
{
	TEST_EQ(locPart1("AOC_Day10Test.txt"), 8);
	TEST_EQ(locPart1("AOC_Day10Part1.txt"), 7005);
}

static auto locPart2(char const* aFile)
{
	GC_DynamicArray2D<char> mapData;
	GC_DynamicArray2D<int32> originalCosts;
	int32 max = locCalculateLoop(aFile, originalCosts, &mapData) + 1;

	// Double the size of the playing field
	GC_DynamicArray2D<int32> costs;
	{
		GC_HashMap<char, Dirs> ValidDirs = Dirs::GetValidDirs();

		costs.SetSize(originalCosts.Size() * 2);

		for_range2d(originalCosts.Width(), originalCosts.Height())
		{
			costs(x * 2, y * 2) = originalCosts(x, y);
			costs(x * 2 + 1, y * 2) = -2;
			costs(x * 2, y * 2 + 1) = -2;
			costs(x * 2 + 1, y * 2 + 1) = -2;

			if (originalCosts(x, y) >= 0)
			{
				if (Dirs const* dirs = ValidDirs.Find(mapData(x, y)))
				{
					for (GC_Cardinal dirType : dirs->D)
					{
						GC_Vector2i dir = GC_Vector2i(x * 2, y * 2) + GC_CardinalVector(dirType);
						if (dir >= GC_Vector2i(0) && dir < costs.Size())
						{
							costs(dir) = originalCosts(x, y);
						}
					}
				}
			}
		}
	}

	GC_DynamicArray<bool> fills;
	fills.Resize(max + 1, false);

	GC_BitVector visited;
	visited.Resize(costs.Count());
	for_range2d(costs.Width(), costs.Height())
	{
		visited[x + y * costs.Width()] = costs(x, y) >= 0;
	}

	for_range2d(costs.Width(), costs.Height())
	{
		if (costs(x, y) < 0)
		{
			uint val = fills.Count();
			fills.Add(true);

			GC_Algorithm::FloodFillHeap<int32>(costs, visited, { x, y }, [&](int32& cost)
				{
					if (cost >= 0)
					{
						return false;
					}

					cost = cost == -1 ? val : max;
					GC_Vector2u pos = costs.IndexOf(cost);
					if (pos.x == 0 || pos.y == 0 || pos.x == costs.Width() - 1 || pos.y == costs.Height() - 1)
					{
						fills[val] = false;
					}

					return true;
				});
		}
	}

	return GC_Algorithm::Sum(costs.AsRange(), [&](uint c) { return (uint)(c < fills.Count() && fills[c]); });
}

DEFINE_TEST_G(Part2, Day10)
{
	TEST_EQ(locPart2("AOC_Day10Test1.txt"), 4);
	TEST_EQ(locPart2("AOC_Day10Test2.txt"), 8);
	TEST_EQ(locPart2("AOC_Day10Test3.txt"), 10);
	TEST_EQ(locPart2("AOC_Day10Part1.txt"), 417);
}
