#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		GC_Vector3u size(0);
		for_index(auto part : GC_Tokenize(line, 'x'))
		{
			size[i] = GC_Atoi(part);
		}

		GC_Vector3u sides(size.x* size.y, size.y* size.z, size.z* size.x);
		result += sides.x * 2 + sides.y * 2 + sides.z * 2 + GC_Min(sides.x, GC_Min(sides.y, sides.z));
	}

	return result;
}

DEFINE_TEST_G(Part1, Day2)
{
	TEST_EQ(locPart1("AOC_Day2Test.txt"), 58+43);
	TEST_EQ(locPart1("AOC_Day2Part1.txt"), 1606483);
}

static auto locPart2(char const* aFile)
{
	uint64 result = 0;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		GC_Vector3u size(0);
		for_index(auto part : GC_Tokenize(line, 'x'))
		{
			size[i] = GC_Atoi(part);
		}

		GC_Sort(size.begin(), size.end());

		result += size.x + size.x + size.y + size.y + size.x*size.y*size.z;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day2)
{
	TEST_EQ(locPart2("AOC_Day2Test.txt"), 34+14);
	TEST_EQ(locPart2("AOC_Day2Part1.txt"), 3842356);
}
