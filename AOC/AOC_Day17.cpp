#include "AOC_Precompiled.h"

static uint64 locPart1(char const* aFile, uint64 RocksFalling)
{
	GC_DynamicArray<byte> data;
	GC_File::ReadAllData(aFile, data);
	while (GC_IsSpace(data.Last()))
		data.Resize(data.Count() - 1);

	struct Rock
	{
		GC_Vector2u Size;
		GC_Vector2u Heights[4];
	};
	Rock Rocks[5] =
	{
		{ {4, 1}, {{0, 1},{0, 1},{0, 1},{0, 1}}},
		{ {3, 3}, {{1, 2},{0, 3},{1, 2}}},
		{ {3, 3}, {{0, 1},{0, 1},{0, 3}}},
		{ {1, 4}, {{0, 4}}},
		{ {2, 2}, {{0, 2},{0, 2}}},
	};

	uint heights[7] = {};
	uint nextRock = 0;
	uint nextJet = 0;
	uint64 lastRoundHeight = 0;
	uint64 lastRoundRock = 0;

	GC_DynamicArray2D<bool> heightMap;
	heightMap.Initialize(7, 100000, false);

	GC_DynamicArray<GC_Vector3<uint64>> pattern;
	pattern.Reserve(1000);
	pattern.Add({ 0,0,0 });

	uint64 patternHeight = 0;

	for (int i = 0; i < RocksFalling; ++i)
	{
		auto const& rock = Rocks[nextRock++ % 5];

		uint height = GC_Algorithm::Max(heights);

		GC_Vector2u pos(2, height + 3 );

		while (1)
		{
			if (data[nextJet] == '<')
			{
				if (pos.x > 0)
				{
					bool canMove = true;
					for_range_v(r, rock.Size.x)
					{
						for(uint min = rock.Heights[r].x; min < rock.Heights[r].y; min++)
							if (heightMap(pos.x + r - 1, pos.y + min))
							{
								canMove = false;
								break;
							}

						if (!canMove) break;
					}

					if (canMove)
						pos.x--;
				}
			}
			else
			{
				if (pos.x + rock.Size.x < 7)
				{
					bool canMove = true;
					for_range_v(r, rock.Size.x)
					{
						for (uint min = rock.Heights[r].x; min < rock.Heights[r].y; min++)
							if (heightMap(pos.x + r + 1, pos.y + min))
							{
								canMove = false;
								break;
							}

						if (!canMove) break;
					}

					if (canMove)
						pos.x++;
				}
			}

			++nextJet;
			if (nextJet == data.Count())
			{
				nextJet = 0;

				if (patternHeight == 0)
				{
					uint64 curHeight = GC_Algorithm::Max(heights);
					pattern.Add({ curHeight - lastRoundHeight, nextRock % 5, nextRock - lastRoundRock });
					lastRoundHeight = curHeight;
					lastRoundRock = nextRock;

					for_range_v(p, GC_Min<uint>(100, pattern.Count() / 2))
					{
						if (p)
						{
							bool match = true;
							{
								for_range_v(m, p)
									if ((GC_Vector2<uint64>&)pattern[pattern.Count() - m - 1] != (GC_Vector2<uint64>&)pattern[pattern.Count() - p - m - 1])
									{
										match = false;
										break;
									}
							}
							if (match)
							{
								uint64 remainingRocks = RocksFalling - i;
								uint64 rocksPerRound = GC_Algorithm::Sum(pattern.Right(p)).z;
								uint64 totalRounds = remainingRocks / rocksPerRound;
								uint64 extra = remainingRocks % rocksPerRound;

								for_range_v(a, p)
								{
									uint64 pc = pattern[pattern.Count() - p + a].x;
									patternHeight += pc * totalRounds;
								}

								RocksFalling = nextRock + extra - 1;
							}
						}
					}
				}
			}

			bool canDrop = pos.y > 0;
			if (canDrop)
			for_range_v(r, rock.Size.x)
			{
				for (uint min = rock.Heights[r].x; min < rock.Heights[r].y; min++)
					if (heightMap(pos.x + r, pos.y + min - 1))
					{
						canDrop = false;
						break;
					}

				if (!canDrop) break;
			}

			if (!canDrop)
			{
				for_range_v(r, rock.Size.x)
				{
					heights[pos.x + r] = GC_Max(heights[pos.x + r], pos.y + rock.Heights[r].y);

					for (uint min = rock.Heights[r].x; min < rock.Heights[r].y; min++)
					{
						GC_ASSERT(!heightMap(pos.x + r, pos.y + min));
						heightMap(pos.x + r, pos.y + min) = true;
					}
				}
				break;
			}

			pos.y--;
		}
	}

	uint max = GC_Algorithm::Max(heights);
	return max + patternHeight;
}

DEFINE_TEST_G(Part1, Day17)
{
	TEST_EQ(locPart1("AOC_Day17Test.txt", 2022), 3068);
	TEST_EQ(locPart1("AOC_Day17Part1.txt", 2022), 3191);
}

DEFINE_TEST_G(Part2, Day17)
{
	TEST_EQ(locPart1("AOC_Day17Test.txt", 1000000000000), 1514285714288);
	TEST_EQ(locPart1("AOC_Day17Part1.txt", 1000000000000), 1572093023267);
}
