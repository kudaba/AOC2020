#include "AOC_Precompiled.h"
#include "AOC_Day11.h"
#include "GC_DynamicArray2D.h"
#include "GC_CardinalDirections.h"

template <typename Type>
constexpr bool GC_InRangeUnSafe(Type aValue, Type aMin, Type aMax)
{
	return aValue >= aMin && aValue <= aMax;
}

static uint locDay11Part1(char const* aFile)
{
	auto lines = GC_File::ReadAllLines(aFile);
	GC_DynamicArray2D<char> map;

	uint const w = lines[0].Count();
	uint const h = lines.Count();

	map.SetSize({ w, h });
	for_index(auto line : lines)
	{
		GC_Memcpy(map.Row(i).Buffer(), line.Buffer(), w);
	}

	GC_Vector2i const r0(0);
	GC_Vector2i const r1(w-1, h-1);

	GC_DynamicArray<GC_Vector2u> changes;
	changes.Reserve(map.Count());

	GC_Vector2i dirs[GC_Cardinal8::Count];
	for (GC_Cardinal8 dir : GC_Cardinal8::Range())
		dirs[dir] = GC_CardinalDirection(dir);

	while (1)
	{
		for_range_v(x, w)
		{
			for_range_v(y, h)
			{
				char const c = map(x, y);
				if (c == '.') continue;

				uint occCount = 0;
				for_range ((uint)GC_Cardinal8::Count)
				{
					GC_Vector2i test = dirs[i] + GC_Vector2u{ x, y };
					if (GC_InRangeUnSafe(test, r0, r1))
					{
						if (map(test) == '#')
							++occCount;
					}
				}

				if ((c == '#' && occCount >= 4) ||
					(c == 'L' && occCount == 0))
				{
					changes.Add({ x, y });
				}
			}
		}

		if (changes.Count() == 0)
			break;

		for (GC_Vector2i c : changes)
			map(c) = map(c) == '#' ? 'L' : '#';

		changes.Clear();
	}

	uint occ = 0;
	for (char c : map)
		if (c == '#')
			++occ;

	return occ;
}

DEFINE_TEST_G(Part1, Day11)
{
	TEST_EQ(locDay11Part1("AOC_Day11Test.txt"), 37);
	TEST_EQ(locDay11Part1("AOC_Day11Part1.txt"), 2321);
}

static uint locDay11Part2(char const* aFile)
{
	auto lines = GC_File::ReadAllLines(aFile);
	GC_DynamicArray2D<char> map;

	uint const w = lines[0].Count();
	uint const h = lines.Count();

	map.SetSize({ w, h });
	for_index(auto line : lines)
	{
		GC_Memcpy(map.Row(i).Buffer(), line.Buffer(), w);
	}

	GC_DynamicArray2D<char> map2 = map;

	GC_Vector2i const r0(0);
	GC_Vector2i const r1(w-1, h-1);

	GC_DynamicArray<GC_Vector2u> changes;
	changes.Reserve(map.Count());

	GC_Vector2i dirs[GC_Cardinal8::Count];
	for (GC_Cardinal8 dir : GC_Cardinal8::Range())
		dirs[dir] = GC_CardinalDirection(dir);

	while (1)
	{
		for_range_v(x, w)
		{
			for_range_v(y, h)
			{
				char const c = map(x, y);
				if (c == '.') continue;

				uint occCount = 0;
				for_range ((uint)GC_Cardinal8::Count)
				{
					GC_Vector2i test = dirs[i] + GC_Vector2u{ x, y };
					while (GC_InRangeUnSafe(test, r0, r1))
					{
						char const cc = map(test);
						if (cc == '#')
						{
							++occCount;
							break;
						}
						if (cc == 'L')
							break;
						test += dirs[i];
					}
				}

				if ((c == '#' && occCount >= 5) ||
					(c == 'L' && occCount == 0))
				{
					changes.Add({ x, y });
				}
			}
		}

		if (changes.Count() == 0)
			break;

		for (GC_Vector2i c : changes)
			map(c) = map(c) == '#' ? 'L' : '#';

		changes.Clear();
	}

	uint occ = 0;
	for (char c : map)
		if (c == '#')
			++occ;

	return occ;
}

DEFINE_TEST_G(Part2, Day11)
{
	TEST_EQ(locDay11Part2("AOC_Day11Test.txt"), 26);
	TEST_EQ(locDay11Part2("AOC_Day11Part1.txt"), 2102);
}
