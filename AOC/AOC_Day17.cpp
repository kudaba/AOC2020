#include "AOC_Precompiled.h"
#include "AOC_Day17.h"

#define DEBUG_DAY17 0

template <typename T>
static uint locDay17(char const* aFile)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_StrSlice chunk; GC_Strtok(text, "\n", chunk);
	uint width = chunk.Count();

	typename GC_VectorDirections<T>::All directions;

	GC_HashSet<T> active;
	active.Reserve(10000);
	for_range_v(x, width)
	{
		for_range_v(y, width)
		{
			if (text[y*(width+1) + x] == '#')
				active.Add(T(x, y, 0));
		}
	}

	GC_HashMap<T, uint> counts;
	counts.Reserve(10000);
	for_range(6)
	{
		counts.Clear();

		for (T cube : active)
		{
			{
				auto elem = counts.Add(cube);
				if (elem.myFirst) *elem.mySecond = 0;
			}

			for (T dir : directions)
				if (dir != T(0))
				{
					auto elem = counts.Add(cube + dir);
					if (elem.myFirst) *elem.mySecond = 1;
					else ++(*elem.mySecond);
				}
		}

		T min(INT_MAX), max(INT_MIN);
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
	TEST_EQ(locDay17<GC_Vector3i>("AOC_Day17Test.txt"), 112);
	TEST_EQ(locDay17<GC_Vector3i>("AOC_Day17Part1.txt"), 426);
}

DEFINE_TEST_G(Part2, Day17)
{
#if RUN_TESTS
	TEST_EQ(locDay17<GC_Vector4i>("AOC_Day17Test.txt"), 848);
#endif
	TEST_EQ(locDay17<GC_Vector4i>("AOC_Day17Part1.txt"), 1892);
}
