#include "AOC_Precompiled.h"

static uint64 locVolume(GC_Vector3i const& min, GC_Vector3i const& max)
{
	auto volume = max - min + GC_Vector3i(1); // inclusive of whole range
	return (uint64)volume.x * volume.y * volume.z;
}

struct Cube
{
	GC_Vector3i min;
	GC_Vector3i max;

	GC_DynamicArray<Cube> FlipRegion;

	void Flip(GC_Vector3i const& newMin, GC_Vector3i const& newMax)
	{
		for (uint ci = 0; ci < FlipRegion.Count(); ++ci)
		{
			Cube& c = FlipRegion[ci];

			bool overlap = true;
			for_range(3)
				if (newMax[i] < c.min[i] || newMin[i] > c.max[i])
					overlap = false;

			if (!overlap)
				continue;

			// remove whole thing
			if (newMax >= c.max && newMin <= c.min)
			{
				FlipRegion.RemoveAtFast(ci--);
			}
			else
			{
				c.Flip(GC_Max(c.min, newMin), GC_Min(c.max, newMax));
			}
		}

		FlipRegion.Add({newMin, newMax});
	}

	uint64 Volume() const
	{
		return locVolume(min, max) - GC_Algorithm::Sum(FlipRegion, [](auto const& c) { return c.Volume(); });
	}
};

static auto locPart1(char const* aFile, int aRange)
{
	Cube cubes{GC_Vector3i(-aRange), GC_Vector3i(aRange)};
	cubes.FlipRegion.Reserve(500);

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<2>(line, " ");
		auto rangeStrings = GC_StrSplit<3>(parts[1], ",");

		GC_Vector3i newMin;
		GC_Vector3i newMax;
		bool valid = true;

		for_index(auto r : rangeStrings)
		{
			auto mm = GC_StrSplit<2>(r.Range(2), "..");
			newMin[i] = GC_Atoi(mm[0]);
			newMax[i] = GC_Atoi(mm[1]);

			if (newMax[i] < newMin[i])
				GC_Swap(newMin[i], newMax[i]);

			if (newMin[i] < -aRange || newMax[i] > aRange)
			{
				valid = false;
				break;
			}
		}

		if (valid)
		{
			cubes.Flip(newMin, newMax);

			if (parts[0][1] == 'f')
				cubes.FlipRegion.PopBack();
		}
	}

	return locVolume(cubes.min, cubes.max) - cubes.Volume();
}

DEFINE_TEST_G(Part1, Day22)
{
	TEST_EQ(locPart1("AOC_Day22Test.txt", 50), 590784);
	TEST_EQ(locPart1("AOC_Day22Part1.txt", 50), 600458);
}

DEFINE_TEST_G(Part2, Day22)
{
	TEST_EQ(locPart1("AOC_Day22Test2.txt", 1000000), 2758514936282235);
	TEST_EQ(locPart1("AOC_Day22Part1.txt", 1000000), 1334275219162622);
}
