#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, uint rounds)
{
	auto map = GC_File::Parse2d<bool>(aFile, [](char c) { return c == '#'; });

	uint const extra = 200;

	GC_DynamicArray<GC_KeyValuePair<GC_Vector2u, GC_Optional<uint>>> elves;
	for_range2d(map.Width(), map.Height())
		if (map(x, y))
			elves.Add({ GC_Vector2u(x + extra, y + extra), {} });

	map.Initialize(map.Size() + GC_Vector2u(extra) * 2, false);

	for (auto& elf : elves)
		map(elf.Key) = true;

	GC_Vector2i Dirs[8] =
	{
		{-1,-1}, {0, -1},
		{1,1}, {0, 1},
		{-1,1}, {-1, 0},
		{1,-1}, {1, 0},
	};
	uint Bits[4] =
	{
		0b01000011,
		0b00011100,
		0b00110001,
		0b11000100,
	};

	GC_HashMap<GC_Vector2u, uint> proposals;
	proposals.Reserve(elves.Count());

	for_range_v(r, rounds)
	{
		proposals.Clear();

		for (auto& elf : elves)
		{
			uint8 any = 0;
			for_range(8)
			{
				any |= map(elf.Key + Dirs[i]) << i;
			}

			if (any)
			for_range(4)
			{
				uint rot = (i + r) % 4;
				if (!(any & Bits[rot]))
				{
					GC_Vector2i next = elf.Key + Dirs[rot * 2 + 1];

					auto item = proposals.Add(next);
					if (item.First)
						*item.Second = 0;
					else
						(*item.Second)++;

					elf.Value = (uint)(GC_AddressDiff(&proposals.First().Value, item.Second) / sizeof(proposals.First()));

					break;
				}
			}
		}

		if (proposals.Count() == 0)
			return r + 1;

		for (auto& elf : elves)
		{
			if (uint const* n = elf.Value.TryGet())
			{
				auto const& p = proposals.GetByIndex(*n);
				if (p.Value == 0)
				{
					map(elf.Key) = false;
					map(p.Key) = true;
					elf.Key = p.Key;
				}
				elf.Value.Reset();
			}
		}
	}

	GC_Vector2u min(UINT_MAX), max(0);
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

DEFINE_TEST_G(Part2, Day23)
{
	TEST_EQ(locPart1("AOC_Day23Test.txt", UINT_MAX), 20);
	TEST_EQ(locPart1("AOC_Day23Part1.txt", UINT_MAX), 893);
}
