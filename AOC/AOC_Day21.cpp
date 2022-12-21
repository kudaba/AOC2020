#include "AOC_Precompiled.h"

struct Operation
{
	char Op;
	GC_String Left;
	GC_String Right;
};

static bool locHumnHit;

struct Node
{
	GC_Optional<int64> Constant;
	GC_Optional<Operation> Op;

	int64 Eval(GC_HashMap<GC_String, Node> const& nodes) const
	{
		if (Constant.IsSet())
			return Constant.Get();

		Operation const& op = Op.Get();

		if (op.Left == "humn" || op.Right == "humn")
			locHumnHit = true;

		int64 l = nodes[op.Left].Eval(nodes);
		int64 r = nodes[op.Right].Eval(nodes);
		switch (op.Op)
		{
		case '+': return l + r;
		case '-': return l - r;
		case '*': return l * r;
		case '/': return l / r;
		}
		return 0;
	}


	void Eval(GC_HashMap<GC_String, Node> const& nodes, int64 expected, int64& result) const
	{
		GC_ASSERT(Constant.IsSet() == false);
		Operation const& op = Op.Get();

		int64 test = 0;

		GC_Optional<bool> humnLeft;

		if (op.Left == "humn")
			humnLeft = true;
		else if (op.Right == "humn")
			humnLeft = false;

		locHumnHit = false;
		test = nodes[op.Left].Eval(nodes);

		if (!humnLeft.IsSet())
			humnLeft = locHumnHit;

		if (!humnLeft.Get())
		{
			int64 next = 0;
			switch (op.Op)
			{
			case '+': next = expected - test; break;
			case '-': next = -(expected - test); break;
			case '*': next = expected / test; break;
			case '/': next = test / expected; break;
			}

			if (op.Right == "humn")
				result = next;
			else
				nodes[op.Right].Eval(nodes, next, result);
		}
		else
		{
			test = nodes[op.Right].Eval(nodes);

			int64 next = 0;
			switch (op.Op)
			{
			case '+': next = expected - test; break;
			case '-': next = expected + test; break;
			case '*': next = expected / test; break;
			case '/': next = test * expected; break;
			}

			if (op.Left == "humn")
				result = next;
			else
				nodes[op.Left].Eval(nodes, next, result);
		}
	}
};

static GC_HashMap<GC_String, Node> locLoad(char const* aFile)
{
	GC_HashMap<GC_String, Node> nodes;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<2>(line, ": ");
		auto opParts = GC_StrSplit<3>(parts[1], ' ');

		Node n;
		if (opParts.Count() == 1)
		{
			n.Constant = GC_Atoi(opParts[0]);
		}
		else
		{
			n.Op = { opParts[1][0], opParts[0], opParts[2] };
		}

		nodes.Add(parts[0], n);
	}

	return nodes;
}

static auto locPart1(char const* aFile)
{
	GC_HashMap<GC_String, Node> nodes = locLoad(aFile);
	return nodes["root"].Eval(nodes);
}

DEFINE_TEST_G(Part1, Day21)
{
	TEST_EQ(locPart1("AOC_Day21Test.txt"), 152);
	TEST_EQ(locPart1("AOC_Day21Part1.txt"), 121868120894282);
}

static auto locPart2(char const* aFile)
{
	GC_HashMap<GC_String, Node> nodes = locLoad(aFile);

	locHumnHit = false;

	Operation const& root = nodes["root"].Op.Get();
	int64 left = nodes[root.Left].Eval(nodes);
	bool humnLeft = locHumnHit;

	int64 result = 0;
	if (!humnLeft)
	{
		nodes[root.Right].Eval(nodes, left, result);
	}
	else
	{
		int64 right = nodes[root.Right].Eval(nodes);
		nodes[root.Left].Eval(nodes, right, result);
	}
	return result;
}

DEFINE_TEST_G(Part2, Day21)
{
	TEST_EQ(locPart2("AOC_Day21Test.txt"), 301);
	TEST_EQ(locPart2("AOC_Day21Part1.txt"), 3582317956029);
}
