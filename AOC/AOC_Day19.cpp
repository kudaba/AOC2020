#include "AOC_Precompiled.h"

struct Scanner
{
	GC_DynamicArray<GC_Vector3i> Beacons;
	GC_Vector3i Position = GC_Vector3i(0);
	bool Fixed = false;
};

static bool locCompareScanners(GC_HashSet<GC_Vector3i>& finalPositions, Scanner& targetScanner, GC_HashSet<GC_Vector3i>& adjustedPositions)
{
	GC_Vector3i orders[] =
	{
		GC_Vector3i(0, 1, 2),
		GC_Vector3i(1, 2, 0),
		GC_Vector3i(2, 0, 1),
		GC_Vector3i(2, 1, 0),
		GC_Vector3i(1, 0, 2),
		GC_Vector3i(0, 2, 1),
	};

	for (auto o : orders)
		for (int x = -1; x <= 1; x += 2)
			for (int y = -1; y <= 1; y += 2)
				for (int z = -1; z <= 1; z += 2)
				{
					adjustedPositions.Clear();
					for (auto const& p : targetScanner.Beacons)
						adjustedPositions.Add(
							GC_Vector3i(
								p[o[0]] * x,
								p[o[1]] * y,
								p[o[2]] * z
							));

					for (auto const& anchor : adjustedPositions)
					{
						for (auto const& soff : finalPositions)
						{
							auto const offset = soff - anchor;

							uint overlap = 0;
							for (auto const& p : adjustedPositions)
								if (finalPositions.Contains(p + offset))
									if (++overlap == 12)
									{
										targetScanner.Position = offset;
										for (auto np : adjustedPositions)
											finalPositions.Add(np + offset);
										return true;
									}
						}
					}
				}
	return false;
}

static auto locPart1(char const* aFile, uint64& part2)
{
	GC_DynamicArray<Scanner> scanners;

	GC_String text = GC_File::ReadAllText(aFile);
	for (GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{
		auto& scanner = scanners.Add().Beacons;

		GC_StrSlice line;
		GC_StrLine(chunk, line);
		while (GC_StrLine(chunk, line))
		{
			auto& beacon = scanner.Add();
			for_index(GC_StrSlice axis; GC_Strtok(line, ",", axis);)
			{
				beacon[i] = GC_Atoi(axis);
			}
		}
	}

	scanners[0].Fixed = true;

	GC_HashSet<GC_Vector3i> finalPositions(scanners[0].Beacons);
	GC_HashSet<GC_Vector3i> adjustedPositions;

	while (1)
	{
		bool changed = false;

		for(auto& s : scanners)
		{
			if (!s.Fixed)
			{
				if (locCompareScanners(finalPositions, s, adjustedPositions))
				{
					s.Fixed = changed = true;
				}
			}
		}

		if (!changed)
			break;
	}

	part2 = 0;
	for_range(scanners.Count())
		for_range_v(j, scanners.Count())
	{
		GC_Vector3i diff = scanners[i].Position - scanners[j].Position;
		uint64 sum = GC_Abs(diff.x) + GC_Abs(diff.y) + GC_Abs(diff.z);
		part2 = GC_Max(sum, part2);
	}

	return finalPositions.Count();
}

DEFINE_TEST_G(Part1And2, Day19)
{
	uint64 part2;
	TEST_EQ(locPart1("AOC_Day19Test.txt", part2), 79);
	TEST_EQ(part2, 3621);
	TEST_EQ(locPart1("AOC_Day19Part1.txt", part2), 318);
	TEST_EQ(part2, 12166);
}
