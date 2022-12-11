#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, uint rounds, uint worry)
{
	struct Monkey
	{
		GC_DynamicArray<uint64> items;
		bool Add;
		uint Count;
		uint Mod;
		uint Targets[2];
		uint64 Inspect = 0;
	};

	GC_DynamicArray<Monkey> monkeys;

	uint64 superMod = 1;

	GC_String text = GC_File::ReadAllText(aFile);
	for (GC_StrSlice monkey : GC_StrSplit(text, "\n\n"))
	{
		Monkey& current = monkeys.Add();
		auto parts = GC_StrSplit<6>(monkey, '\n');
		{
			for (GC_StrSlice item : GC_StrSplit<8>(GC_StrSplit<2>(parts[1], ": ")[1], ", "))
			{
				current.items.Add(GC_Atoi(item));
			}
		}
		{
			auto op = GC_StrSplit<2>(GC_StrSplit<2>(parts[2], "old ")[1], ' ');
			current.Add = op[0][0] == '+';
			current.Count = GC_Atoi(op[1]);
		}
		current.Mod = GC_Atoi(GC_StrSplit<2>(parts[3], " by ")[1]);
		current.Targets[1] = GC_Atoi(GC_StrSplit<2>(parts[4], " monkey ")[1]);
		current.Targets[0] = GC_Atoi(GC_StrSplit<2>(parts[5], " monkey ")[1]);

		superMod *= current.Mod;
	}

	for_range(rounds)
	{
		for (Monkey& monkey : monkeys)
		{
			monkey.Inspect += monkey.items.Count();

			for (uint64 item : monkey.items)
			{
				item = ((monkey.Add ? (item + monkey.Count) : (item * (monkey.Count ? monkey.Count : item))) / worry) % superMod;
				monkeys[monkey.Targets[!(item % monkey.Mod)]].items.Add(item);
			}

			monkey.items.Clear();
		}
	}

	monkeys.Sort([](Monkey& l, Monkey& r) {return l.Inspect > r.Inspect; });

	return monkeys[0].Inspect * monkeys[1].Inspect;
}

DEFINE_TEST_G(Part1, Day11)
{
	TEST_EQ(locPart1("AOC_Day11Test.txt", 20, 3), 10605);
	TEST_EQ(locPart1("AOC_Day11Part1.txt", 20, 3), 64032);
}

DEFINE_TEST_G(Part2, Day11)
{
	TEST_EQ(locPart1("AOC_Day11Test.txt", 10000, 1), 2713310158);
	TEST_EQ(locPart1("AOC_Day11Part1.txt", 10000, 1), 12729522272);
}
