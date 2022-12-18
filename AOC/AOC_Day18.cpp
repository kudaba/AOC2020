#include "AOC_Precompiled.h"

static auto locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<GC_Vector3u>(aFile, [](auto aLine)
		{
			GC_Vector3u cube = {};
			for_index(auto part : GC_Tokenize(aLine, ','))
				cube[i] = GC_Atoi(part);
			return cube;
		});
}

static auto locPart1(char const* aFile, bool doFill = false)
{
	auto cubes = locParseData(aFile);

	char grid[32][32][32] = {};

	GC_Vector3u min(UINT_MAX);
	GC_Vector3u max(0);
	for (auto c : cubes)
	{
		c += GC_Vector3u(1);
		grid[c.x][c.y][c.z] = '#';
		min = GC_Min(min, c);
		max = GC_Max(max, c);
	}

	GC_DynamicArray<GC_Vector3u> fill;
	fill.Reserve(max.x * max.y * max.z);
	fill.Add(max + GC_Vector3u(1));

	max += GC_Vector3u(2);

	if (doFill)
	{
		GC_Vector3u dirs[] =
		{
			GC_Vector3u::Up, GC_Vector3u::Down, GC_Vector3u::Left, GC_Vector3u::Right, GC_Vector3u::Forward, GC_Vector3u::Reverse
		};

		while (fill.Count())
		{
			GC_Vector3u c = fill.Last();
			fill.PopBack();

			if (grid[c.x][c.y][c.z] == 0)
			{
				grid[c.x][c.y][c.z] = '.';

				for (auto d : dirs)
				{
					GC_Vector3u nc = c + d;
					if (GC_InRange(nc, min - GC_Vector3u(1), max) && grid[nc.x][nc.y][nc.z] == 0)
						fill.Add(nc);
				}
			}
		}
	}

	uint64 result = 0;

	for_range(3)
	{
		uint xc = i;
		uint yc = (i + 1) % 3;
		uint zc = (i + 2) % 3;

		for (uint x = min[xc]; x <= max[xc]; ++x)
			for (uint y = min[yc]; y <= max[yc]; ++y)
			{
				char covered = doFill ? '.' : 0;
				char next = '#';
				GC_Vector3u c = {};
				c[xc] = x;
				c[yc] = y;

				for (uint z = min[zc]; z <= max[zc]; ++z)
				{
					c[zc] = z;
					if (grid[c.x][c.y][c.z] == next)
					{
						++result;
						GC_Swap(covered, next);
					}
				}

				if (covered == '#')
					++result;
			}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day18)
{
	TEST_EQ(locPart1("AOC_Day18Test0.txt"), 10);
	TEST_EQ(locPart1("AOC_Day18Test.txt"), 64);
	TEST_EQ(locPart1("AOC_Day18Part1.txt"), 3586);
}

DEFINE_TEST_G(Part2, Day18)
{
	TEST_EQ(locPart1("AOC_Day18Test.txt", true), 58);
	TEST_EQ(locPart1("AOC_Day18Part1.txt", true), 2072);
}
