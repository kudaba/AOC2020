#include "AOC_Precompiled.h"
#include "AOC_Day16.h"

static uint locDay16Part1(char const* aFile)
{
	uint result = 0;

	GC_DynamicArray<GC_Pair<uint, uint>> allRanges;

	bool yours = false;
	bool theirs = false;
	for (auto line : GC_File::ReadAllLines(aFile, false))
	{
		if (GC_StrStartsWith(line.Buffer(), "your ticket"))
		{
			yours = true;
		}
		else if (GC_StrStartsWith(line.Buffer(), "nearby tickets"))
		{
			theirs = true;
		}
		else if (theirs)
		{
			GC_StrSlice number;
			while (GC_Strtok(line, ",", number))
			{
				uint num = GC_Atou(number);
				bool inRange = false;
				for (auto pair : allRanges)
					if (GC_InRange(num, pair.myFirst, pair.mySecond))
					{
						inRange = true;
						break;
					}

				if (!inRange)
					result += num;
			}
		}
		else if (yours)
		{
			// tbd
		}
		else
		{
			GC_StrSlice range;
			GC_Strtok(line, ": ", range);
			GC_Strtok(line, ": ", range);

			GC_StrSlice ranges;
			for_range(2)
			{
				GC_Strtok(range, " or ", ranges);

				GC_StrSlice numbers;
				GC_Strtok(ranges, "-", numbers);
				uint min = GC_Atou(numbers);
				GC_Strtok(ranges, "-", numbers);
				uint max = GC_Atou(numbers);

				allRanges.Add({ min, max });
			}
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day16)
{
	TEST_EQ(locDay16Part1("AOC_Day16Test.txt"), 71);
	TEST_EQ(locDay16Part1("AOC_Day16Part1.txt"), 27911);
}

static uint64 locDay16Part2(char const* aFile, char const* field)
{
	struct Field
	{
		GC_String name;
		GC_Vector2u ranges[2];
		bool sorted = false;
	};

	GC_DynamicArray<Field> allRanges;
	GC_DynamicArray<GC_DynamicArray<uint>> myTicket;
	GC_DynamicArray<GC_DynamicArray<uint>> allTickets;

	GC_DynamicArray<GC_DynamicArray<uint>>* ticketReader = nullptr;
	for (auto line : GC_File::ReadAllLines(aFile, false))
	{
		if (GC_StrStartsWith(line.Buffer(), "your ticket"))
		{
			ticketReader = &myTicket;
		}
		else if (GC_StrStartsWith(line.Buffer(), "nearby tickets"))
		{
			ticketReader = &allTickets;
		}
		else if (ticketReader)
		{
			GC_DynamicArray<uint>& ticket = ticketReader->Add();
			GC_StrSlice number;
			while (GC_Strtok(line, ",", number))
			{
				uint num = GC_Atou(number);
				ticket.Add(num);

				bool inRange = false;
				for (auto f : allRanges)
					if (GC_InRange(num, f.ranges[0].x, f.ranges[0].y) ||
						GC_InRange(num, f.ranges[1].x, f.ranges[1].y))
					{
						inRange = true;
						break;
					}

				if (!inRange)
				{
					ticketReader->PopBack();
					break;
				}
			}
		}
		else
		{
			GC_StrSlice range;
			GC_Strtok(line, ": ", range);
			Field& f = allRanges.Add();
			f.name = range;

			GC_Strtok(line, ": ", range);

			GC_StrSlice ranges;
			for (GC_Vector2u& r : f.ranges)
			{
				GC_Strtok(range, " or ", ranges);

				GC_StrSlice numbers;
				GC_Strtok(ranges, "-", numbers);
				r.x = GC_Atou(numbers);
				GC_Strtok(ranges, "-", numbers);
				r.y = GC_Atou(numbers);
			}
		}
	}

	// Sort fields
	// for each field, if field matches one slot then consume it
	GC_DynamicArray<Field> sortedFields = allRanges;
	bool changed = true;
	while (changed)
	{
		changed = false;
		for (Field& f : allRanges)
		{
			if (f.sorted) continue;
			changed = true;

			uint target = allRanges.Count();
			for_range(allRanges.Count())
			{
				if (sortedFields[i].sorted) continue;

				bool inRange = true;
				for (auto const& ticket : allTickets)
					if (!GC_InRange(ticket[i], f.ranges[0].x, f.ranges[0].y) &&
						!GC_InRange(ticket[i], f.ranges[1].x, f.ranges[1].y))
					{
						inRange = false;
						break;
					}

				if (inRange)
				{
					if (target == allRanges.Count())
						target = i;
					else
					{
						target = allRanges.Count();
						break;
					}
				}
			}

			if (target < allRanges.Count())
			{
				f.sorted = true;
				sortedFields[target] = f;
			}
		}
	}

	uint64 result = 1;

	for_index(Field const& f : sortedFields)
	{
		if (GC_StrStartsWith(f.name, field))
			result *= myTicket[0][i];
	}

	return result;
}

DEFINE_TEST_G(Part2, Day16)
{
	TEST_EQ(locDay16Part2("AOC_Day16Test2.txt", "class"), 12);
	TEST_EQ(locDay16Part2("AOC_Day16Part1.txt", "departure"), 737176602479);
}
