#include "AOC_Precompiled.h"


static auto locPart1(char const* aFile)
{
	GC_DynamicArray<GC_DynamicArray<char>> stacks;

	bool readingStacks = true;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		if (line.IsEmpty())
			readingStacks = false;
		else if (readingStacks)
		{
			for (uint i = 0; i < line.Count(); i += 4)
			{
				uint stack = i / 4;
				if (line[i] == '[')
				{
					if (stack >= stacks.Count())
						stacks.Resize(stack + 1);

					stacks[stack].PushFront(line[i + 1]);
				}
			}
		}
		else
		{
			auto parts = GC_StrSplit<6>(line, ' ');
			uint count = GC_Atoi(parts[1]);
			uint from = GC_Atoi(parts[3]) - 1;
			uint to = GC_Atoi(parts[5]) - 1;

			for_range(count)
			{
				stacks[to].PushBack(stacks[from].Last());
				stacks[from].PopBack();
			}
		}
	}

	GC_String result;
	result.Reserve(stacks.Count());
	result[stacks.Count()] = 0;

	for_index(auto const& stack : stacks)
	{
		result[i] = stack.Last();
	}

	return result;
}

DEFINE_TEST_G(Part1, Day5)
{
	TEST_STR_EQ(locPart1("AOC_Day5Test.txt"), "CMZ");
	TEST_STR_EQ(locPart1("AOC_Day5Part1.txt"), "WHTLRMZRC");
}

static auto locPart2(char const* aFile)
{
	GC_DynamicArray<GC_DynamicArray<char>> stacks;

	bool readingStacks = true;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		if (line.IsEmpty())
			readingStacks = false;
		else if (readingStacks)
		{
			for (uint i = 0; i < line.Count(); i += 4)
			{
				uint stack = i / 4;
				if (line[i] == '[')
				{
					if (stack >= stacks.Count())
						stacks.Resize(stack + 1);

					stacks[stack].PushFront(line[i + 1]);
				}
			}
		}
		else
		{
			auto parts = GC_StrSplit<6>(line, ' ');
			uint count = GC_Atoi(parts[1]);
			uint from = GC_Atoi(parts[3]) - 1;
			uint to = GC_Atoi(parts[5]) - 1;

			stacks[to] += stacks[from].Right(count);
			stacks[from].Resize(stacks[from].Count() - count);
		}
	}

	GC_String result;
	result.Reserve(stacks.Count());
	result[stacks.Count()] = 0;

	for_index(auto const& stack : stacks)
	{
		result[i] = stack.Last();
	}

	return result;
}

DEFINE_TEST_G(Part2, Day5)
{
	TEST_STR_EQ(locPart2("AOC_Day5Test.txt"), "MCD");
	TEST_STR_EQ(locPart2("AOC_Day5Part1.txt"), 0);
}
