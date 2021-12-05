#include "AOC_Precompiled.h"

GC_Vector2i readV(GC_StrSlice in)
{
	GC_Vector2i out;

	GC_StrSlice part;
	GC_Strtok(in, ",", part);
	out.x = GC_Atoi(part);
	GC_Strtok(in, ",", part);
	out.y = GC_Atoi(part);
	return out;
}

static GC_DynamicArray<GC_Pair<GC_Vector2i, GC_Vector2i>> locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<GC_Pair<GC_Vector2i, GC_Vector2i>>(aFile, [](auto aLine) 
		{
			GC_StrSlice part;
			GC_Strtok(aLine, " -> ", part);
			auto s = readV(part);
			GC_Strtok(aLine, " -> ", part);
			return GC_Pair<GC_Vector2i, GC_Vector2i> { s, readV(part) };
		});
}

static uint locPart1(char const* aFile)
{
	uint result = 0;

	GC_DynamicArray2D<int> arr;

	auto data = locParseData(aFile);
	GC_Vector2i size(0);
	for (auto item : data)
	{
		auto rr = GC_Max(item.myFirst, item.mySecond) + GC_Vector2i(1);
		size.Set(GC_Max<int>(size.x, rr.x), GC_Max<int>(size.y, rr.y));
	}

	arr.SetSize(size);
	GC_MemZero(arr.Buffer(), arr.SizeInBytes());

	for (auto item : data)
	{
		if (item.myFirst.x == item.mySecond.x || item.myFirst.y == item.mySecond.y)
		{
			int const index = item.myFirst.x == item.mySecond.x;
			int const dir = item.myFirst[index] > item.mySecond[index] ? -1 : 1;

			for (GC_Vector2i i = item.myFirst; i != item.mySecond; i[index] += dir)
			{
				arr(i)++;
			}
			arr(item.mySecond)++;
		}
	}

	for (int i : arr)
		if (i >= 2)
			result++;

	return result;
}

DEFINE_TEST_G(Part1, Day5)
{
	TEST_EQ(locPart1("AOC_Day5Test.txt"), 5);
	TEST_EQ(locPart1("AOC_Day5Part1.txt"), 4873);
}

static uint locPart2(char const* aFile)
{
	uint result = 0;

	GC_DynamicArray2D<int> arr;

	auto data = locParseData(aFile);
	GC_Vector2i size(0);
	for (auto item : data)
	{
		auto rr = GC_Max(item.myFirst, item.mySecond) + GC_Vector2i(1);
		size.Set(GC_Max<int>(size.x, rr.x), GC_Max<int>(size.y, rr.y));
	}

	arr.SetSize(size);
	GC_MemZero(arr.Buffer(), arr.SizeInBytes());

	for (auto item : data)
	{
		GC_Vector2i dir = item.mySecond - item.myFirst;
		dir = GC_SafeDivide(dir, GC_Abs(dir));
		for (GC_Vector2i i = item.myFirst; i != item.mySecond; i += dir)
		{
			arr(i)++;
		}
		arr(item.mySecond)++;
	}

	for (int i : arr)
		if (i >= 2)
			result++;

	return result;
}

DEFINE_TEST_G(Part2, Day5)
{
	TEST_EQ(locPart2("AOC_Day5Test.txt"), 12);
	TEST_EQ(locPart2("AOC_Day5Part1.txt"), 19472);
}
