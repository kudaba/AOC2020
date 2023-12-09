#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	auto map = GC_File::Parse2d<char>(aFile, [](char c)
		{
			return c;
		});

	const uint width = map.Width();
	const uint height = map.Height();

	uint result = 0;

	for_range2d(width, height)
	{
		char& c = map(x, y);
		if (GC_IsDigit(c))
		{
			uint ex = x;
			while (ex < width && GC_IsDigit(map(ex, y)))
			{
				++ex;
			}

			uint nx = x ? x - 1 : x;
			uint ny = y ? y - 1 : y;
			uint ey = y < height ? y + 1 : y;

			ex = ex < width ? ex + 1 : ex;
			ey = ey < height ? ey + 1 : ey;

			auto area = map.Area({ nx, ny }, { ex, ey });
			for_range2d_v(ix, iy, area.Width(), area.Height())
			{
				const char nc = area(ix, iy);
				if (!(nc == '.' || GC_IsDigit(nc)))
				{
					result += GC_Atoi(&c);
					x = ex - 1;
					break;
				}
			}
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day3)
{
	TEST_EQ(locPart1("AOC_Day3Test.txt"), 4361);
	TEST_EQ(locPart1("AOC_Day3Part1.txt"), 521601);
}

static auto locPart2(char const* aFile)
{
	auto map = GC_File::Parse2d<char>(aFile, [](char c)
		{
			return c;
		});

	const uint width = map.Width();
	const uint height = map.Height();

	uint64 result = 0;

	for_range2d(width, height)
	{
		char& c = map(x, y);
		if (c == '*')
		{
			GC_Vector2u n1(width);
			GC_Vector2u n2(width);
			GC_Vector2u n3(width);
			for_range2d_v(dx, dy, 3, 3)
			{
				GC_Vector2i p = GC_Clamp<GC_Vector2i>(GC_Vector2i(x + dx - 1, y + dy - 1), { 0, 0 }, map.Size());
				if (GC_IsDigit(map(p)))
				{
					GC_Vector2u np = p;
					while (np.x && GC_IsDigit(map(np.x - 1, p.y)))
						--np.x;

					if (n1.x == width)
					{
						n1 = np;
					}
					else if (np != n1 && n2.x == width)
					{
						n2 = np;
					}
					else if (np != n1 && np != n2)
					{
						n3 = np;
						break;
					}
				}
			}

			if (n1.x != width && n2.x != width && n3.x == width)
			{
				result += GC_Atoi(&map(n1)) * GC_Atoi(&map(n2));
			}
		}
	}

	return result;
}

DEFINE_TEST_G(Part2, Day3)
{
	TEST_EQ(locPart2("AOC_Day3Test.txt"), 467835);
	TEST_EQ(locPart2("AOC_Day3Part1.txt"), 80694070);
}
