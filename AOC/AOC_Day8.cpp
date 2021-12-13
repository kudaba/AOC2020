#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto const parts = GC_StrSplit<2>(line, " | ");

		char seg[8] = { 0 };

		GC_StrSlice in;
		while (GC_Strtok(parts[0], " ", in))
			seg[in.Count()]++;

		GC_StrSlice out;
		while (GC_Strtok(parts[1], " ", out))
			if (seg[out.Count()] == 1)
				++result;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day8)
{
	TEST_EQ(locPart1("AOC_Day8Test.txt"), 26);
	TEST_EQ(locPart1("AOC_Day8Part1.txt"), 318);
}

char ToMask(GC_StrSlice num)
{
	char mask = 0;
	for (char c : num)
		mask |= 1 << (c - 'a');
	return mask;
}

static auto locPart2(char const* aFile)
{
	uint64 result = 0;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto const parts = GC_StrSplit<2>(line, " | ");

		char numbers[10] = {};
		char _235[3] = {};
		char _069[3] = {};
		char* _235w = _235;
		char* _069w = _069;

		for (GC_StrSlice in; GC_Strtok(parts[0], " ", in);)
		{
			switch (in.Count())
			{
			case 2: numbers[1] = ToMask(in); break;
			case 3: numbers[7] = ToMask(in); break;
			case 4: numbers[4] = ToMask(in); break;
			case 5: *(_235w++) = ToMask(in); break;
			case 6: *(_069w++) = ToMask(in); break;
			case 7: numbers[8] = ToMask(in); break;
			}
		}

		// 3
		for (char& c : _235)
			if ((c & numbers[1]) == numbers[1])
			{
				numbers[3] = c;
				c = 0;
				break;
			}

		// 9
		for (char& c : _069)
			if ((c & numbers[4]) == numbers[4])
			{
				numbers[9] = c;
				c = 0;
				break;
			}

		// 5
		for (char& c : _235)
			if ((c|numbers[1]) == numbers[9])
			{
				numbers[5] = c;
				c = 0;
				break;
			}

		//  2
		for (char& c : _235)
			if (c)
			{
				numbers[2] = c;
				break;
			}	

		// 6
		for (char& c : _069)
			if ((~(numbers[1] & numbers[2]) & 0x7f) == c)
			{
				numbers[6] = c;
				c = 0;
				break;
			}

		//  0
		for (char& c : _069)
			if (c)
			{
				numbers[0] = c;
				break;
			}

		uint value = 0;

		for (GC_StrSlice out; GC_Strtok(parts[1], " ", out);)
		{
			char t = ToMask(out);
			for_index(char c : numbers)
				if (c == t)
				{
					value = value * 10 + i;
					break;
				}
		}

		result += value;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day8)
{
	TEST_EQ(locPart2("AOC_Day8Test.txt"), 61229);
	TEST_EQ(locPart2("AOC_Day8Part1.txt"), 996280);
}
