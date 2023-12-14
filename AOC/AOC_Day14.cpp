#include "AOC_Precompiled.h"

static void locTilt(GC_DynamicArray2D<char>& platform, GC_Cardinal dir, bool print = false)
{
	uint firstAxis = 0;
	int s = 0, e = 0, d = 0;

	switch (dir)
	{
		GC_ENUM_UNREACHABLE(dir);
	case GC_Cardinal::North:
		firstAxis = 0;
		s = 0; e = platform.Height();
		d = -1;
		break;
	case GC_Cardinal::South:
		firstAxis = 0;
		s = platform.Height() - 1; e = -1;
		d = 1;
		break;
	case GC_Cardinal::West:
		firstAxis = 1;
		s = 0; e = platform.Width();
		d = -1;
		break;
	case GC_Cardinal::East:
		firstAxis = 1;
		s = platform.Width() - 1; e = -1;
		d = 1;
		break;
	}

	GC_Vector2i pos(0);
	GC_Vector2i npos(0);

	int& p = pos[!firstAxis];
	int& np = npos[!firstAxis];

	for_range_v(x, platform.Size()[firstAxis])
	{
		pos[firstAxis] = x;
		npos[firstAxis] = x;

		for (int y = s - d; y != e; y -= d)
		{
			p = y;
			if (platform(pos) == 'O')
			{
				np = y + d;
				while (np - d != s && platform(npos) == '.')
				{
					np += d;
				}
				np -= d;

				GC_Swap(platform(pos), platform(npos));
			}
		}
	}

	if (print)
	{
		for_range_v(y, platform.Height())
		{
			for (char c : platform.Row(y))
			{
				TestFixture::GetCurrentTest()->Printf("%c", c);

			}
			TestFixture::GetCurrentTest()->Printf("\n");
		}
		TestFixture::GetCurrentTest()->Printf("\n");
	}
}

static uint locLoad(GC_DynamicArray2D<char>& platform)
{
	uint result = 0;

	for_range2d(platform.Width(), platform.Height())
	{
		if (platform(x, y) == 'O')
			result += platform.Height() - y;
	}

	return result;
}

static auto locPart1(char const* aFile)
{
	GC_DynamicArray2D<char> platform = GC_File::Parse2d<char>(aFile, [](char c) { return c; });

	locTilt(platform, GC_Cardinal::North);
	return locLoad(platform);
}

DEFINE_TEST_G(Part1, Day14)
{
	TEST_EQ(locPart1("AOC_Day14Test.txt"), 136);
	TEST_EQ(locPart1("AOC_Day14Part1.txt"), 103614);
}

static auto locPart2(char const* aFile, bool print = false)
{
	GC_DynamicArray2D<char> platform = GC_File::Parse2d<char>(aFile, [](char c) { return c; });

	GC_DynamicArray<uint> loads;

	uint cycle = 0;

	while (cycle == 0)
	{
		locTilt(platform, GC_Cardinal::North, print);
		locTilt(platform, GC_Cardinal::West, print);
		locTilt(platform, GC_Cardinal::South, print);
		locTilt(platform, GC_Cardinal::East, print);

		loads.Add(locLoad(platform));
		if (loads.Count() < 2)
			continue;

		uint end = loads.Count();

		for_range(end / 2 - 1)
		{
			uint n = i + 1;
			if (GC_Memeq(&loads[end - n], &loads[end - n * 2], n))
			{
				uint expected = loads[end - n];

				locTilt(platform, GC_Cardinal::North, print);
				locTilt(platform, GC_Cardinal::West, print);
				locTilt(platform, GC_Cardinal::South, print);
				locTilt(platform, GC_Cardinal::East, print);

				uint next = locLoad(platform);
				if (next == expected)
				{
					cycle = end - n;
					break;
				}
				loads.Add(next);
			}
		}
	}

	uint cycleLength = loads.Count() - cycle;
	return loads[cycle + ((1000000000-cycle - 1) % (cycleLength))];
}

DEFINE_TEST_G(Part2, Day14)
{
	TEST_EQ(locPart2("AOC_Day14Test.txt"), 64);
	TEST_EQ(locPart2("AOC_Day14Part1.txt"), 83790);
}
