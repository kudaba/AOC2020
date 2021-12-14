#include "AOC_Precompiled.h"

struct Rule
{
	GC_String Pattern;
	char Replacement;
	uint Left;
	uint Right;
	uint64 Count;
};

inline const auto& GC_HashKeyResolver(Rule const& aValue) { return aValue.Pattern; }

static auto locPart1(char const* aFile, uint iterations)
{
	auto text = GC_File::ReadAllText(aFile);
	auto parts = GC_StrSplit<2>(text, "\n\n");

	GC_HashSet<Rule> rules;

	// setup rules
	for (GC_StrSlice ln; GC_StrLine(parts[1], ln);)
	{
		auto rparts = GC_StrSplit<2>(ln, " -> ");
		rules.Add({ rparts[0], rparts[1][0], 0, 0, 0 });
	}

	// optimize rules
	for (auto& r : rules)
	{
		char left[3] = { r.Pattern[0], r.Replacement, 0 };
		char right[3] = { r.Replacement, r.Pattern[1], 0 };

		r.Left = rules.IndexOf(rules[left]);
		r.Right = rules.IndexOf(rules[right]);
	}

	// prime data
	for_range (parts[0].Count() - 1)
		rules.Find(GC_String(parts[0].Buffer() + i, 2))->Count++;

	// run algo
	GC_HashSet<Rule> rules2 = rules;

	for_range_v(x, iterations)
	{
		for (auto & r : rules2)
			r.Count = 0;

		for (auto r : rules)
		{
			rules2.GetByIndex(r.Left).Count += r.Count;
			rules2.GetByIndex(r.Right).Count += r.Count;
		}

		rules.Swap(rules2);
	}

	uint64 counts[26] = {};

	for (auto r : rules)
		counts[r.Pattern[1] - 'A'] += r.Count;
	counts[parts[0][0] - 'A']++;

	uint64 min = UINT64_MAX;
	for (auto c : counts)
		if (c)
			min = GC_Min(min, c);

	return GC_Algorithm::Max(counts) - min;
}

DEFINE_TEST_G(Part1, Day14)
{
	TEST_EQ(locPart1("AOC_Day14Test.txt", 10), 1588);
	TEST_EQ(locPart1("AOC_Day14Part1.txt", 10), 3697);
}

DEFINE_TEST_G(Part2, Day14)
{
	TEST_EQ(locPart1("AOC_Day14Test.txt", 40), 2188189693529);
	TEST_EQ(locPart1("AOC_Day14Part1.txt", 40), 4371307836157);
}
