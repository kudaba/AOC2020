#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	GC_String text = GC_File::ReadAllText(aFile);
	int ymin = GC_Atoi(GC_StrSplit<2>(GC_StrSplit<4>(text, " ")[3].Range(2), "..")[0]);
	return GC_Algorithm::SumOfRange(-ymin) + ymin;
}

DEFINE_TEST_G(Part1, Day17)
{
	TEST_EQ(locPart1("AOC_Day17Test.txt"), 45);
	TEST_EQ(locPart1("AOC_Day17Part1.txt"), 5460);
}

static auto locPart2(char const* aFile)
{
	// By Block parsing (block of lines separate by two new lines)
	GC_String text = GC_File::ReadAllText(aFile);
	auto parts = GC_StrSplit<4>(text, " ");
	auto xparts = GC_StrSplit<2>(parts[2].Range(2), "..");
	auto yparts = GC_StrSplit<2>(parts[3].Range(2), "..");

	int xmin = GC_Atoi(xparts[0]);
	int xmax = GC_Atoi(xparts[1]);
	int ymin = GC_Atoi(yparts[0]);
	int ymax = GC_Atoi(yparts[1]);

	GC_HashMap<int, GC_HybridArray<int, 16>> validYSteps;

	int maxSteps = 0;
	for (int y = ymin; y <= -ymin; ++y)
		for (int sx = y > 0 ? -y-1 : y, tx = 0; tx >= ymin; tx += sx--)
			if (tx <= ymax)
			{
				maxSteps = GC_Max<int>(maxSteps, y - sx);
				validYSteps.GetOrAdd(y - sx).Add(y);
			}

	GC_HashSet<GC_Vector2i> targets;

	for_range_v(x, xmax+1)
		for (int sx = x, tx = 0; sx > 0 && tx <= xmax; tx += sx--)
			if (tx >= xmin)
			{
				if (tx + GC_Algorithm::SumOfRange(sx) <= xmax)
				{
					for (int i = x - sx; i <= maxSteps; ++i)
						if (GC_DynamicArray<int> const* yts = validYSteps.Find(i))
							for (int y : *yts)
								targets.Add({ x,y });
					break;
				}
				else
				{
					if (GC_DynamicArray<int> const* yts = validYSteps.Find(x - sx))
						for (int y : *yts)
							targets.Add({ x,y });
				}
			}

	return targets.Count();
}

DEFINE_TEST_G(Part2, Day17)
{
	TEST_EQ(locPart2("AOC_Day17Test.txt"), 112);
	TEST_EQ(locPart2("AOC_Day17Part1.txt"), 3618);
}
