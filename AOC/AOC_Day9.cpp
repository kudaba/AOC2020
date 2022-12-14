#include "AOC_Precompiled.h"

struct State : GC_HashSet<GC_String>
{
	bool operator==(State const& anOther) const
	{
		if (Bits != anOther.Bits)
			return false;

		if (!anOther.End)
			for_range(Count())
			if (GetByIndex(i) != anOther.GetByIndex(i))
				return false;

		return true;
	}

	uint64 Bits = 0;
	bool End = false;
};

inline uint32 GC_GetHash(State const& anItem, uint32 aSeed = 0)
{
	for (auto const& i : anItem)
		aSeed = GC_GetHash(i, aSeed);
	return aSeed;
}

static auto locPart1(char const* aFile, int mult = 1)
{
	GC_HashMap<GC_String, GC_HashMap<GC_String,uint>> alldests;
	State allroutes;
	allroutes.End = true;

	uint maxCost = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<2>(line, " = ");
		auto names = GC_StrSplit<2>(parts[0], " to ");

		uint const cost = GC_Atoi(parts[1]);
		maxCost = GC_Max(maxCost, cost);

		{
			auto& dests = alldests.GetOrAdd(names[0]);
			dests.Add(names[1], cost);
		}
		{
			auto& dests = alldests.GetOrAdd(names[1]);
			dests.Add(names[0], cost);
		}

		allroutes.Add(names[0]);
		allroutes.Add(names[1]);
	}
	allroutes.Bits = (1 << allroutes.Count()) - 1;

	GC_DynamicArray<GC_Pair<State, uint>> routes;
	routes.Reserve(allroutes.Count() * allroutes.Count());
	for_index(char const* r : allroutes)
	{
		auto& nr = routes.Add();
		nr.First.Add(r);
		nr.First.Bits = 1ull << i;
		nr.Second = 0;
	}

	State finalState;
	RunDijsktraLongStep<State, int>(State(), allroutes, [&](auto const& state, auto pred)
	{
	    if (state.Count() == 0)
	    {
	        for_index(char const* r : allroutes)
	        {
				State s;
				s.Add(r);
				s.Bits = 1ull << i;
				pred(s, 0);
	        }
	    }
	    else
	    {
	        if (auto const& dests = alldests.Find(state.Last()))
	        {
	            for (auto const& d : *dests)
	            {
	                if (!state.Contains(d.Key))
	                {
						State s = state;
						s.Add(d.Key);
						s.Bits |= 1ull << allroutes.IndexOf(*allroutes.Find(d.Key));
						pred(s, mult > 0 ? d.Value : (maxCost - d.Value));
	                }
	            }
	        }
	    }
	}, 0, &finalState);

	uint cost = 0;
	for_range(finalState.Count() - 1)
	{
		cost += *alldests.Find(finalState.GetByIndex(i))->Find(finalState.GetByIndex(i + 1));
	}
	return cost;
}

DEFINE_TEST_G(Part1, Day9)
{
	TEST_EQ(locPart1("AOC_Day9Test.txt"), 605);
	TEST_EQ(locPart1("AOC_Day9Part1.txt"), 207);
}

DEFINE_TEST_G(Part2, Day9)
{
	TEST_EQ(locPart1("AOC_Day9Test.txt", -1), 982);
	TEST_EQ(locPart1("AOC_Day9Part1.txt", -1), 804);
}
