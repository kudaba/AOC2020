#include "AOC_Precompiled.h"
#include "AOC_Day19.h"

struct Rule
{
	GC_HybridArray<GC_HybridArray<uint, 3>, 1> subRules;
	char match;

	void Match(GC_StrSlice line, GC_ArrayRange<Rule const> rules, GC_HybridArray<uint, 32>& usedOut) const
	{
		if (subRules.Count())
		{
			GC_HybridArray<uint, 32> used;
			GC_HybridArray<uint, 32> nextUsed;
			GC_HybridArray<uint, 32> subUsed;

			for (auto& sub : subRules)
			{
				used.Resize(1);
				used[0] = 0;

				for (uint rule : sub)
				{
					nextUsed.Clear();
					for (uint u : used)
					{
						if (u < line.Count())
						{
							subUsed.Clear();
							rules[rule].Match(line.Range(u), rules, subUsed);
							for (uint su : subUsed)
								nextUsed.Add(u + su);
						}
					}
					used.Swap(nextUsed);
				}

				usedOut.AddRange(used);
			}
		}
		else if (line.Count() && line[0] == match)
		{
			usedOut.Add(1);
		}
	}
};

static uint locDay19Part1(char const* aFile, GC_Function<void(GC_DynamicArray<Rule>&)> fixup = nullptr)
{
	uint result = 0;

	bool readingRules = true;
	GC_DynamicArray<Rule> rules;
	GC_HybridArray<uint, 32> used;
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		if (line.Count() == 0)
		{
			readingRules = false;
			if (fixup)
				fixup(rules);
		}
		else if (readingRules)
		{
			GC_StrSlice part;
			GC_Strtok(line, ": ", part);
			uint index = GC_Atoi(part);
			GC_Strtok(line, ": ", part);

			if (rules.Count() <= index)
				rules.Resize(index + 1);
			Rule& rule = rules[index];

			if (part[0] == '"')
				rule.match = part[1];
			else
			{
				GC_StrSlice subRule;
				while (GC_Strtok(part, " | ", subRule))
				{
					auto& subRuleIndices = rule.subRules.Add();
					GC_StrSlice subIndex;
					while (GC_Strtok(subRule, " ", subIndex))
						subRuleIndices.Add() = GC_Atoi(subIndex);
				}
			}
		}
		else
		{
			used.Clear();
			rules[0].Match(line, rules, used);
			if (used.Contains(line.Count()))
				++result;
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day19)
{
	TEST_EQ(locDay19Part1("AOC_Day19Test.txt"), 2);
	TEST_EQ(locDay19Part1("AOC_Day19Part1.txt"), 222);
}

static uint locDay19Part2(char const* aFile)
{
	return locDay19Part1(aFile, [](GC_DynamicArray<Rule>& rules)
		{
			rules[8].subRules.Resize(2);
			rules[8].subRules[0].Resize(1);
			rules[8].subRules[0][0] = 42;
			rules[8].subRules[1].Resize(2);
			rules[8].subRules[1][0] = 42;
			rules[8].subRules[1][1] = 8;

			rules[11].subRules.Resize(2);
			rules[11].subRules[0].Resize(2);
			rules[11].subRules[0][0] = 42;
			rules[11].subRules[0][1] = 31;
			rules[11].subRules[1].Resize(3);
			rules[11].subRules[1][0] = 42;
			rules[11].subRules[1][1] = 11;
			rules[11].subRules[1][2] = 31;
		});
}

DEFINE_TEST_G(Part2, Day19)
{
#if RUN_TESTS
	TEST_EQ(locDay19Part2("AOC_Day19Test2.txt"), 12);
#endif
	TEST_EQ(locDay19Part2("AOC_Day19Part1.txt"), 339);
}
