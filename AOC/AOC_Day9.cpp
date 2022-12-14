#include "AOC_Precompiled.h"

struct State : GC_DynamicArray<GC_String>
{
	bool operator==(State const& anOther) const
	{
		if (Count() != anOther.Count())
			return false;

		if (!anOther.End)
		for_range(Count())
			if ((*this)[i] != anOther[i])
				return false;

		return true;
	}

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
	struct Dest
	{
		GC_String Name;
		uint Cost;
	};

	GC_HashMap<GC_String, GC_DynamicArray<Dest>> alldests;
	State allroutes;
	allroutes.End = true;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<2>(line, " = ");
		auto names = GC_StrSplit<2>(parts[0], " to ");

		{
			auto& dests = alldests.GetOrAdd(names[0]);
			auto& dest = dests.Add();
			dest.Name = names[1];
			dest.Cost = GC_Atoi(parts[1]);
		}
		{
			auto& dests = alldests.GetOrAdd(names[1]);
			auto& dest = dests.Add();
			dest.Name = names[0];
			dest.Cost = GC_Atoi(parts[1]);
		}

		allroutes.AddUnique(names[0]);
		allroutes.AddUnique(names[1]);
	}


	//return RunDijsktraLongStep<State, int>(State(), allroutes, [&](auto const& state, auto pred)
	//{
	//	if (state.Count() == 0)
	//	{
	//		State s;
	//		s.Add();
	//		for (char const* r : allroutes)
	//		{
	//			s[0] = r;
	//			pred(s, 0);
	//		}
	//	}
	//	else
	//	{
	//		if (auto const& dests = alldests.Find(state.Last()))
	//		{
	//			State s = state;
	//			s.Add();
	//			for (auto const& d : *dests)
	//			{
	//				if (!s.Contains(d.Name))
	//				{
	//					s.Last() = d.Name;
	//					pred(s, d.Cost * mult);
	//				}
	//			}
	//		}
	//	}
	//}).GetDefault(0) * mult;

	GC_DynamicArray<GC_Pair<State, uint>> routes;
	routes.Reserve(allroutes.Count() * allroutes.Count());
	for (char const* r : allroutes)
	{
		auto& nr = routes.Add();
		nr.First.Add(r);
		nr.Second = 0;
	}

	for_range(allroutes.Count() - 1)
	{
		auto oldroutes = GC_Move(routes);
		routes.Reserve(oldroutes.Count() * 2);

		for (auto& r : oldroutes)
		{
			if (auto const& dests = alldests.Find(r.First.Last()))
			{
				for (auto const& d : *dests)
				{
					if (!r.First.Contains(d.Name))
					{
						auto& nr = routes.Add();
						nr.First.Reserve(allroutes.Count());
						nr = r;
						nr.First.Add(d.Name);
						nr.Second += d.Cost;
					}
				}
			}
		}
	}

	if (mult > 0)
	{
		uint cost = UINT_MAX;
		for (auto const& r : routes)
		{
			cost = GC_Min(cost, r.Second);
		}
		return cost;
	}
	else
	{
		uint cost = 0;
		for (auto const& r : routes)
		{
			cost = GC_Max(cost, r.Second);
		}
		return cost;
	}
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
