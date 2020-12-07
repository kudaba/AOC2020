#include "AOC_Precompiled.h"
#include "AOC_Day7.h"

static uint lodDay7Part1(char const* aFile)
{
	uint result = 0;
	GC_HashMap<GC_String, GC_HashSet<GC_String>> owningBagMap;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		GC_StrSlice outerBagTok;
		GC_Strtok(line, " bags contain ", outerBagTok);

		GC_StrSlice outerBag = outerBagTok;
		GC_Strtok(line, " bags contain ", outerBagTok);

		if (GC_String(outerBagTok) != "no other bags")
		{
			GC_StrSlice bagPart;
			while (GC_Strtok(outerBagTok, ",", bagPart))
			{
				GC_StrSlice p1;
				GC_StrSlice p2;

				GC_StrSlice part;
				while (GC_Strtok(bagPart, " ", part))
				{
					if (GC_String(part).StartsWith("bag"))
					{
						GC_String bag;
						bag.Format("{} {}", p1, p2);
						owningBagMap.GetOrAdd(bag).Add(outerBag);
					}
					else
					{
						p1 = p2;
						p2 = part;
					}
				}
			}
		}
	}

	GC_HashSet<GC_String> fullSet;
	for (char const* bag : owningBagMap["shiny gold"])
		fullSet.Add(bag);

	for (; result < fullSet.Count(); ++result)
	{
		if (GC_HashSet<GC_String> const* set = owningBagMap.Find(fullSet.GetByIndex(result)))
		{
			for (char const* bag : *set)
				fullSet.Add(bag);
		}
	}

	return fullSet.Count();
}

DEFINE_TEST_G(Part1, Day7)
{
	TEST_EQ(lodDay7Part1("AOC_Day7Test.txt"), 4);
	TEST_EQ(lodDay7Part1("AOC_Day7Part1.txt"), 172);
}

static uint locCountBags(char const* bag, GC_HashMap<GC_String, GC_HashMap<GC_String, uint>> const& owningBagMap)
{
	uint total = 1;
	if (auto const* set = owningBagMap.Find(bag))
	{
		for (auto const& subBag : *set)
		{
			total += subBag.myValue * locCountBags(subBag.myKey, owningBagMap);
		}
	}
	return total;
}

static uint locDay7Part2(char const* aFile)
{
	GC_HashMap<GC_String, GC_HashMap<GC_String, uint>> owningBagMap;

	for (auto line : GC_File::ReadAllLines(aFile))
	{
		GC_StrSlice outerBagTok;
		GC_Strtok(line, " bags contain ", outerBagTok);

		GC_StrSlice outerBag = outerBagTok;
		GC_Strtok(line, " bags contain ", outerBagTok);

		if (GC_String(outerBagTok) != "no other bags")
		{
			GC_StrSlice bagPart;
			while (GC_Strtok(outerBagTok, ",", bagPart))
			{
				GC_StrSlice n;
				GC_StrSlice p1;
				GC_StrSlice p2;

				GC_StrSlice part;
				while (GC_Strtok(bagPart, " ", part))
				{
					if (GC_String(part).StartsWith("bag"))
					{
						GC_String bag;
						bag.Format("{} {}", p1, p2);
						owningBagMap.GetOrAdd(outerBag).Add(bag, GC_Atou(n));
					}
					else
					{
						n = p1;
						p1 = p2;
						p2 = part;
					}
				}
			}
		}
	}

	return locCountBags("shiny gold", owningBagMap) - 1;
}

DEFINE_TEST_G(Part2, Day7)
{
	TEST_EQ(locDay7Part2("AOC_Day7Test2.txt"), 126);
	TEST_EQ(locDay7Part2("AOC_Day7Part1.txt"), 39645);
}
