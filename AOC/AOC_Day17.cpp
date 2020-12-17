#include "AOC_Precompiled.h"
#include "AOC_Day17.h"

#define DEBUG_DAY17_PART1 0
#define DEBUG_DAY17_PART2 0

static uint locDay17Part1(char const* aFile)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_StrSlice chunk; GC_Strtok(text, "\n", chunk);
	uint width = chunk.Count();

	GC_HashSet<GC_Vector3i> active;
	for_range_v(x, width)
	{
		for_range_v(y, width)
		{
			if (text[y*(width+1) + x] == '#')
				active.Add(GC_Vector3i(x, y, 0));
		}
	}

	GC_HashMap<GC_Vector3i, uint> counts;
	for_range(6)
	{
		counts.Clear();

		for (GC_Vector3i cube : active)
		{
			int x = cube.x;
			int y = cube.y;
			int z = cube.z;
			for (int xx = x-1; xx <= x+1; ++xx)
				for (int yy = y-1; yy <= y+1; ++yy)
					for (int zz = z-1; zz <= z+1; ++zz)
						if (xx != x || yy != y || zz != z)
						{
							auto elem = counts.Add(GC_Vector3i(xx, yy, zz));
							if (elem.myFirst) *elem.mySecond = 1;
							else ++(*elem.mySecond);
						}
						else
						{
							auto elem = counts.Add(GC_Vector3i(xx, yy, zz));
							if (elem.myFirst) *elem.mySecond = 0;
						}
		}

		GC_Vector3i min(INT_MAX), max(INT_MIN);
		for (auto itr : counts)
		{
			if (itr.myValue == 3)
			{
				active.Add(itr.myKey);
				min = GC_Min(min, itr.myKey);
				max = GC_Max(max, itr.myKey);
			}
			else if (itr.myValue != 2)
				active.Remove(itr.myKey);
			else if (active.Contains(itr.myKey))
			{
				min = GC_Min(min, itr.myKey);
				max = GC_Max(max, itr.myKey);
			}
		}

#if DEBUG_DAY17
		GC_TestFixture::GetCurrentTest()->Printf("%d X: %d-%d Y:%d-%d\n", i+1, min.x, max.x, min.y, max.y);
		for (int z = min.z; z <= max.z; ++z)
		{
			GC_TestFixture::GetCurrentTest()->Printf("Z: %d\n", z);
			for (int y = min.y; y <= max.y; ++y)
			{
				for (int x = min.x; x <= max.x; ++x)
				{
					GC_TestFixture::GetCurrentTest()->Printf(active.Contains({ x,y,z }) ? "#" : ".");
				}
				GC_TestFixture::GetCurrentTest()->Printf("\n");
			}
		}
#endif
	}

	return active.Count();
}

DEFINE_TEST_G(Part1, Day17)
{
	TEST_EQ(locDay17Part1("AOC_Day17Test.txt"), 112);
	TEST_EQ(locDay17Part1("AOC_Day17Part1.txt"), 426);
}

static uint locDay17Part2(char const* aFile)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_StrSlice chunk; GC_Strtok(text, "\n", chunk);
	uint width = chunk.Count();

	GC_HashSet<GC_Vector4i> active;
	for_range_v(x, width)
	{
		for_range_v(y, width)
		{
			if (text[y*(width+1) + x] == '#')
				active.Add({ (int)x, (int)y, 0, 0 });
		}
	}

	GC_HashMap<GC_Vector4i, uint> counts;
	for_range(6)
	{
		counts.Clear();

		for (GC_Vector4i cube : active)
		{
			int x = cube.x;
			int y = cube.y;
			int z = cube.z;
			int w = cube.w;
			for (int xx = x-1; xx <= x+1; ++xx)
				for (int yy = y-1; yy <= y+1; ++yy)
					for (int zz = z-1; zz <= z+1; ++zz)
						for (int ww = w-1; ww <= w+1; ++ww)
						if (xx != x || yy != y || zz != z || ww != w)
						{
							auto elem = counts.Add({ xx, yy, zz, ww });
							if (elem.myFirst) *elem.mySecond = 1;
							else ++(*elem.mySecond);
						}
						else
						{
							auto elem = counts.Add({ xx, yy, zz, ww });
							if (elem.myFirst) *elem.mySecond = 0;
						}
		}

		GC_Vector4i min(INT_MAX), max(INT_MIN);
		for (auto itr : counts)
		{
			if (itr.myValue == 3)
			{
				active.Add(itr.myKey);
				min = GC_Min(min, itr.myKey);
				max = GC_Max(max, itr.myKey);
			}
			else if (itr.myValue != 2)
				active.Remove(itr.myKey);
			else if (active.Contains(itr.myKey))
			{
				min = GC_Min(min, itr.myKey);
				max = GC_Max(max, itr.myKey);
			}
		}

#if DEBUG_DAY17_PART2
		GC_TestFixture::GetCurrentTest()->Printf("%d X: %d-%d Y:%d-%d\n", i+1, min.x, max.x, min.y, max.y);
		for (int w = min.w; w <= max.w; ++w)
			for (int z = min.z; z <= max.z; ++z)
		{
			GC_TestFixture::GetCurrentTest()->Printf("Z: %d W: %d\n", z, w);
			for (int y = min.y; y <= max.y; ++y)
			{
				for (int x = min.x; x <= max.x; ++x)
				{
					GC_TestFixture::GetCurrentTest()->Printf(active.Contains({ x,y,z }) ? "#" : ".");
				}
				GC_TestFixture::GetCurrentTest()->Printf("\n");
			}
		}
#endif
	}

	return active.Count();
}

DEFINE_TEST_G(Part2, Day17)
{
	TEST_EQ(locDay17Part2("AOC_Day17Test.txt"), 848);
	TEST_EQ(locDay17Part2("AOC_Day17Part1.txt"), 1892);
}
