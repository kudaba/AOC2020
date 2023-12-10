#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	auto parts = GC_StrSplit<2>(text, "\n\n");

	auto Dirs = parts[0];

	using Node = GC_Pair<GC_String, GC_String>;
	GC_HashMap<GC_String, Node> Nodes;

	for (auto line : GC_StrSplit<32>(parts[1], '\n'))
	{
		auto nodeParts = GC_StrSplit<2>(line, " = (");
		auto dirParts = GC_StrSplit<2>(nodeParts[1], ", ");

		Node& node = Nodes.GetOrAdd(GC_String(nodeParts[0].Buffer(), 3));
		node.First = GC_String(dirParts[0].Buffer(), 3);
		node.Second = GC_String(dirParts[1].Buffer(), 3);
	}

	GC_String pos = "AAA";
	uint dirIndex = 0;
	uint Steps = 0;
	while (pos != "ZZZ")
	{
		pos = Dirs[dirIndex++] == 'L' ? Nodes[pos].First : Nodes[pos].Second;
		if (dirIndex == Dirs.Count())
			dirIndex = 0;

		++Steps;
	}

	return Steps;
}

DEFINE_TEST_G(Part1, Day8)
{
	TEST_EQ(locPart1("AOC_Day8Test.txt"), 6);
	TEST_EQ(locPart1("AOC_Day8Part1.txt"), 19199);
}

static auto locPart2(char const* aFile)
{
	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	auto parts = GC_StrSplit<2>(text, "\n\n");

	auto Dirs = parts[0];

	using Node = GC_Pair<GC_String, GC_String>;
	GC_HashMap<GC_String, Node> Nodes;
	GC_DynamicArray<GC_String> Ghosts;

	for (auto line : GC_StrSplit<32>(parts[1], '\n'))
	{
		auto nodeParts = GC_StrSplit<2>(line, " = (");
		auto dirParts = GC_StrSplit<2>(nodeParts[1], ", ");

		GC_String nodeName = GC_String(nodeParts[0].Buffer(), 3);
		Node& node = Nodes.GetOrAdd(nodeName);
		node.First = GC_String(dirParts[0].Buffer(), 3);
		node.Second = GC_String(dirParts[1].Buffer(), 3);

		if (nodeName[2] == 'A')
			Ghosts.Add(nodeName);
	}

	GC_DynamicArray<uint> Cycles;

	for (GC_String& g : Ghosts)
	{
		uint dirIndex = 0;
		uint cycle = 0;
		while (g[2] != 'Z')
		{
			g = Dirs[dirIndex++] == 'L' ? Nodes[g].First : Nodes[g].Second;
			if (dirIndex == Dirs.Count())
				dirIndex = 0;

			++cycle;
		}

		Cycles.Add(cycle);
	}

	return GC_Math::GetLowestCommonMultiple(Cycles);
}

DEFINE_TEST_G(Part2, Day8)
{
	TEST_EQ(locPart2("AOC_Day8Test2.txt"), 6);
	TEST_EQ(locPart2("AOC_Day8Part1.txt"), 13663968099527);
}
