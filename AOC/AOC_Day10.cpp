#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, bool isCorrupt)
{
	uint64 cresult = 0;
	GC_DynamicArray<uint64> iresults;

	GC_DynamicArray<char> stack;
	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		uint corrupt = 0;
		stack.Clear();
		for (char c : line)
		{
			switch (c)
			{
			case '(':
			case '{':
			case '<':
			case '[':
				stack.PushBack(c);
				break;
			case ')':
				if (stack.Last() == '(')
					stack.PopBack();
				else
					corrupt = 3;
				break;
			case '}':
				if (stack.Last() == '{')
					stack.PopBack();
				else
					corrupt = 1197;
				break;
			case '>':
				if (stack.Last() == '<')
					stack.PopBack();
				else
					corrupt = 25137;
				break;
			case ']':
				if (stack.Last() == '[')
					stack.PopBack();
				else
					corrupt = 57;
				break;
			}

			if (corrupt)
				break;
		}

		if (corrupt)
			cresult += corrupt;
		else
		{
			uint64 inc = 0;
			while (stack.Count())
			{
				inc *= 5;
				switch (stack.Last())
				{
				case '[': inc += 2; break;
				case '(': inc += 1; break;
				case '{': inc += 3; break;
				case '<': inc += 4; break;
				}
				stack.PopBack();
			}

			iresults.Add(inc);
		}
	}

	if (isCorrupt)
		return cresult;

	iresults.Sort();
	return iresults[iresults.Count() / 2];
}

DEFINE_TEST_G(Part1, Day10)
{
	TEST_EQ(locPart1("AOC_Day10Test.txt", true), 26397);
	TEST_EQ(locPart1("AOC_Day10Part1.txt", true), 323613);
}

DEFINE_TEST_G(Part2, Day10)
{
	TEST_EQ(locPart1("AOC_Day10Test.txt", false), 288957);
	TEST_EQ(locPart1("AOC_Day10Part1.txt", false), 3103006161);
}
