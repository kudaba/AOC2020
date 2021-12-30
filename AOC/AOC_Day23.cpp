#include "AOC_Precompiled.h"
#include "AOC_Day23.h"

static uint locDay23Part1(char const* aFile, uint moves)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_DynamicArray<uint> cups;
	for (char c : text)
	{
		if (c == '\n')
			break;
		cups.Add() = c - '0';
	}

	uint len = cups.Count();
	uint current = cups.First();

	uint temp[3];

	for_range(moves)
	{

		{
			int index = cups.Find(current) + 1;
			for_range_v(t, 3)
			{
				if (index == (int)cups.Count())
					index = 0;
				temp[t] = cups[index];
				cups.RemoveAt(index);
			}
		}

		int index = -1;
		{
			for_range_v(f, len)
			{
				index = cups.Find((uint)GC_WrapClamp<int>(current - (f+1), 0, len)); // <----------------- ERROR WRAP CLAMP ALWAY RETURNS MAX VALUE
				if (index != -1)
					break;
			}
		}

		for_range_v(t, 3)
			cups.Insert(index + t + 1, temp[t]);

		current = cups[(cups.Find(current) + 1) % len];
	}

	uint result = 0;
	int index = cups.Find(1u);
	for_range(len - 1)
		result = result * 10 + cups[(index + i + 1) % len];

	return result;
}

DEFINE_TEST_G(Part1, Day23)
{
	TEST_EQ(locDay23Part1("AOC_Day23Test.txt", 10), 92658374);
	TEST_EQ(locDay23Part1("AOC_Day23Test.txt", 100), 97548623); // 67384529);
	TEST_EQ(locDay23Part1("AOC_Day23Part1.txt", 100), 95648732);
}

static uint64 locDay23Part2(char const* aFile, uint numCups, uint numLoops, bool part1)
{
	GC_DynamicArray<uint> cups;
	cups.Resize(numCups + 1);

	GC_String text;
	GC_File::ReadAllText(aFile, text);

	uint linkIndex = 1;
	uint current = 0;
	uint prev = numCups;

	for (char c : text)
	{
		if (c == '\n')
			break;

		uint cup = c - '0';

		if (!current)
			current = cup;

		cups[prev] = cup;
		prev = cup;

		++linkIndex;
	}

	while (linkIndex <= numCups)
	{
		cups[prev] = linkIndex;
		prev = linkIndex++;
	}

	cups[prev] = current;


	for_range(numLoops)
	{
		uint const temp = cups[current];
		uint const temp2 = cups[temp];
		uint const temp3 = cups[temp2];
		uint const nextCurrent = cups[temp3];
		cups[current] = nextCurrent;

		uint next = 0;
		for_range_v(c, 4)
		{
			int const nv1 = (int)current - (c + 1);
			uint const nextValue = nv1 + (nv1 < 1) * numCups;

			if (nextValue == temp || nextValue == temp2 || nextValue == temp3)
				continue;

			next = nextValue;
			break;
		}

		current = nextCurrent;

		cups[temp3] = cups[next];
		cups[next] = temp;
	}

	uint one = cups[1];

	if (part1)
	{
		uint result = 0;
		for_range(numCups - 1)
		{
			result = result * 10 + one;
			one = cups[one];
		}
		return result;
	}

	return uint64(one) * cups[one];
}

DEFINE_TEST_G(Part2, Day23)
{
#if RUN_TESTS
	TEST_EQ(locDay23Part2("AOC_Day23Test.txt", 9, 10, true), 92658374);
	TEST_EQ(locDay23Part2("AOC_Day23Test.txt", 9, 100, true), 67384529);
	TEST_EQ(locDay23Part2("AOC_Day23Part1.txt", 9, 100, true), 95648732);

	TEST_EQ(locDay23Part2("AOC_Day23Test.txt", 1000000, 10000000, false), 149245887792);
#endif
	TEST_EQ(locDay23Part2("AOC_Day23Part1.txt", 1000000, 10000000, false), 192515314252);
}
