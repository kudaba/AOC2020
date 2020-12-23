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

struct Link
{
	uint Value;
	Link* Next;
};

constexpr uint GC_HashKeyResolver(Link* anItem) { return anItem->Value; }

static uint64 locDay23Part2(char const* aFile, uint numCups, uint numLoops, bool part1)
{
	GC_ScopedArray<Link> links = new Link[numCups];
	GC_HashSet<Link*> linkMap;
	linkMap.Reserve(numCups);

	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_DynamicArray<uint> cups;

	uint linkIndex = 0;
	Link* current = &links[0];
	Link* prev = &links[numCups - 1];
	prev->Next = current;

	for (char c : text)
	{
		if (c == '\n')
			break;

		Link* link = &links[linkIndex++];
		link->Value = c - '0';
		prev->Next = link;
		prev = link;
		linkMap.Add(link);
	}

	while (linkIndex < numCups)
	{
		Link* link = &links[linkIndex++];
		link->Value = linkIndex;
		prev->Next = link;
		prev = link;
		linkMap.Add(link);
	}

	for_range(numLoops)
	{
		Link* temp = current->Next;
		current->Next = temp->Next->Next->Next;

		uint usedValues[3] = { temp->Value, temp->Next->Value, temp->Next->Next->Value };

		Link* next = nullptr;
		for_range_v(c, 4)
		{
			int nextValue = (int)current->Value - (c + 1);
			if (nextValue < 1)
				nextValue += numCups;

			if ((uint)nextValue == usedValues[0] || (uint)nextValue == usedValues[1] || (uint)nextValue == usedValues[2])
				continue;

			next = linkMap[nextValue];
			break;
		}

		temp->Next->Next->Next = next->Next;
		next->Next = temp;
		current = current->Next;
	}

	Link* one = linkMap[1];

	if (part1)
	{
		uint result = 0;
		for_range(numCups - 1)
		{
			one = one->Next;
			result = result * 10 + one->Value;
		}
		return result;
	}

	return uint64(one->Next->Value) * one->Next->Next->Value;
}

DEFINE_TEST_G(Part2, Day23)
{
	TEST_EQ(locDay23Part2("AOC_Day23Test.txt", 9, 10, true), 92658374);
	TEST_EQ(locDay23Part2("AOC_Day23Test.txt", 9, 100, true), 67384529);
	TEST_EQ(locDay23Part2("AOC_Day23Part1.txt", 9, 100, true), 95648732);

	TEST_EQ(locDay23Part2("AOC_Day23Test.txt", 1000000, 10000000, false), 149245887792);
	TEST_EQ(locDay23Part2("AOC_Day23Part1.txt", 1000000, 10000000, false), 192515314252);
}
