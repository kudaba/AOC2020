#include "AOC_Precompiled.h"
#include "AOC_Day3.h"

template <uint Size>
uint countTrees(char const* (&lines)[Size], uint step, uint stepy = 1)
{
	uint trees = 0;
	uint len = GC_Strlen(lines[0]);
	uint offset = 0;
	for (uint i = 0; i < Size; i += stepy)
	{
		if (lines[i][offset] == '#') ++trees;
		offset = (offset + step) % len;
	}
	return trees;
}

DEFINE_TEST_G(Part1, Day3)
{
	TEST_EQ(countTrees(testInput, 3), 7);
	TEST_EQ(countTrees(part1Input, 3), 203);

	uint trees = 0, offset = 0;
	for (auto line : GC_File::ReadAllLines("AOC_Day3Part1.txt"))
		trees += line[offset%line.Count()] == '#', offset += 3;
	TEST_EQ(trees, 203);
}

template <uint Size>
uint productTrees(char const* (&lines)[Size])
{
	return
		countTrees(lines, 1, 1) *
		countTrees(lines, 3, 1) *
		countTrees(lines, 5, 1) *
		countTrees(lines, 7, 1) *
		countTrees(lines, 1, 2);
}

DEFINE_TEST_G(Part2, Day3)
{
	TEST_EQ(productTrees(testInput), 336);
	TEST_EQ(productTrees(part1Input), 3316272960);
}
