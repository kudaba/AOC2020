#include "AOC_Precompiled.h"

GC_DECLARE_ENUM(nodeop,
	Set,
	AND,
	OR,
	LSHIFT,
	RSHIFT,
	NOT,
	Evaluated);

struct node
{
	mutable nodeop op;
	GC_String left;
	GC_String right;
	mutable uint16 result;

	static uint16 eval(GC_HashMap<GC_String, node> const& nodes, char const* input)
	{
		if (GC_IsDigit(input[0]))
			return (uint16)GC_Atou(input);
		return nodes[input].eval(nodes);
	}

	uint16 eval(GC_HashMap<GC_String, node> const& nodes) const
	{
		switch (op)
		{
		case nodeop::Set: op = nodeop::Evaluated; result = eval(nodes, left); return result;
		case nodeop::AND: op = nodeop::Evaluated; result = eval(nodes, left) & eval(nodes, right); return result;
		case nodeop::OR: op = nodeop::Evaluated; result = eval(nodes, left) | eval(nodes, right); return result;
		case nodeop::LSHIFT: op = nodeop::Evaluated; result = eval(nodes, left) << eval(nodes, right); return result;
		case nodeop::RSHIFT: op = nodeop::Evaluated; result = eval(nodes, left) >> eval(nodes, right); return result;
		case nodeop::NOT: op = nodeop::Evaluated; result = ~eval(nodes, left); return result;
		case nodeop::Evaluated: return result;
		default:
			GC_UNREACHABLE();
			return 0;
		}
	}
};

static auto locPart1(char const* aFile, char const* wire, GC_Optional<uint16> overrideb = nullopt)
{
	GC_HashMap<GC_String, node> nodes;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit(line, " -> ");
		node& n = nodes.GetOrAdd(parts[1]);

		auto nodeparts = GC_StrSplit(parts[0], ' ');
		if (nodeparts.Count() == 1)
		{
			n.op = nodeop::Set;
			n.left = nodeparts[0];
		}
		else if (nodeparts.Count() == 2)
		{
			n.op = nodeop::NOT;
			n.left = nodeparts[1];
		}
		else
		{
			n.left = nodeparts[0];
			n.op = nodeop::FromString(GC_String(nodeparts[1]));
			n.right = nodeparts[2];
		}
	}

	if (overrideb.IsSet())
	{
		node const& node = nodes["b"];
		node.op = nodeop::Evaluated;
		node.result = overrideb.Get();
	}

	return nodes[wire].eval(nodes);
}

DEFINE_TEST_G(Part1, Day7)
{
	TEST_EQ(locPart1("AOC_Day7Test.txt", "d"), 72);
	TEST_EQ(locPart1("AOC_Day7Test.txt", "e"), 507);
	TEST_EQ(locPart1("AOC_Day7Test.txt", "f"), 492);
	TEST_EQ(locPart1("AOC_Day7Test.txt", "g"), 114);
	TEST_EQ(locPart1("AOC_Day7Test.txt", "h"), 65412);
	TEST_EQ(locPart1("AOC_Day7Test.txt", "i"), 65079);
	TEST_EQ(locPart1("AOC_Day7Test.txt", "x"), 123);
	TEST_EQ(locPart1("AOC_Day7Test.txt", "y"), 456);
	TEST_EQ(locPart1("AOC_Day7Part1.txt", "a"), 3176);
}

DEFINE_TEST_G(Part2, Day7)
{
	TEST_EQ(locPart1("AOC_Day7Part1.txt", "a", (uint16)3176), 14710);
}
