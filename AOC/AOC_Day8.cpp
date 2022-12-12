#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	auto data = GC_File::Parse2d<char>(aFile, [](char c) -> char { return c - '0'; });
	GC_DynamicArray2D<bool> visited(data.Size(), 0);

	uint64 result = (data.Width() + data.Height() - 2) * 2;

	for (uint y = 1, e = data.Height() - 1; y < e; ++y)
	{
		auto row = data.Row(y);
		char min = row.First();
		char max = row.Last();
		uint index = 1;
		for (; index < row.Count() - 1 && min < 9; ++index)
		{
			if (char c = row[index]; c > min)
			{
				if (!visited(index, y))
				{
					visited(index, y) = true;
					++result;
				}
				min = c;
			}
		}
		uint end = row.Count() - 2;
		for (; end > 0 && max < 9; --end)
		{
			if (char c = row[end]; c > max)
			{
				if (!visited(end, y))
				{
					visited(end, y) = true;
					++result;
				}
				max = c;
			}
		}
	}

	for (uint y = 1, e = data.Width() - 1; y < e; ++y)
	{
		auto row = data.Column(y);
		char min = *row.begin();
		char max = *(row.end()-1);
		uint index = 1;
		for (; index < row.Count() - 1 && min < 9; ++index)
		{
			if (char c = row[index]; c > min)
			{
				if (!visited(y, index))
				{
					visited(y, index) = true;
					++result;
				}
				min = c;
			}
		}
		uint end = row.Count() - 2;
		for (; end > 0 && max < 9; --end)
		{
			if (char c = row[end]; c > max)
			{
				if (!visited(y, end))
				{
					visited(y, end) = true;
					++result;
				}
				max = c;
			}
		}
	}

	//for_range_v(y, data.Height())
	//{
	//	for_range_v(x, data.Width())
	//	{
	//		if (visited(x, y))
	//		{
	//			printf(ANSI_COLOR_YELLOW "%d", data(x, y));
	//		}
	//		else
	//		{
	//			printf(ANSI_COLOR_RESET "%d", data(x, y));
	//		}
	//	}

	//	printf("\n");
	//}

	return result;
}

DEFINE_TEST_G(Part1, Day8)
{
	TEST_EQ(locPart1("AOC_Day8Test.txt"), 21);
	TEST_EQ(locPart1("AOC_Day8Part1.txt"), 1779);
}

static auto locPart2(char const* aFile)
{
	auto data = GC_File::Parse2d<char>(aFile, [](char c) -> char { return c - '0'; });

	uint64 result = 0;

	int const w = data.Width(), h = data.Height();

	for_range2d(data.Width(), data.Height())
	{
		char c = data(x, y);

		uint64 dist[GC_Cardinal::Count] = { 0 };

		for (GC_Cardinal dire : GC_Cardinal::Range())
		{
			GC_Vector2i dir = GC_CardinalDirection(dire);
			GC_Vector2i p = GC_Vector2i(x, y) + dir;
			while (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h)
			{
				dist[dire]++;

				if (data(p) >= c)
					break;

				p += dir;
			}
		}

		result = GC_Max(result, GC_Algorithm::Product(dist));
	}

	return result;
}

DEFINE_TEST_G(Part2, Day8)
{
	TEST_EQ(locPart2("AOC_Day8Test.txt"), 8);
	TEST_EQ(locPart2("AOC_Day8Part1.txt"), 172224);
}
