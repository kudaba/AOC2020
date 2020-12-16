#include "AOC_Precompiled.h"
#include "AOC_Day15.h"

GC_FORCEINLINE uint32 GC_GetHash(uint anItem) { return anItem; }

static uint locDay15(char const* aFile, uint const count)
{
	FINAL_ONLY(GC_HighResTimer timer);

	GC_String text;
	GC_File::ReadAllText(aFile, text);

	uint const smallSize = count / 10;

	GC_DynamicArray<uint> spokenArray;
	spokenArray.Resize(smallSize);
	GC_MemZero(spokenArray.Buffer(), spokenArray.SizeInBytes());

	GC_HashMap<uint,uint> spokenMap;
	spokenMap.Reserve(count / 5);

	uint i = 1;
	for (GC_StrSlice chunk; GC_Strtok(text, ",", chunk); i++)
	{
		spokenArray[GC_Atoi(chunk)] = i;
	}

	FINAL_ONLY(GC_TestFixture::GetCurrentTest()->Printf("Startup Time: %d\n", timer.GetElapsedMillis()));
	FINAL_ONLY(timer.Reset());

	uint last = 0;

	for (; i < count; ++i)
	{
		bool isNew;
		uint index;

		if (last < smallSize)
		{
			index = spokenArray[last];
			spokenArray[last] = i;
			isNew = index == 0;
		}
		else
		{
			auto item = spokenMap.Add(last);
			isNew = item.myFirst;
			index = *item.mySecond;
			*item.mySecond = i;
		}

		last = isNew ? 0 : (i - index);
	}

	FINAL_ONLY(GC_TestFixture::GetCurrentTest()->Printf("Runtime Time: %d\n", timer.GetElapsedMillis()));
	return last;
}

DEFINE_TEST_G(Part1, Day15)
{
	TEST_EQ(locDay15("AOC_Day15Test.txt", 2020), 436);
	TEST_EQ(locDay15("AOC_Day15Part1.txt", 2020), 257);
}

DEFINE_TEST_G(Part2, Day15)
{
	TEST_EQ(locDay15("AOC_Day15Test.txt", 30000000), 175594);
	TEST_EQ(locDay15("AOC_Day15Part1.txt", 30000000), 8546398);
}
