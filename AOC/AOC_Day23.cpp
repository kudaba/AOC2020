#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, uint rounds)
{
	auto map = GC_File::Parse2d<bool>(aFile, [](char c) { return c == '#'; });

	struct Elf
	{
		GC_Optional<GC_Cardinal> Proposed;
	};

	GC_HashMap<GC_Vector2i, Elf> elves;
	for_range2d(map.Width(), map.Height())
		if (map(x, y))
			elves.Add(GC_Vector2i(x, y), {});

	GC_Function<bool(GC_Vector2i)> rules[] =
	{
		[&](GC_Vector2i p) { return !(elves.Contains(p + GC_Vector2(-1,1)) || elves.Contains(p + GC_Vector2(0,1)) || elves.Contains(p + GC_Vector2(1,1))); },
		[&](GC_Vector2i p) { return !(elves.Contains(p + GC_Vector2(1,-1)) || elves.Contains(p + GC_Vector2(1,0)) || elves.Contains(p + GC_Vector2(1,1))); },
		[&](GC_Vector2i p) { return !(elves.Contains(p + GC_Vector2(-1,-1)) || elves.Contains(p + GC_Vector2(0,-1)) || elves.Contains(p + GC_Vector2(1,-1))); },
		[&](GC_Vector2i p) { return !(elves.Contains(p + GC_Vector2(-1,-1)) || elves.Contains(p + GC_Vector2(-1,0)) || elves.Contains(p + GC_Vector2(-1,1))); },
	};
	GC_Cardinal Dirs[4] = { GC_Cardinal::South, GC_Cardinal::North, GC_Cardinal::West, GC_Cardinal::East };

	GC_HashMap<GC_Vector2i, uint> proposals;
	GC_HashMap<GC_Vector2i, Elf> elves2;
	elves2.Reserve(elves.Count());
	for_range_v(r, rounds)
	{
		proposals.Clear();

		for (auto& elf : elves)
		{
			bool any = false;
			for_range2d(3, 3)
			{
				GC_Vector2i off(x - 1, y - 1);
				if ((off.x || off.y) && elves.Contains(elf.Key + off))
				{
					any = true;
					break;
				}
			}

			if (any)
			for_range(4)
			{
				GC_Cardinal dir = Dirs[(i + r) % 4];
				if (rules[dir](elf.Key))
				{
					elf.Value.Proposed = dir;
					GC_Vector2u n = elf.Key + GC_CardinalDirection(elf.Value.Proposed.Get());
					if (uint* p = proposals.Find(n))
						(*p)++;
					else
						proposals.Add(n, 0);
					break;
				}
			}
		}

		elves2.Clear();

		for (auto& elf : elves)
		{
			GC_Vector2i np = elf.Key;

			if (elf.Value.Proposed.IsSet())
			{
				GC_Vector2u n = elf.Key + GC_CardinalDirection(elf.Value.Proposed.Get());
				if (proposals[n] == 0)
				{
					np = n;
				}
			}

			elves2.Add(np, Elf());
		}

		elves.Swap(elves2);
	}

	GC_Vector2i min(INT_MAX), max(INT_MIN);
	for (auto& elf : elves)
	{
		min = GC_Min(min, elf.Key);
		max = GC_Max(max, elf.Key);
	}

	return (max - min + GC_Vector2i(1)).Area() - elves.Count();
}

DEFINE_TEST_G(Part1, Day23)
{
	TEST_EQ(locPart1("AOC_Day23Test.txt", 10), 110);
	TEST_EQ(locPart1("AOC_Day23Part1.txt", 10), 3815);
}

static auto locPart2(char const* aFile)
{
	auto map = GC_File::Parse2d<bool>(aFile, [](char c) { return c == '#'; });

	struct Elf
	{
		GC_Optional<GC_Cardinal> Proposed;
	};

	GC_HashMap<GC_Vector2i, Elf> elves;
	for_range2d(map.Width(), map.Height())
		if (map(x, y))
			elves.Add(GC_Vector2i(x, y), {});

	GC_Function<bool(GC_Vector2i)> rules[] =
	{
		[&](GC_Vector2i p) { return !(elves.Contains(p + GC_Vector2(-1,1)) || elves.Contains(p + GC_Vector2(0,1)) || elves.Contains(p + GC_Vector2(1,1))); },
		[&](GC_Vector2i p) { return !(elves.Contains(p + GC_Vector2(1,-1)) || elves.Contains(p + GC_Vector2(1,0)) || elves.Contains(p + GC_Vector2(1,1))); },
		[&](GC_Vector2i p) { return !(elves.Contains(p + GC_Vector2(-1,-1)) || elves.Contains(p + GC_Vector2(0,-1)) || elves.Contains(p + GC_Vector2(1,-1))); },
		[&](GC_Vector2i p) { return !(elves.Contains(p + GC_Vector2(-1,-1)) || elves.Contains(p + GC_Vector2(-1,0)) || elves.Contains(p + GC_Vector2(-1,1))); },
	};
	GC_Cardinal Dirs[4] = { GC_Cardinal::South, GC_Cardinal::North, GC_Cardinal::West, GC_Cardinal::East };

	GC_HashMap<GC_Vector2i, uint> proposals;
	GC_HashMap<GC_Vector2i, Elf> elves2;
	elves2.Reserve(elves.Count());
	uint round = 0;
	while(1)
	{
		proposals.Clear();

		for (auto& elf : elves)
		{
			bool any = false;
			for_range2d(3, 3)
			{
				GC_Vector2i off(x - 1, y - 1);
				if ((off.x || off.y) && elves.Contains(elf.Key + off))
				{
					any = true;
					break;
				}
			}

			if (any)
				for_range(4)
			{
				GC_Cardinal dir = Dirs[(i + round) % 4];
				if (rules[dir](elf.Key))
				{
					elf.Value.Proposed = dir;
					GC_Vector2u n = elf.Key + GC_CardinalDirection(elf.Value.Proposed.Get());
					if (uint* p = proposals.Find(n))
						(*p)++;
					else
						proposals.Add(n, 0);
					break;
				}
			}
		}

		++round;

		if (proposals.Count() == 0)
			return round;

		elves2.Clear();

		for (auto& elf : elves)
		{
			GC_Vector2i np = elf.Key;

			if (elf.Value.Proposed.IsSet())
			{
				GC_Vector2u n = elf.Key + GC_CardinalDirection(elf.Value.Proposed.Get());
				if (proposals[n] == 0)
				{
					np = n;
				}
			}

			elves2.Add(np, Elf());
		}

		elves.Swap(elves2);
	}

	return 0u;
}

DEFINE_TEST_G(Part2, Day23)
{
	TEST_EQ(locPart2("AOC_Day23Test.txt"), 20);
	TEST_EQ(locPart2("AOC_Day23Part1.txt"), 893);
}
