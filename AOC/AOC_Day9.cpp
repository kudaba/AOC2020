#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, uint size)
{
	GC_HybridArray<GC_Vector2i, 10> rope;
	rope.Resize(size, GC_Vector2i(0));

	GC_HashSet<GC_Vector2i> visited;
	visited.Add(rope.Last());

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		GC_Vector2i dir(0);
		switch (line[0])
		{
		case 'R': dir.x += 1; break;
		case 'L': dir.x -= 1; break;
		case 'U': dir.y += 1; break;
		case 'D': dir.y -= 1; break;
		}

		for_range_v(x, GC_Atoi(line.Range(2)))
		{
			rope[0] += dir;

			for_range(size-1)
			{
				GC_Vector2i head = rope[i];
				GC_Vector2i& tail = rope[i+1];

				GC_Vector2i diff(GC_Abs(head.x - tail.x), GC_Abs(head.y - tail.y));
				if (diff.x == 0)
				{
					if (diff.y > 1)
					{
						tail.y += (head.y - tail.y) / diff.y;
					}
				}
				else if (diff.y == 0)
				{
					if (diff.x > 1)
					{
						tail.x += (head.x - tail.x) / diff.x;
					}
				}
				else if (diff.x > 1 || diff.y > 1)
				{
					tail += (head - tail) / diff;
				}
			}

			visited.Add(rope.Last());
		}
	}

	return visited.Count();
}

DEFINE_TEST_G(Part1, Day9)
{
	TEST_EQ(locPart1("AOC_Day9Test.txt", 2), 13);
	TEST_EQ(locPart1("AOC_Day9Part1.txt", 2), 6470);
}

DEFINE_TEST_G(Part2, Day9)
{
	TEST_EQ(locPart1("AOC_Day9Test2.txt", 10), 36);
	TEST_EQ(locPart1("AOC_Day9Part1.txt", 10), 2658);
}
