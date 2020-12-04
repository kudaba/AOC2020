#include "AOC_Precompiled.h"
#include "AOC_Day4.h"

uint numValid(char const* file, GC_Func<bool(uint, GC_StrSlice)> validFunc)
{
	auto lines = GC_File::ReadAllLines(file);

	uint validCount = 0;
	bool fieldsSet[GC_ARRAY_COUNT(fields)] = { false };
	fieldsSet[GC_ARRAY_COUNT(fields)-1] = true;

	for (auto line : lines)
	{
		if (line.Count() == 0)
		{
			bool valid = true;
			for (bool b : fieldsSet)
				valid &= b;
			if (valid)
				validCount++;

			GC_MemZero(fieldsSet);
			fieldsSet[GC_ARRAY_COUNT(fields)-1] = true;
		}
		else
		{
			GC_StrSlice pair;
			while (GC_Strtok(line, " ", pair))
			{
				GC_StrSlice key;
				GC_Strtok(pair, ":", key);
				GC_StrSlice value = key;
				GC_Strtok(pair, ":", value);

				for_index(char const* f : fields)
					if (GC_Strneq(key.Buffer(), f, 3) && !fieldsSet[i])
					{
						fieldsSet[i] = validFunc(i, value);
						break;
					}
			}
		}
	}

	return validCount;
}

DEFINE_TEST_G(Part1, Day4)
{
	TEST_EQ(numValid("AOC_Day4_Test.txt", [](auto, auto) {return true; }), 2);
	TEST_EQ(numValid("AOC_Day4Part1.txt", [](auto, auto) {return true; }), 182);
}

bool veryValid(uint key, GC_StrSlice valueStr)
{
	bool valid = false;
	GC_StaticString<128> value = valueStr;

	switch (key)
	{
	case 0:
		valid = GC_InRange(GC_Atoi(value), 1920, 2002);
		break;
	case 1:
		valid = GC_InRange(GC_Atoi(value), 2010, 2020);
		break;
	case 2:
		valid = GC_InRange(GC_Atoi(value), 2020, 2030);
		break;
	case 3:
		if (value.EndsWith("cm"))
			valid = GC_InRange(GC_Atoi(value), 150, 193);
		else if (value.EndsWith("in"))
			valid = GC_InRange(GC_Atoi(value), 59, 76);
		break;
	case 4:
		if (value.StartsWith("#") && valueStr.Count() == 7)
		{
			valid = true;
			for (uint j = 1; j < 7; ++j)
				valid &= GC_IsHexDigit(value[j]);
		}
		break;
	case 5:
		for (char const* eye : eyes)
			if (value == eye)
			{
				valid = true;
				break;
			}
		break;
	case 6:
		if (valueStr.Count() == 9)
		{
			valid = true;
			for (uint j = 0; j < 9; ++j)
				valid &= GC_IsDigit(value[j]);
		}
		break;
	}

	return valid;
}

DEFINE_TEST_G(Part2, Day4)
{
	TEST_EQ(numValid("AOC_Day4_TestGood.txt", veryValid), 4);
	TEST_EQ(numValid("AOC_Day4_TestBad.txt", veryValid), 0);
	TEST_EQ(numValid("AOC_Day4Part1.txt", veryValid), 109);
}
