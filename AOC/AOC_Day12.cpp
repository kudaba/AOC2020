#include "AOC_Precompiled.h"

struct Matcher
{
	GC_StaticBitVector128 anyBits;
	GC_StaticBitVector128 damagedBits;
	GC_StaticBitVector128 unknownBits;

	GC_HashMap<GC_Pair<uint, uint>, uint64> CachedResults;
};

static uint64 GetMatchCount(Matcher& aMatcher, uint extraSpace, GC_ArrayRange<uint> runs, const GC_StaticBitVector128& prevMatch = {}, uint index = 0)
{
	uint64 matches = 0;
	const uint run = runs[0];

	const GC_ArrayRange<uint> nextRuns = runs.Range(1);

	GC_StaticBitVector128 nextMatch;
	for_range(run)
	{
		nextMatch.SetValue(i + index, true);
	}

	for_range_v(s, extraSpace + 1)
	{
		if ((nextMatch & aMatcher.anyBits) == nextMatch)
		{
			const uint nextIndex = index + run + s + 1;
			const GC_Pair<uint, uint> key = { nextRuns.Count(), nextIndex };

			uint64 const* cache = aMatcher.CachedResults.Find(key);
			if (!cache || *cache)
			{
				GC_StaticBitVector128 testMatch = prevMatch | nextMatch;
				for (uint i = nextIndex; i < 128; ++i)
					testMatch.SetValue(i, aMatcher.damagedBits.GetValue(i));

				if ((aMatcher.damagedBits & testMatch) == aMatcher.damagedBits)
				{
					uint64 cacheMiss = 1;
					if (!cache)
					{
						// Cache assuming valid up to this point
						if (nextRuns.Count())
						{
							GC_StaticBitVector128 tmp;
							for_range(nextIndex)
								tmp.SetValue(i, aMatcher.damagedBits.GetValue(i));

							cacheMiss = GetMatchCount(aMatcher, extraSpace - s, nextRuns, tmp, nextIndex);
						}
						else
						{
							for (uint i = nextIndex; i < 128; ++i)
							{
								if (aMatcher.damagedBits.GetValue(i))
								{
									cacheMiss = 0;
									break;
								}
							}
						}

						cache = &cacheMiss;
						aMatcher.CachedResults.Add(key, cacheMiss);
					}

					matches += *cache;
				}
			}
		}

		nextMatch.SetValue(s + index, false);
		nextMatch.SetValue(s + index + run, true);
	}

	return matches;
}

Matcher matcher;

uint64 locGetMatchCount2(GC_StrSlice aLine, uint aCopies = 1)
{
	matcher.anyBits = matcher.damagedBits = matcher.unknownBits = {};
	matcher.CachedResults.Clear();

	GC_DynamicArray<uint> runs;
	GC_DynamicArray<char> springs;

	uint minSize = 0;

	auto parts = GC_StrSplit<2>(aLine, ' ');
	for_range(aCopies)
	{
		if (i)
			springs.Add('?');
		springs += parts[0];

		for (auto run : GC_StrSplit<32>(parts[1], ','))
		{
			runs.Add(GC_Atoi(run));
			minSize += runs.Last();
		}
	}

	minSize += runs.Count() - 1;

	for_index(char c : springs)
	{
		if (c == '?')
		{
			matcher.unknownBits.SetValue(i, true);
		}
		else if (c == '#')
		{
			matcher.damagedBits.SetValue(i, true);
		}
	}

	matcher.anyBits = matcher.damagedBits | matcher.unknownBits;

	uint extraSpace = springs.Count() - minSize;

	return GetMatchCount(matcher, extraSpace, runs);
}

static auto locPart1(char const* aFile, uint aCopies = 1)
{
	uint64 result = 0;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		result += locGetMatchCount2(line, aCopies);
	}

	return result;
}

DEFINE_TEST_G(Part1, Day12)
{
	TEST_EQ(locPart1("AOC_Day12Test.txt"), 21);
	TEST_EQ(locPart1("AOC_Day12Part1.txt"), 6852);
	matcher.CachedResults.Reset();
}

DEFINE_TEST_G(Part2, Day12)
{
	TEST_EQ(locPart1("AOC_Day12Test.txt", 5), 525152);
	TEST_EQ(locPart1("AOC_Day12Part1.txt", 5), 8475948826693);
	matcher.CachedResults.Reset();
}
