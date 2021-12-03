#include "AOC_Precompiled.h"

static uint locPart1(char const* aFile)
{
	uint zeros[32] = { 0 };
	uint ones[32] = { 0 };

	uint len = 0;
	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		len = line.Count();
		for_index(char c : line)
		{
			if (c == '0')
				zeros[i]++;
			else
				ones[i]++;
		}
	}

	uint gamma = 0, eps = 0;
	for (uint i = 0; i < len; ++i)
	{
		gamma = (gamma << 1) + (ones[i] > zeros[i]);
		eps = (eps << 1) + (ones[i] < zeros[i]);
	}

	return gamma * eps;
}

DEFINE_TEST_G(Part1, Day3)
{
	TEST_EQ(locPart1("AOC_Day3Test.txt"), 198);
	TEST_EQ(locPart1("AOC_Day3Part1.txt"), 2640986);
}

static uint locPart2(char const* aFile)
{
	auto lines1 = GC_File::ReadAllLines(aFile);
	auto lines2 = lines1;

	uint const len = lines1[0].Count();

	while (lines1.Count() > 1)
	{
		for (uint i = 0; i < len && lines2.Count() > 1; ++i)
		{
			uint zeros = { 0 };
			uint ones = { 0 };

			// By line parsing
			for (auto line : lines1)
			{
				if (line[i] == '0')
					zeros++;
				else
					ones++;
			}

			char keep = zeros > ones ? '0' : '1';

			for (uint j = 0; j < lines1.Count(); ++j)
				if (lines1[j][i] != keep)
					lines1.RemoveAt(j--);
		}
	}

	while (lines2.Count() > 1)
	{
		for (uint i = 0; i < len && lines2.Count() > 1; ++i)
		{
			uint zeros = { 0 };
			uint ones = { 0 };

			// By line parsing
			for (auto line : lines2)
			{
				if (line[i] == '0')
					zeros++;
				else
					ones++;
			}

			char keep = zeros <= ones ? '0' : '1';

			for (uint j = 0; j < lines2.Count(); ++j)
				if (lines2[j][i] != keep)
					lines2.RemoveAt(j--);
		}
	}

	uint gamma = 0, eps = 0;
	for (uint i = 0; i < len; ++i)
	{
		gamma = (gamma << 1) + (lines1[0][i] == '1');
		eps = (eps << 1) + (lines2[0][i] == '1');
	}

	return gamma * eps;
}

DEFINE_TEST_G(Part2, Day3)
{
	TEST_EQ(locPart2("AOC_Day3Test.txt"), 230);
	TEST_EQ(locPart2("AOC_Day3Part1.txt"), 6822109);
}
