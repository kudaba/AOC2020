#include "AOC_Precompiled.h"

struct Cube : GC_Cube<int64>
{
	GC_DynamicArray<Cube> FlipRegion;

	void Flip(GC_Cube<int64> newCube)
	{
		for (uint ci = 0; ci < FlipRegion.Count(); ++ci)
		{
			Cube& c = FlipRegion[ci];

			GC_Cube overlap;
			if (newCube.Overlaps(c, overlap))
			{
				// remove whole thing
				if (overlap == c)
				{
					FlipRegion.RemoveAtFast(ci--);
				}
				else
				{
					c.Flip(overlap);
				}
			}
		}

		FlipRegion.Add({ newCube });
	}

	uint64 CalcVolume() const
	{
		return VolumeInclusive() - GC_Algorithm::Sum(FlipRegion, [](auto const& c) { return c.CalcVolume(); });
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

		GC_Vector3<int64> newMin(0);
		GC_Vector3<int64> newMax(0);
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
			cubes.Flip({ newMin, newMax });

			if (parts[0][1] == 'f')
				cubes.FlipRegion.PopBack();
		}
	}

	return cubes.VolumeInclusive() - cubes.CalcVolume();
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

static auto locPart1CSG(char const* aFile, int aRange)
{
	GC_DynamicArray<GC_Cube<s64>> cubes;
	cubes.Reserve(1000000);

	GC_DynamicArray<GC_Cube<s64>> newCubes;
	newCubes.Reserve(1000000);

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<2>(line, " ");
		auto rangeStrings = GC_StrSplit<3>(parts[1], ",");

		GC_Cube<s64> newCube = {};
		bool valid = true;

		for_index(auto r : rangeStrings)
		{
			auto mm = GC_StrSplit<2>(r.Range(2), "..");
			newCube.Min[i] = GC_Atoi(mm[0]);
			newCube.Max[i] = GC_Atoi(mm[1]);

			if (newCube.Max[i] < newCube.Min[i])
				GC_Swap(newCube.Min[i], newCube.Max[i]);

			if (newCube.Min[i] < -aRange || newCube.Max[i] > aRange)
			{
				valid = false;
				break;
			}
		}

		if (valid)
		{
			newCubes.Clear();

			for (uint i = 0; i < cubes.Count(); ++i)
				newCubes.AddRange(cubes[i].Split(newCube));

			if (parts[0][1] == 'n')
				newCubes.Add(newCube);

			cubes.Swap(newCubes);
		}
	}

	return GC_Algorithm::Sum(cubes, [](auto const& c) {return c.VolumeInclusive(); });
}

DEFINE_TEST_G(Part1, Day22CSG)
{
	TEST_EQ(locPart1CSG("AOC_Day22Test.txt", 50), 590784);
	TEST_EQ(locPart1CSG("AOC_Day22Part1.txt", 50), 600458);
}

DEFINE_TEST_G(Part2, Day22CSG)
{
	TEST_EQ(locPart1CSG("AOC_Day22Test2.txt", 1000000), 2758514936282235);
	TEST_EQ(locPart1CSG("AOC_Day22Part1.txt", 1000000), 1334275219162622);
}
