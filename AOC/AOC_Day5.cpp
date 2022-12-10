#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	int const vowel =
		1 << ('a' - 'a') |
		1 << ('e' - 'a') |
		1 << ('i' - 'a') |
		1 << ('o' - 'a') |
		1 << ('u' - 'a');

	uint64 result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		char vowels = 0;
		bool doubles = false;
		char last = 0;
		for (char c : line)
		{
			if ((1 << (c - 'a')) & vowel)
				vowels++;
			if (c == last)
				doubles = true;

			if ((last == 'a' && c == 'b') ||
				(last == 'c' && c == 'd') ||
				(last == 'p' && c == 'q') ||
				(last == 'x' && c == 'y'))
			{
				doubles = false;
				break;
			}
			last = c;
		}

		if (vowels >= 3 && doubles)
			++result;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day5)
{
	TEST_EQ(locPart1("AOC_Day5Test.txt"), 2);
	TEST_EQ(locPart1("AOC_Day5Part1.txt"), 255);
}

static auto locPart2(char const* aFile)
{
	uint64 result = 0;
	GC_HashMap<uint16, uint> pairs;
	pairs.Reserve(100);

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		pairs.Clear();

		bool pairRepeat = false;
		bool letterRepeat = false;
		char last1 = 0;
		char last2 = 0;
		for_index(char c : line)
		{
			if (!pairRepeat)
			{
				uint16 pair = (last1 << 8) | c;

				if (uint const* index = pairs.Find(pair))
				{
					if (i - *index > 1)
					{
						pairRepeat = true;
						if (letterRepeat)
							break;
					}
				}
				else
				{
					pairs.Add(pair, i);
				}
			}

			if (c == last2)
			{
				letterRepeat = true;
				if (pairRepeat)
					break;
			}

			last2 = last1;
			last1 = c;
		}

		if (pairRepeat && letterRepeat)
			++result;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day5)
{
	TEST_EQ(locPart2("AOC_Day5Test2.txt"), 2);
	TEST_EQ(locPart2("AOC_Day5Part1.txt"), 55);
}
