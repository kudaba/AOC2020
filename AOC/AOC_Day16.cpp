#include "AOC_Precompiled.h"

struct Zone
{
	GC_HashMap<GC_String, uint> Tunnels;
	GC_DynamicArray<uint> Paths;
	uint32 FlowRate;
	uint32 Index;
};

static GC_HashMap<GC_String, Zone> locLoad(char const* aFile)
{
	GC_HashMap<GC_String, Zone> Map;
	for_index_v(s, auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<32>(line, ' ');
		Zone z;
		z.FlowRate = GC_Atoi(GC_StrSplit<2>(parts[4], '=')[1]);
		z.Index = s;

		for (uint i = 9; i < parts.Count(); ++i)
			z.Tunnels.Add(parts[i].Left(2), 0);

		Map.Add(parts[1], GC_Move(z));
	}

	for(auto& z : Map)
		for (auto& t : z.Value.Tunnels)
			t.Value = Map[t.Key].Index;

	for_index_v(s, auto& z : Map)
	{
		for_index(auto& y : Map)
		{
			uint path = 0;
			if (&z != &y && y.Value.FlowRate)
			{
				path = RunDijsktraShortStep(s, i, [&](auto const& pos, auto add)
					{
						for (auto& t : Map.GetByIndex(pos).Value.Tunnels)
							add(t, 1);
					}).GetDefault(0);
			}
			z.Value.Paths.Add(path);
		}
	}

	return Map;
}

static auto locPart1(char const* aFile, uint Steps, uint People, uint Random)
{
	GC_HashMap<GC_String, Zone> Map = locLoad(aFile);

	GC_DynamicArray<Zone const*> zones;
	for (Zone const& z : Map)
	{
		if (z.FlowRate)
			zones.Add(&z);
	}

	uint mid = People == zones.Count() / People;

	Zone const* aa = Map.Find("AA");
	auto calc = [&]()
	{
		uint flow = 0;

		for_range(2)
		{
			Zone const* c = aa;
			uint minute = 0;
			for (int zi = i * mid, ze = i ? zones.Count() : mid; zi < ze; ++zi)
			{
				Zone const* z = zones[zi];
				minute += c->Paths[z->Index] + 1;
				if (minute >= Steps)
					break;

				flow += z->FlowRate * (Steps - minute);
				c = z;
			}
		}

		return flow;
	};

	GC_Random r(Random);

	uint cur = calc();
	for (double temp = 100, change = 0.002f; (temp > change); temp -= change)
	{
		uint oldMid = mid;
		uint changeMid = r.Rand(People);
		uint i = r.Rand(zones.Count());
		uint j = r.Rand(zones.Count());
		while (i == j) j = r.Rand(zones.Count());

		if (changeMid)
			mid = r.Rand(zones.Count());
		else
			GC_Swap(zones[i], zones[j]);

		uint flow = calc();

		if (flow > cur || exp((flow - (float)cur) / temp) > r.FRand(1.f))
		{
			cur = flow;
		}
		else
		{
			if (changeMid)
				mid = oldMid;
			else
				GC_Swap(zones[i], zones[j]);
		}
	}

	return cur;
}

DEFINE_TEST_G(Part1, Day16)
{
	TEST_EQ(locPart1("AOC_Day16Test.txt", 30, 1, 0), 1651);
	TEST_EQ(locPart1("AOC_Day16Part1.txt", 30, 1, 0), 2077);
}

DEFINE_TEST_G(Part2, Day16)
{
	TEST_EQ(locPart1("AOC_Day16Test.txt", 26, 2, 1758691438), 1707);
	TEST_EQ(locPart1("AOC_Day16Part1.txt", 26, 2, 1779942239), 2741);
}
