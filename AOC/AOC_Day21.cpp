#include "AOC_Precompiled.h"
#include "AOC_Day21.h"
#include "GC_StringBuilder.h"

static void locLoadIngredients(char const* aFile, GC_HashMap<GC_String, uint>& all_ingredients, GC_HashMap<GC_String, GC_HashSet<GC_String>>& possible_allergens)
{
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		GC_StrSlice chunk;
		GC_Strtok(line, " (contains ", chunk);

		GC_HashSet<GC_String> ingredients;
		{
			GC_StrSlice ingredient;
			while (GC_Strtok(chunk, " ", ingredient))
			{
				ingredients.Add(ingredient);
				auto iter = all_ingredients.Add(GC_String(ingredient));
				if (iter.myFirst)
					*iter.mySecond = 1;
				else
					++(*iter.mySecond);
			}
		}

		GC_Strtok(line, " (contains ", chunk);

		GC_StrSlice allergen;
		while (GC_Strtok(chunk, ", ", allergen))
		{
			if (allergen.Last() == ')')
				allergen = allergen.Left(allergen.Count() - 1);

			auto possible = possible_allergens.Add(GC_String(allergen));
			if (possible.myFirst)
			{
				*possible.mySecond = ingredients;
			}
			else
			{
				for (uint i = 0; i < possible.mySecond->Count(); ++i)
					if (!ingredients.Contains(possible.mySecond->GetByIndex(i)))
					{
						possible.mySecond->Remove(possible.mySecond->GetByIndex(i--));
					}
			}
		}
	}
}

static uint locDay21Part1(char const* aFile)
{
	GC_HashMap<GC_String, uint> all_ingredients;
	GC_HashMap<GC_String, GC_HashSet<GC_String>> possible_allergens;
	locLoadIngredients(aFile, all_ingredients, possible_allergens);

	GC_HashSet<GC_String> all_possible_allergens;

	for (auto const& possible : possible_allergens)
		for (auto const& ingregient : possible.myValue)
			all_possible_allergens.Add(ingregient);

	uint result = 0;

	for (auto const& ingredient : all_ingredients)
		if (!all_possible_allergens.Contains(ingredient.myKey))
			result += ingredient.myValue;

	return result;
}

DEFINE_TEST_G(Part1, Day21)
{
	TEST_EQ(locDay21Part1("AOC_Day21Test.txt"), 5);
	TEST_EQ(locDay21Part1("AOC_Day21Part1.txt"), 2412);
}

static GC_String locDay21Part2(char const* aFile)
{
	GC_HashMap<GC_String, uint> all_ingredients;
	GC_HashMap<GC_String, GC_HashSet<GC_String>> possible_allergens;

	locLoadIngredients(aFile, all_ingredients, possible_allergens);

	bool changed = true;
	while (changed)
	{
		changed = false;
		for (auto& possible : possible_allergens)
		{
			if (possible.myValue.Count() > 1)
			{
				for (uint i = 0; i < possible.myValue.Count(); ++i)
				{
					char const* ing = possible.myValue.GetByIndex(i);
					for (auto const& possible2 : possible_allergens)
						if (&possible2 != &possible && possible2.myValue.Count() == 1)
							if (possible2.myValue.First() == ing)
							{
								possible.myValue.Remove(ing);
								--i;
							}
				}

				if (possible.myValue.Count() > 1)
					changed = true;
			}
		}
	}

	GC_StringBuilder result;

	GC_Algorithm::Sort(possible_allergens, [](auto const& l, auto const& r) { return GC_Strcmp(l.myKey, r.myKey) < 0; });

	for (auto const& p : possible_allergens)
	{
		if (result.Length())
			result.Append(",");
		result.Append(p.myValue.First());
	}

	return result.ToString();
}

DEFINE_TEST_G(Part2, Day21)
{
	TEST_STR_EQ(locDay21Part2("AOC_Day21Test.txt"), "mxmxvkd,sqjhc,fvjkl");
	TEST_STR_EQ(locDay21Part2("AOC_Day21Part1.txt"), "mfp,mgvfmvp,nhdjth,hcdchl,dvkbjh,dcvrf,bcjz,mhnrqp");
}
