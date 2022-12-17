#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	auto map = GC_File::Parse2d<char>(aFile, [](char c) { return c; });
	GC_Vector2i start(0), end(0);
	for (char& c : map)
	{
		if (c == 'S')
		{
			start = map.IndexOf(c);
			c = 'a';
		}
		else if (c == 'E')
		{
			end = map.IndexOf(c);
			c = 'z';
		}
	}

	return RunDijsktraShortStep(start, end, [&](auto pos, auto add)
	{
		for (auto dire : GC_Cardinal::Range())
		{
			GC_Vector2i next = pos + GC_CardinalVector(dire);
			if (GC_InArrayRange<uint>(next.x, 0, map.Width()) &&
				GC_InArrayRange<uint>(next.y, 0, map.Height()) &&
				map(next) - map(pos) <= 1)
			{
				add(next, 1);
			}
		}
	}).GetDefault(0);
}

DEFINE_TEST_G(Part1, Day12)
{
	TEST_EQ(locPart1("AOC_Day12Test.txt"), 31);
	TEST_EQ(locPart1("AOC_Day12Part1.txt"), 350);
}

static auto locPart2(char const* aFile)
{
	auto map = GC_File::Parse2d<char>(aFile, [](char c) { return c; });
	GC_Vector2u end(0);
	for (char& c : map)
	{
		if (c == 'S')
		{
			c = 'a';
		}
		else if (c == 'E')
		{
			end = map.IndexOf(c);
			c = 'z';
		}
	}

	struct Pos
	{
		GC_Vector2u P;
		int c; // no padding

		bool operator==(Pos other) const { return c == 'a' || (P == other.P); }
	};

	return RunDijsktraShortStep(Pos { end, 'z' }, Pos {map.Size(), 0}, [&](auto pos, auto add)
		{
			for (auto dire : GC_Cardinal::Range())
			{
				GC_Vector2i next = pos.P + GC_CardinalVector(dire);
				if (GC_InArrayRange<uint>(next.x, 0, map.Width()) &&
					GC_InArrayRange<uint>(next.y, 0, map.Height()) &&
					map(pos.P) - map(next) <= 1)
				{
					add({next, map(next) }, 1);
				}
			}
		}).GetDefault(0);
}

DEFINE_TEST_G(Part2, Day12)
{
	TEST_EQ(locPart2("AOC_Day12Test.txt"), 29);
	TEST_EQ(locPart2("AOC_Day12Part1.txt"), 349);
}
