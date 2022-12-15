#include "AOC_Precompiled.h"

struct Sensor
{
	GC_Vector2i pos;
	GC_Vector2i beacon;
	int size;
};

GC_DynamicArray<Sensor> locRead(char const* aFile)
{
	GC_DynamicArray<Sensor> Sensors;

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		auto parts = GC_StrSplit<5>(line, '=');
		Sensors.Add({ {GC_Atoi(parts[1]), GC_Atoi(parts[2])}, {GC_Atoi(parts[3]), GC_Atoi(parts[4])} });

		Sensor& s = Sensors.Last();
		GC_Vector2i diff = s.beacon - s.pos;
		s.size = GC_Abs(diff.x) + GC_Abs(diff.y);
	}

	return Sensors;
}

static auto locPart1(char const* aFile, int row)
{
	GC_DynamicArray<Sensor> Sensors = locRead(aFile);

	GC_DynamicArray<GC_Vector2i> cords;

	for (Sensor const& s : Sensors)
	{
		int dist = GC_Abs(row - s.pos.y);
		if (dist < s.size)
		{
			dist = s.size - dist;
			GC_HybridArray<GC_Vector2i,8> newCords;
			newCords.Add(GC_Vector2i(s.pos.x - dist, s.pos.x + dist));
			for (GC_Vector2i c : cords)
			{
				for (uint i = 0; i < newCords.Count(); ++i)
				{
					GC_Vector2i& cord = newCords[i];
					if (cord.x > c.y || cord.y < c.x)
						continue;
					if (cord.x >= c.x && cord.y <= c.y)
					{
						newCords.RemoveAtFast(i--);
						continue;
					}
					if (c.x >= cord.x && c.y <= cord.y)
					{
						newCords.Add({ cord.x, c.x });
						newCords.Add({ c.y, cord.y });
						newCords.RemoveAtFast(i--);
						continue;
					}

					if (cord.y <= c.y)
						cord.y = GC_Min(cord.y, c.x);
					if (cord.x >= c.x)
						cord.x = GC_Max(cord.x, c.y);
				}
			}

			for (GC_Vector2i cord : newCords)
				if (cord.x < cord.y)
					cords.Add(cord);
		}
	}

	uint count = 0;
	for (GC_Vector2i c : cords)
		count += c.y - c.x;
	return count;
}

DEFINE_TEST_G(Part1, Day15)
{
	TEST_EQ(locPart1("AOC_Day15Test.txt", 10), 26);
	TEST_EQ(locPart1("AOC_Day15Part1.txt", 2000000), 4724228);
}

static uint64 locPart2(char const* aFile, int rows)
{
	GC_DynamicArray<Sensor> Sensors = locRead(aFile);

	//GC_Vector2i min(INT_MAX), max(INT_MIN);
	//for (Sensor const& s : Sensors)
	//{
	//	min = GC_Min(min, s.pos - GC_Vector2i(s.size));
	//	max = GC_Max(max, s.pos + GC_Vector2i(s.size));
	//}

	//GC_DynamicArray2D<char> image(max - min + GC_Vector2i(1), '.');
	//for (Sensor const& s : Sensors)
	//{
	//	for (int x = s.pos.x - s.size; x <= s.pos.x + s.size; x++)
	//	{
	//		int ny = s.size - GC_Abs(s.pos.x - x);
	//		for (int y = s.pos.y - ny; y <= s.pos.y + ny; ++y)
	//		{
	//			image(GC_Vector2i(x, y) - min) = '#';
	//		}
	//	}
	//}

	//image(-min) = 'T';
	//image(GC_Vector2i(rows) - min) = 'B';
	//for_index(Sensor const& s : Sensors)
	//	image(s.pos - min) = char(i < 10 ? '0' + i : ('a' + i - 10));

	//for_range(image.Height())
	//{
	//	printf("%.*s\n", image.Width(), image.Row(i).Buffer());
	//}

	GC_DynamicArray<Sensor const*> candidates;
	candidates.Reserve(Sensors.Count());

	for (Sensor const& s : Sensors)
	{
		candidates.Clear();

		const int nsize = s.size + 1;

		GC_Vector2i min(INT_MAX), max(INT_MIN);

		for (Sensor const& s2 : Sensors)
		{
			if (&s != &s2)
			{
				GC_Vector2i diff = s.pos - s2.pos;
				if (GC_Abs(diff.x) + GC_Abs(diff.y) <= nsize + s2.size)
				{
					candidates.Add(&s2);

					min = GC_Min(min, s2.pos - GC_Vector2i(s2.size));
					max = GC_Max(max, s2.pos + GC_Vector2i(s2.size));
				}
			}
		}

		min = GC_Max(min, GC_Vector2i(0));
		max = GC_Min(max, GC_Vector2i(rows));

		for (int i = GC_Max(s.pos.x - nsize, min.x); i <= GC_Min(s.pos.x + nsize, max.x); i++)
		{
			const int ny = nsize - GC_Abs(s.pos.x - i);
			const GC_Vector2i b[2] = { {i, s.pos.y - ny}, {i, s.pos.y + ny} };
			for (const GC_Vector2i& nb : b)
			{
				if (GC_InRange(nb.y, min.y, max.y))
				{
					bool valid = true;
					for (Sensor const* s2 : candidates)
					{
						GC_Vector2i diff = nb - s2->pos;
						if (GC_Abs(diff.x) + GC_Abs(diff.y) <= s2->size)
						{
							valid = false;
							break;
						}
					}

					if (valid)
					{
						return (uint64)nb.x * 4000000 + (uint64)nb.y;
					}
				}
			}
		}
	}

	return 0;
}

DEFINE_TEST_G(Part2, Day15)
{
	TEST_EQ(locPart2("AOC_Day15Test.txt", 20), 56000011);
	TEST_EQ(locPart2("AOC_Day15Part1.txt", 4000000), 13622251246513);
}
