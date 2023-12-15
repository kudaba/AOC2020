#include "AOC_Precompiled.h"

static uint locHash(GC_StrSlice str)
{
	uint hash = 0;

	for (char c : str)
	{
		hash = ((hash + c) * 17) % 256;
	}

	return hash;
}

static auto locPart1(char const* aFile)
{
	uint64 result = 0;

	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	for (GC_StrSlice chunk; GC_Strtoks(text, ",\n", chunk);)
	{
		result += locHash(chunk);
	}

	return result;
}

DEFINE_TEST_G(Part1, Day15)
{
	TEST_EQ(locPart1("AOC_Day15Test.txt"), 1320);
	TEST_EQ(locPart1("AOC_Day15Part1.txt"), 511343);
}

static auto locPart2(char const* aFile)
{
	GC_DynamicArray<GC_Pair<GC_String, uint>> Boxes[256];

	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	for (GC_StrSlice chunk; GC_Strtoks(text, ",\n", chunk);)
	{
		if (char const* sub = GC_Strchr(chunk.Buffer(), '-', 0, chunk.Count()))
		{
			GC_String left = chunk.Left(chunk.Count() - 1);
			uint hash = locHash(left.ToSlice());
			int32 index = Boxes[hash].Find(left, [](auto& i, auto& s) { return i.First == s; });
			if (index >= 0)
			{
				Boxes[hash].RemoveAt(index);
			}
		}
		else
		{
			auto parts = GC_StrSplit<2>(chunk, '=');
			uint focal = GC_Atoi(parts[1]);
			uint hash = locHash(parts[0]);
			int32 index = Boxes[hash].Find(parts[0], [](auto& i, auto& s) { return i.First == s; });
			if (index >= 0)
			{
				Boxes[hash][index].Second = focal;
			}
			else
			{
				Boxes[hash].Add({ parts[0], focal });
			}
		}
	}

	uint64 result = 0;

	for_index(auto& box : Boxes)
	{
		for_index_v(p, auto& pair : box)
		{
			result += (i + 1) * (p + 1) * pair.Second;
		}
	}

	return result;
}

DEFINE_TEST_G(Part2, Day15)
{
	TEST_EQ(locPart2("AOC_Day15Test.txt"), 145);
	TEST_EQ(locPart2("AOC_Day15Part1.txt"), 294474);
}
