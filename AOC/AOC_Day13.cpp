#include "AOC_Precompiled.h"

struct Item
{
	using Arr = GC_DynamicArray<Item>;
	GC_Optional<Arr> Sub;
	GC_Optional<int> Value;
	bool Divider = false;

	void Parse(char const*& data)
	{
		if (GC_IsDigit(*data))
		{
			int i = *(data++) - '0';
			while (GC_IsDigit(*data))
				i = i * 10 + *(data++) - '0';
			Value = i;
		}
		else
		{
			GC_ASSERT(*data == '[');

			++data;
			Sub = Arr();
			if (*data != ']')
			{
				Sub.Get().Add().Parse(data);
				while (*data == ',')
				{
					++data;
					Sub.Get().Add().Parse(data);
				}
			}

			GC_ASSERT(*data == ']');
			++data;
		}
	}

	int Compare(Item& other)
	{
		if (Value.IsSet() != other.Value.IsSet())
		{
			if (Value.IsSet())
			{
				Sub = Arr();
				Sub.Get().Add().Value = Value;
				Value = nullopt;
			}
			else
			{
				other.Sub = Arr();
				other.Sub.Get().Add().Value = other.Value;
				other.Value = nullopt;
			}
		}

		if (Value.IsSet())
		{
			int diff = other.Value.Get() - Value.Get();
			return diff ? GC_Sign(diff) : 0;
		}
		else
		{
			for_range(GC_Min(Sub.Get().Count(), other.Sub.Get().Count()))
			{
				if (int c = Sub.Get()[i].Compare(other.Sub.Get()[i]))
					return c;
			}

			int diff = (int)other.Sub.Get().Count() - Sub.Get().Count();
			return diff ? GC_Sign(diff) : 0;
		}
	}

	bool operator<(Item& other)
	{
		return Compare(other) > 0;
	}
};

static auto locPart1(char const* aFile)
{
	GC_String data = GC_File::ReadAllText(aFile);

	uint64 result = 0;

	for_index_v(indice, auto pair : GC_StrSplit(data, "\n\n"))
	{
		Item items[2];
		{
			for_index(auto part : GC_StrSplit<2>(pair, "\n"))
			{
				char const* dataptr = part.Buffer();
				items[i].Parse(dataptr);
			}
		}

		if (items[0].Compare(items[1]) >= 0)
		{
			result += indice + 1;
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day13)
{
	TEST_EQ(locPart1("AOC_Day13Test.txt"), 13);
	TEST_EQ(locPart1("AOC_Day13Part1.txt"), 6046);
}

static auto locPart2(char const* aFile)
{
	GC_String data = GC_File::ReadAllText(aFile);

	GC_DynamicArray<Item> items;

	for (auto pair : GC_StrSplit(data, "\n\n"))
	{
		for (auto part : GC_StrSplit<2>(pair, "\n"))
		{
			char const* dataptr = part.Buffer();
			items.Add().Parse(dataptr);
		}
	}

	{
		char const* div1 = "[[2]]";
		char const* div2 = "[[6]]";

		items.Add().Parse(div1);
		items.Last().Divider = true;

		items.Add().Parse(div2);
		items.Last().Divider = true;
	}

	uint64 result = 1;

	items.Sort();

	for_index(Item const& item : items)
	{
		if (item.Divider)
			result *= i + 1;
	}

	return result;
}

DEFINE_TEST_G(Part2, Day13)
{
	TEST_EQ(locPart2("AOC_Day13Test.txt"), 140);
	TEST_EQ(locPart2("AOC_Day13Part1.txt"), 21423);
}
