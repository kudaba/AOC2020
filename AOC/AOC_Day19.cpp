#include "AOC_Precompiled.h"

struct BluePrint
{
	GC_Vector4u Reqs[4] = {};
};

static auto locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<BluePrint>(aFile, [](auto aLine)
		{
			BluePrint bp;

			for_index(auto r : GC_TokenizeString(aLine, ". "))
			{
				auto c = GC_StrSplit<2>(r, "costs ")[1];
				for (auto m : GC_TokenizeString(c, " and "))
				{
					auto p = GC_StrSplit(m, ' ');
					if (p[1].Last() == '.')
						p[1] = p[1].Left(p[1].Count() - 1);
					uint t = (GC_String("ore") == p[1]) ? 0 : ((GC_String("clay") == p[1]) ? 1 : 2);
					bp.Reqs[i][t] = GC_Atoi(p[0]);
				}
			}

			return bp;
		});
}

struct State
{
	GC_Vector4u resources = {};
	GC_Vector4u robots = { 1, 0, 0, 0 };
	uint choice = 0;
};

static uint locGetMax(BluePrint const& bp, uint minutes)
{
	GC_Vector4u maxRobots(
		GC_Max(bp.Reqs[1].x, bp.Reqs[2].x),
		bp.Reqs[2].y,
		bp.Reqs[3].z,
		99
	);

	GC_DynamicArray<State> states;
	states.Reserve(5000000);

	for_range(4)
	{
		State s;
		s.choice = i;
		states.Add(s);
	}

	uint maxGeodeRobots = 0;
	uint maxGeodes = 0;

	uint const maxGeodeRobotsDiff = 1;
	
	for_range_v(m, minutes)
	{
		for (uint i = 0, e = states.Count(); i < e; ++i)
		{
			State const& s = states[i];
			if ((s.robots[s.choice] == maxRobots[s.choice]) ||
				(maxGeodeRobots > maxGeodeRobotsDiff && states[i].robots.w < maxGeodeRobots - maxGeodeRobotsDiff))
			{
				states.RemoveAtFast(i--);
				--e;
			}
		}

		for (uint i = 0, e = states.Count(); i < e; ++i)
		{
			State& s = states[i];

			bool const build = s.resources >= bp.Reqs[s.choice];
			s.resources += s.robots;
			maxGeodes = GC_Max(maxGeodes, s.resources.w);

			if (build)
			{
				s.resources -= bp.Reqs[s.choice];
				s.robots[s.choice]++;
				maxGeodeRobots = GC_Max(maxGeodeRobots, s.robots.w);

				if (s.resources >= bp.Reqs[3])
				{
					s.choice = 3;
				}
				else
				{
					State ns = s;
					uint const choice = s.choice;

					for_range_v(c, 4u)
						if (c != choice && ns.robots[c] < maxRobots[c])
						{
							ns.choice = c;
							states.Add(ns);
						}
				}
			}
		}
	}

	return maxGeodes;
}

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

	for_index(auto item : locParseData(aFile))
	{
		result += locGetMax(item, 24) * (i + 1);
	}

	return result;
}

DEFINE_TEST_G(Part1, Day19)
{
	TEST_EQ(locPart1("AOC_Day19Test.txt"), 33);
	TEST_EQ(locPart1("AOC_Day19Part1.txt"), 1346);
}

static auto locPart2(char const* aFile)
{
	uint64 result = 1;

	for_index(auto item : locParseData(aFile))
	{
		result *= locGetMax(item, 32);
		if (i == 2)
			break;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day19)
{
	TEST_EQ(locPart2("AOC_Day19Test.txt"), 56 * 62);
	TEST_EQ(locPart2("AOC_Day19Part1.txt"), 7644);
}
