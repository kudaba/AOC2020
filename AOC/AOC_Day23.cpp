#include "AOC_Precompiled.h"

const uint costs[] = { 1,10,100,1000 };

template <uint D = 2>
struct State
{
	bool operator==(State const& other) const { return GC_Memeq(*this, other); }

	char hall[11];
	char rooms[4][D];
};

template <uint D = 2>
static uint locPart1(char const* aFile)
{
	using state = State<D>;

	auto lines = GC_File::ReadAllLines(aFile);

	state initState;
	state endState;
	GC_MemSet(&initState, 5, sizeof(initState));
	GC_MemSet(&endState, 5, sizeof(initState));
	for_range2d(4, D)
	{
		initState.rooms[x][y] = lines[2 + y][3 + x * 2] - 'A';
		endState.rooms[x][y] = (char)x;
	}

	return RunDijsktraShortStep(initState, endState,
		[](state& candidate, auto const& anAddToQueue)
		{
			// Add a new entry into the queue
			auto addTarget = [&](char& unit, char& tile, uint moves)
			{
				GC_ASSERT(unit != 5 && tile == 5 && moves < 20);
				uint const moveCost = costs[unit] * moves;
				GC_Swap(unit, tile);
				anAddToQueue(candidate, moveCost);
				GC_Swap(unit, tile);
			};

			auto moveToTarget = [&](char& unit, uint hallIndex, uint moves)
			{
				uint const target = 2 + unit * 2;
				uint dist = 0;
				bool blocked = false;

				// See if path is clear to my room
				if (target < hallIndex)
				{
					dist = hallIndex - target;
					for (uint j = target; j < hallIndex; ++j)
						if (candidate.hall[j] != 5)
						{
							blocked = true;
							break;
						}
				}
				else
				{
					dist = target - hallIndex;
					for (uint j = hallIndex + 1; j <= target; ++j)
						if (candidate.hall[j] != 5)
						{
							blocked = true;
							break;
						}
				}

				if (!blocked)
				{
					// Find first free tile, but it's only valid if it's the last
					// tile or the remaining tiles are the correct unit
					for_index_v(y, char& tile : candidate.rooms[unit])
					{
						if (tile != 5)
						{
							if (y)
							{
								bool okToMove = true;
								for (uint yy = y; yy < D; ++yy)
								{
									GC_ASSERT(candidate.rooms[unit][yy] != 5);
									if (candidate.rooms[unit][yy] != unit)
									{
										okToMove = false;
										break;
									}
								}

								if (okToMove)
									addTarget(unit, candidate.rooms[unit][y - 1], moves + dist + y);
							}
							break;
						}
						else if (y == D - 1)
						{
							addTarget(unit, tile, moves + dist + y + 1);
						}
					}
				}
			};

			// check if anyone in the hall is clear to move to their destination
			for_index(char& tile : candidate.hall)
				if (tile < 5)
					moveToTarget(tile, i, 0);

			// Check all rooms if anyone needs to move out of the way
			for_range_v(room, 4)
			{
				uint moveFrom = D;

				// Move if there are any incorrect units in the room
				for_index_v(y, auto tile : candidate.rooms[room])
				{
					if (tile != 5)
					{
						for (uint yy = y; yy < D; ++yy)
							if (candidate.rooms[room][yy] != room)
							{
								moveFrom = y;
								break;
							}

						break;
					}
				}

				if (moveFrom == D) continue;

				char& unit = candidate.rooms[room][moveFrom];

				uint const start = 2 + room * 2;
				for (uint hall = start - 1; hall < 11; --hall)
					if (candidate.hall[hall] != 5)
						break;
					else if (GC_IsOdd(hall) || hall == 0)
					{
						addTarget(unit, candidate.hall[hall], start - hall + moveFrom + 1);
					}
				for (uint hall = start + 1; hall < 11; ++hall)
					if (candidate.hall[hall] != 5)
						break;
					else if (GC_IsOdd(hall) || hall == 10)
					{
						addTarget(unit, candidate.hall[hall], hall - start + moveFrom + 1);
					}

				// Also check if this unit can move directly to its target room
				moveToTarget(unit, start, moveFrom + 1);
			}
		}, 100000).GetDefault(0);
}

DEFINE_TEST_G(Part1, Day23)
{
	TEST_EQ(locPart1("AOC_Day23Test.txt"), 12521);
	TEST_EQ(locPart1("AOC_Day23Part1.txt"), 13066);
}

DEFINE_TEST_G(Part2, Day23)
{
	TEST_EQ(locPart1<4>("AOC_Day23Test2.txt"), 44169);
	TEST_EQ(locPart1<4>("AOC_Day23Part2.txt"), 47328);
}
