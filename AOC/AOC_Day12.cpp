#include "AOC_Precompiled.h"

struct Node
{
	uint16 Id;
	bool Big;

	GC_DynamicArray<uint16> Connected;
};

static auto locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<GC_Pair<Node, Node>>(aFile, [](auto aLine)
		{
			GC_StrSlice part;
			GC_Strtok(aLine, "-", part);
			uint16 s = part.Count() == 2 ? *(uint16 const*)part.begin() : (part[0] == 's' ? 0 : 1);
			bool sb = GC_IsUpper(part[0]);
			GC_Strtok(aLine, "-", part);
			uint16 e = part.Count() == 2 ? *(uint16 const*)part.begin() : (part[0] == 's' ? 0 : 1);
			bool eb = GC_IsUpper(part[0]);

			return GC_Pair<Node, Node>({ s,sb }, { e,eb });
		});
}

static uint64 locGetPaths(GC_HashMap<uint16, Node> const& nodes, uint16 nodeid, GC_HashSet<uint16>& visited)
{
	if (visited.Contains(nodeid))
		return 0;

	if (nodeid == 1)
		return 1;

	auto const& node = nodes[nodeid];

	if (!node.Big)
		visited.Add(nodeid);

	uint64 paths = 0;
	for (uint16 d : node.Connected)
		paths += locGetPaths(nodes, d, visited);

	if (!node.Big)
		visited.Remove(nodeid);

	return paths;
}

static auto locPart1(char const* aFile)
{
	GC_HashMap<uint16, Node> nodes;

	for (auto item : locParseData(aFile))
	{
		auto& node1 = nodes.GetOrAdd(item.myFirst.Id);
		node1.Big = item.myFirst.Big;
		node1.Connected.AddUnique(item.mySecond.Id);

		auto& node2 = nodes.GetOrAdd(item.mySecond.Id);
		node2.Big = item.mySecond.Big;
		node2.Connected.AddUnique(item.myFirst.Id);
	}

	GC_HashSet<uint16> visited;
	return locGetPaths(nodes, 0, visited);
}

DEFINE_TEST_G(Part1, Day12)
{
	TEST_EQ(locPart1("AOC_Day12Test.txt"), 226);
	TEST_EQ(locPart1("AOC_Day12Part1.txt"), 3679);
}

static uint64 locGetPaths2(GC_HashMap<uint16, Node> const& nodes, uint16 nodeid, GC_HashSet<uint16>& visited, uint16 visited2)
{
	if (visited.Contains(nodeid) && (nodeid == 0 || visited2 != 0))
		return 0;

	if (nodeid == 1)
		return 1;

	auto const& node = nodes[nodeid];

	if (!node.Big)
	{
		if (visited.Contains(nodeid))
			visited2 = nodeid;
		else
			visited.Add(nodeid);
	}

	uint64 paths = 0;
	for (uint16 d : node.Connected)
		paths += locGetPaths2(nodes, d, visited, visited2);

	if (!node.Big)
	{
		if (visited2 != nodeid)
			visited.Remove(nodeid);
	}

	return paths;
}

static auto locPart2(char const* aFile)
{
	GC_HashMap<uint16, Node> nodes;

	for (auto item : locParseData(aFile))
	{
		auto& node1 = nodes.GetOrAdd(item.myFirst.Id);
		node1.Big = item.myFirst.Big;
		node1.Connected.AddUnique(item.mySecond.Id);

		auto& node2 = nodes.GetOrAdd(item.mySecond.Id);
		node2.Big = item.mySecond.Big;
		node2.Connected.AddUnique(item.myFirst.Id);
	}

	GC_HashSet<uint16> visited;
	return locGetPaths2(nodes, 0, visited, 0);
}
DEFINE_TEST_G(Part2, Day12)
{
	TEST_EQ(locPart2("AOC_Day12Test.txt"), 3509);
	TEST_EQ(locPart2("AOC_Day12Part1.txt"), 107395);
}
