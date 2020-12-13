#include "AOC_Precompiled.h"
#include "AOC_Day13.h"

static uint locDay13Part1(char const* aFile)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_StrSlice chunk;
	GC_Strtok(text, "\n", chunk);
	int time = GC_Atoi(chunk);
	GC_Strtok(text, "\n", chunk);

	int wait = INT_MAX;
	int waitid = 0;
	GC_StrSlice bus;
	while (GC_Strtok(chunk, ",", bus))
	{
		if (bus[0] == 'x')
			continue;
		int id = GC_Atoi(bus);
		int timetowait = id - (time % id);
		if (timetowait < wait)
		{
			wait = timetowait;
			waitid = id;
		}
	}

	return wait * waitid;
}

DEFINE_TEST_G(Part1, Day13)
{
	TEST_EQ(locDay13Part1("AOC_Day13Test.txt"), 295);
	TEST_EQ(locDay13Part1("AOC_Day13Part1.txt"), 136);
}

static uint64 locDay13Part2(char const* aFile)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	GC_StrSlice chunk;
	GC_Strtok(text, "\n", chunk);
	GC_Strtok(text, "\n", chunk);

	GC_DynamicArray<GC_Pair<uint, uint>> ids;

	GC_StrSlice bus;
	uint offset = 0;

	while (GC_Strtok(chunk, ",", bus))
	{
		if (bus[0] != 'x')
		{
			ids.Add({ GC_Atou(bus), offset });
		}
		++offset;
	}

	uint64 value = 0;
	uint64 step = ids[0].myFirst;

	for (uint i = 1; i < ids.Count(); ++i)
	{
		uint const mod = ids[i].myFirst;
		uint const off = ids[i].mySecond;
		while ((value + off) % mod != 0)
			value += step;

		step *= mod;
	}

	return value;
}

DEFINE_TEST_G(Part2, Day13)
{
	TEST_EQ(locDay13Part2("AOC_Day13Test.txt"), 1068781);
	TEST_EQ(locDay13Part2("AOC_Day13Part1.txt"), 305068317272992);
}
