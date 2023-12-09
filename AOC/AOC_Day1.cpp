#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		int first = -1;
		int last = -1;
		for (char c : line)
		{
			if (GC_IsDigit(c))
			{
				if (first == -1)
				{
					first = c - '0';
					last = first;
				}
				else
				{
					last = c - '0';
				}
			}
		}

		result += first * 10 + last;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day1)
{
	TEST_EQ(locPart1("AOC_Day1Test.txt"), 142);
	TEST_EQ(locPart1("AOC_Day1Part1.txt"), 54573);
}

static auto locPart2(char const* aFile)
{
	uint64 result = 0;

	GC_StaticString<32> Numbers[] =
	{
		"-----",
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine",
	};

	// By line parsing
	for (GC_String const& line : GC_File::ReadAllLines(aFile))
	{
		int first = -1;
		int last = -1;
		for_range(line.Length())
		{
			int value = -1;
			if (GC_IsDigit(line[i]))
			{
				value = line[i] - '0';
			}
			else
			{
				for_index_v(j, auto const& num : Numbers)
				{
					if (GC_Strnieq(num, line.ToString() + i, num.Length()))
					{
						value = j;
						break;
					}
				}
			}

			if (value != -1)
			{
				if (first == -1)
				{
					first = value;
					last = first;
				}
				else
 				{
					last = value;
				}
			}
		}

		result += first * 10 + last;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day1)
{
	TEST_EQ(locPart2("AOC_Day1Test2.txt"), 281);
	TEST_EQ(locPart2("AOC_Day1Part1.txt"), 54591);
}
