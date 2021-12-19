#include "AOC_Precompiled.h"

struct Beacon
{
	GC_Vector3i Pos;
	GC_Vector2i ClosestDist = GC_Vector2i(INT32_MAX);
	GC_Vector2i ClosestIndex;
};

struct Scanner
{
	GC_DynamicArray<Beacon> Beacons;
	GC_Vector3i Position = GC_Vector3i(0);
	bool Fixed = false;
};

static bool locCompareScanners(GC_HashSet<GC_Vector3i>& finalPositions, GC_HashSet<GC_Vector2i>& finalDistances, Scanner& targetScanner, GC_HashSet<GC_Vector3i>& adjustedPositions)
{
	GC_HybridArray<int, 12> beacons;
	uint overlapD = 0;
	for_index(auto const& b : targetScanner.Beacons)
		if (finalDistances.Contains(b.ClosestDist))
		{
			beacons.AddUnique(i);
			beacons.AddUnique(b.ClosestIndex.x);
			beacons.AddUnique(b.ClosestIndex.y);
			if (++overlapD == 4)
				break;
		}

	if (overlapD < 4)
		return false;

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
					for (auto const& p : beacons)
						adjustedPositions.Add(
							GC_Vector3i(
								targetScanner.Beacons[p].Pos[o[0]] * x,
								targetScanner.Beacons[p].Pos[o[1]] * y,
								targetScanner.Beacons[p].Pos[o[2]] * z
							));

					for (auto const& anchor : adjustedPositions)
					{
						for (auto const& soff : finalPositions)
						{
							auto const offset = soff - anchor;

							bool allContained = true;
							for (auto const& p : adjustedPositions)
								if (!finalPositions.Contains(p + offset))
								{
									allContained = false;
									break;
								}

							if (allContained)
							{
								targetScanner.Position = offset;

								for_index(auto const& b : targetScanner.Beacons)
								{
									finalDistances.Add(b.ClosestDist);
									finalPositions.Add(offset +
										GC_Vector3i(
											b.Pos[o[0]] * x,
											b.Pos[o[1]] * y,
											b.Pos[o[2]] * z
										));
								}

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
		auto& beacons = scanners.Add().Beacons;

		GC_StrSlice line;
		GC_StrLine(chunk, line);
		while (GC_StrLine(chunk, line))
		{
			auto& beacon = beacons.Add();
			for_index(GC_StrSlice axis; GC_Strtok(line, ",", axis);)
			{
				beacon.Pos[i] = GC_Atoi(axis);
			}
		}
	}

	for (auto& s : scanners)
	{
		for_range2d(s.Beacons.Count(), s.Beacons.Count())
			if (y > x)
			{
				auto& b1 = s.Beacons[x], &b2 = s.Beacons[y];
				int64 const dist = b1.Pos.DistanceSqr(b2.Pos);
				if (dist < b1.ClosestDist[0])
				{
					b1.ClosestDist[1] = b1.ClosestDist[0];
					b1.ClosestIndex[1] = b1.ClosestIndex[0];
					b1.ClosestDist[0] = (int)dist;
					b1.ClosestIndex[0] = y;
				}
				else if (dist < b1.ClosestDist[1])
				{
					b1.ClosestDist[1] = (int)dist;
					b1.ClosestIndex[1] = y;
				}
				else
					GC_ASSERT(b1.ClosestDist != GC_Vector2i((int)dist));

				if (dist < b2.ClosestDist[0])
				{
					b2.ClosestDist[1] = b2.ClosestDist[0];
					b2.ClosestIndex[1] = b2.ClosestIndex[0];
					b2.ClosestDist[0] = (int)dist;
					b2.ClosestIndex[0] = x;
				}
				else if (dist < b2.ClosestDist[1])
				{
					b2.ClosestDist[1] = (int)dist;
					b2.ClosestIndex[1] = x;
				}
				else
					GC_ASSERT(b1.ClosestDist != GC_Vector2i((int)dist));
			}
	}

	scanners[0].Fixed = true;

	GC_HashSet<GC_Vector3i> finalPositions;
	GC_HashSet<GC_Vector2i> finalDistances;
	for (auto const& b : scanners[0].Beacons)
	{
		finalPositions.Add(b.Pos);
		GC_ASSERT(!finalDistances.Contains(b.ClosestDist));
		finalDistances.Add(b.ClosestDist);
	}

	GC_HashSet<GC_Vector3i> adjustedPositions;

	while (1)
	{
		bool changed = false;

		for(auto& s : scanners)
		{
			if (!s.Fixed)
			{
				if (locCompareScanners(finalPositions, finalDistances, s, adjustedPositions))
				{
					s.Fixed = changed = true;
				}
			}
		}

		if (!changed)
			break;
	}

	part2 = 0;
	for_range2d(scanners.Count(), scanners.Count())
		if (y > x)
		{
			GC_Vector3i diff = scanners[x].Position - scanners[y].Position;
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
