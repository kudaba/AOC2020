#include "AOC_Precompiled.h"
#include "AOC_Day18.h"

uint64 locEvaluate(GC_StrSlice line, uint& index)
{
	uint64 total = 0;
	bool isAdd = true;
	uint64 left = 0;

	while (index < line.Count())
	{
		char c = line[index];
		if (GC_IsDigit(c))
		{
			left = left * 10 + c - '0';
		}
		else if (c == '+' || c == '*')
		{
			if (isAdd) total += left;
			else total *= left;
			isAdd = c == '+';
			left = 0;
		}
		else if (c == '(')
		{
			++index;
			left = locEvaluate(line, index);
		}
		else if (c == ')')
		{
			break;
		}
		++index;
	}

	if (isAdd) total += left;
	else total *= left;
	return total;
}

static uint64 locDay18Part1(char const* aFile)
{
	uint64 result = 0;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		uint index = 0;
		result += locEvaluate(line, index);
	}

	return result;
}

DEFINE_TEST_G(Part1, Day18)
{
	TEST_EQ(locDay18Part1("AOC_Day18Test.txt"), 12240);
	TEST_EQ(locDay18Part1("AOC_Day18Part1.txt"), 202553439706);
}

uint64 locEvaluate2(GC_StrSlice line, uint& index)
{
	struct OP
	{
		bool isAdd = true;
		uint64 number = 0;
	};

	GC_HybridArray<OP, 8> ops;
	OP* op = &ops.Add();

	while (index < line.Count())
	{
		char c = line[index];
		if (GC_IsDigit(c))
		{
			op->number = op->number * 10 + c - '0';
		}
		else if (c == '+' || c == '*')
		{
			op = &ops.Add();
			op->isAdd = c == '+';
		}
		else if (c == '(')
		{
			++index;
			op->number = locEvaluate2(line, index);
		}
		else if (c == ')')
		{
			break;
		}
		++index;
	}

	for (uint i = 1; i < ops.Count(); ++i)
	{
		if (ops[i].isAdd)
		{
			ops[i - 1].number += ops[i].number;
			ops.RemoveAt(i--);
		}
	}

	uint64 total = ops[0].number;
	for (uint i = 1; i < ops.Count(); ++i)
		total *= ops[i].number;
	return total;
}

static uint64 locDay18Part2(char const* aFile)
{
	uint64 result = 0;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		uint index = 0;
		result += locEvaluate2(line, index);
	}

	return result;
}

DEFINE_TEST_G(Part2, Day18)
{
	TEST_EQ(locDay18Part2("AOC_Day18Test.txt"), 669060);
	TEST_EQ(locDay18Part2("AOC_Day18Part1.txt"), 88534268715686);
}
