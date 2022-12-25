#include "AOC_Precompiled.h"

int64 locParse(GC_StrSlice line)
{
	int64 val = 0;
	int64 mult = 1;
	for (int i = line.Count() - 1; i >= 0; --i)
	{
		int64 v = 0;
		switch (line[i])
		{
		case '0':
		case '1':
		case '2':
			v = line[i] - '0';
			break;
		case '-':
			v = -1; break;
		case '=':
			v = -2; break;
		default:
			GC_UNREACHABLE();

		}
		val += v * mult;
		mult *= 5;
	}
	return val;
}

static auto locPart1(char const* aFile)
{
	int64 result = 0;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		result += locParse(line);
	}

	int64 r = result;

	// 1, 5, 25, 125, 625, 3125

	char resultString[64] = {};
	uint ri = 62;
	while (result)
	{
		int remainder = 0;
		char v = result % 5;
		switch (v)
		{
		case 0:
		case 1:
		case 2:
			resultString[ri] = '0' + v;
			break;
		case 3:
			resultString[ri] = '=';
			++remainder;
			break;
		case 4:
			resultString[ri] = '-';
			++remainder;
			break;
		}
		--ri;
		result /= 5;
		result += remainder;
	}


	GC_ASSERT(locParse(GC_StrSlice(resultString + ri + 1, 64 - ri - 2)) == r);

	return GC_String(resultString + ri + 1);
}

DEFINE_TEST_G(Part1, Day25)
{
	TEST_STR_EQ(locPart1("AOC_Day25Test.txt"), "2=-1=0");
	TEST_STR_EQ(locPart1("AOC_Day25Part1.txt"), "20===-20-020=0001-02");
}
