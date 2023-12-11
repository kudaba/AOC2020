#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, uint anExpansion = 1)
{
	GC_BitVector rows;
	GC_BitVector cols;

	GC_DynamicArray<GC_Vector2u> positions;

	{
		for_index_v(y, auto line : GC_File::ReadAllLines(aFile))
		{
			if (!y)
			{
				rows.Resize(line.Count());
				cols.Resize(line.Count());
			}

			for_index_v(x, char c : line)
			{
				if (c == '#')
				{
					positions.Add({x, y});
					rows[y] = true;
					cols[x] = true;
				}
			}
		}
	}

	GC_DynamicArray<uint> remapx;
	GC_DynamicArray<uint> remapy;

	remapx.Resize(cols.Count());
	remapy.Resize(rows.Count());

	uint x = 0;
	uint y = 0;
	for_range(cols.Count())
	{
		remapx[i] = x;
		remapy[i] = y;

		x += cols[i] ? 1 : anExpansion;
		y += rows[i] ? 1 : anExpansion;
	}

	for(GC_Vector2u& p : positions)
	{
		p.x = remapx[p.x];
		p.y = remapy[p.y];
	}

	uint64 result = 0;

	for_index(GC_Vector2u const& p1 : positions)
	{
		for (int32 j = i + 1, je = positions.Count(); j < je; ++j)
		{
			GC_Vector2i diff = positions[j] - p1;
			result += GC_Abs(diff.x) + GC_Abs(diff.y);
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day11)
{
	TEST_EQ(locPart1("AOC_Day11Test.txt", 2), 374);
	TEST_EQ(locPart1("AOC_Day11Part1.txt", 2), 10494813);
}

DEFINE_TEST_G(Part2, Day11)
{
	TEST_EQ(locPart1("AOC_Day11Test.txt", 10), 1030);
	TEST_EQ(locPart1("AOC_Day11Test.txt", 100), 8410);
	TEST_EQ(locPart1("AOC_Day11Part1.txt", 1000000), 840988812853);
}
