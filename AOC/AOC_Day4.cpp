#include "AOC_Precompiled.h"
#include "AOC_Day4.h"

using TestFn = GC_Func<bool(GC_String const& k, GC_String const& v)>;
TestFn fieldsP1[] = {
	[](GC_String const& k, GC_String const&) { return k == "byr"; },
	[](GC_String const& k, GC_String const&) { return k == "iyr"; },
	[](GC_String const& k, GC_String const&) { return k == "eyr"; },
	[](GC_String const& k, GC_String const&) { return k == "hgt"; },
	[](GC_String const& k, GC_String const&) { return k == "hcl"; },
	[](GC_String const& k, GC_String const&) { return k == "ecl"; },
	[](GC_String const& k, GC_String const&) { return k == "pid"; },
	//"cid",
};

char const* eyes[] = {
	"amb",
	"blu",
	"brn",
	"gry",
	"grn",
	"hzl",
	"oth",
};

TestFn fieldsP2[] = {
	[](GC_String const& k, GC_String const& v) { return k == "byr" && GC_InRange(GC_Atoi(v), 1920, 2002); },
	[](GC_String const& k, GC_String const& v) { return k == "iyr" && GC_InRange(GC_Atoi(v), 2010, 2020); },
	[](GC_String const& k, GC_String const& v) { return k == "eyr" && GC_InRange(GC_Atoi(v), 2020, 2030); },
	[](GC_String const& k, GC_String const& v) { return k == "hgt" && (v.EndsWith("cm") ? GC_InRange(GC_Atoi(v), 150, 193) : (v.EndsWith("in") ? GC_InRange(GC_Atoi(v), 59, 76) : false)); },
	[](GC_String const& k, GC_String const& v) { return k == "hcl" && v.Length() == 7 && v.StartsWith("#") && GC_Algorithm::All(GC_StrSlice(v.ToString() + 1, 6), GC_IsHexDigit); },
	[](GC_String const& k, GC_String const& v) { return k == "ecl" && GC_Algorithm::Contains(eyes, v); },
	[](GC_String const& k, GC_String const& v) { return k == "pid" && v.Length() == 9 && GC_Algorithm::All(v, GC_IsDigit); },
	//"cid",
};

template <uint Size>
uint numValid(char const* file, TestFn (&validFunc)[Size])
{
	uint validCount = 0;
	GC_String text;
	GC_File::ReadAllText(file, text);

	GC_String key, value;
	for (GC_StrSlice pp; GC_Strtok(text, "\n\n", pp);)
	{
		uint fieldsSet = 0;
		bool fieldsValid[GC_ARRAY_COUNT(validFunc)] = { false };
		for (GC_StrSlice kvp; GC_Strtoks(pp, " \n", kvp);)
		{
			GC_StrSlice tok;
			GC_Strtok(kvp, ":", tok); key = tok;
			GC_Strtok(kvp, ":", tok); value = tok;
			for_index(auto f : validFunc)
				if (!fieldsValid[i] && f(key, value))
				{
					fieldsValid[i] = true;
					++fieldsSet;
					break;
				}
		}

		validCount += fieldsSet == GC_ARRAY_COUNT(validFunc);
	}
	return validCount;
}

DEFINE_TEST_G(Part1, Day4)
{
	TEST_EQ(numValid("AOC_Day4_Test.txt", fieldsP1), 2);
	TEST_EQ(numValid("AOC_Day4Part1.txt", fieldsP1), 182);
}

DEFINE_TEST_G(Part2, Day4)
{
	TEST_EQ(numValid("AOC_Day4_TestGood.txt", fieldsP2), 4);
	TEST_EQ(numValid("AOC_Day4_TestBad.txt", fieldsP2), 0);
	TEST_EQ(numValid("AOC_Day4Part1.txt", fieldsP2), 109);
}
