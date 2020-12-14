#include "AOC_Precompiled.h"
#include "AOC_Day14.h"

static uint64 locDay14Part1(char const* aFile)
{
	uint64 _and = 0;
	uint64 _or = 0;

	GC_HashMap<uint64, uint64> values;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		if (GC_StrStartsWith(line.Buffer(), "mask = "))
		{
			_and = 0;
			_or = 0;
			GC_StrSlice bits = line.Range(GC_STRING_LENGTH("mask = "));
			for_index(char c : bits)
			{
				if (c == '0')
				{
					_and |= (u64)1 << (36 - (i+1));
				}
				else if (c == '1')
				{
					_or |= (u64)1 << (36 - (i+1));
				}
			}

			_and = ~_and;
		}
		else
		{
			uint64 loc;
			uint64 val;
			GC_VERIFY_OP(sscanf(line.Buffer(), "mem[%lld] = %lld", &loc, &val), == 2);

			values.GetOrAdd(loc) = (val & _and) | _or;
		}
	}

	uint64 total = 0;
	for (auto pair : values)
		total += pair.myValue;

	return total;
}

DEFINE_TEST_G(Part1, Day14)
{
	TEST_EQ(locDay14Part1("AOC_Day14Test.txt"), 165);
	TEST_EQ(locDay14Part1("AOC_Day14Part1.txt"), 12610010960049);
}

static uint64 locDay14Part2(char const* aFile)
{
	uint64 _and = 0;
	uint64 _or = 0;

	GC_HashMap<uint64, uint64> values;
	values.Reserve(100000);
	GC_DynamicArray<uint64> ranges;
	ranges.Reserve(1000);

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		if (GC_StrStartsWith(line.Buffer(), "mask = "))
		{
			ranges.Clear();
			ranges.Add(0);
			_and = 0;
			_or = 0;
			GC_StrSlice bits = line.Range(GC_STRING_LENGTH("mask = "));
			for_index(char c : bits)
			{
				if (c == '1')
				{
					_or |= (u64)1 << (36 - (i+1));
				}
				else if (c == 'X')
				{
					uint64 bit = (u64)1 << (36 - (i + 1));
					_and |= (u64)1 << (36 - (i+1));
					for_range_v(r, ranges.Count())
					{
						ranges.Add(ranges[r] | bit);
					}
				}
			}

			_and = ~_and;
		}
		else
		{
			uint64 loc;
			uint64 val;
			GC_VERIFY_OP(sscanf(line.Buffer(), "mem[%lld] = %lld", &loc, &val), == 2);

			loc = (loc & _and) | _or;
			for (uint64 l : ranges)
				values.GetOrAdd(loc | l) = val;
		}
	}

	uint64 total = 0;
	for (auto pair : values)
		total += pair.myValue;

	return total;
}

DEFINE_TEST_G(Part2, Day14)
{
	TEST_EQ(locDay14Part2("AOC_Day14Test2.txt"), 208);
	TEST_EQ(locDay14Part2("AOC_Day14Part1.txt"), 3608464522781);
}
