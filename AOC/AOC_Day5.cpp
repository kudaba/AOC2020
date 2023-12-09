#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	GC_DynamicArray<uint64> Seeds;
	GC_DynamicArray<bool> Moved;

	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	for_index(GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{
		if (i == 0)
		{
			for (auto seed : GC_StrSplit<32>(GC_StrSplit<2>(chunk, ": ")[1], ' '))
				Seeds.Add(GC_Atoi(seed));

			Moved.Resize(Seeds.Count());
		}
		else
		{
			GC_MemZero(Moved.Buffer(), Moved.SizeInBytes());

			auto parts = GC_StrSplit<32>(chunk, "\n");

			for_range_v(j, parts.Count() - 1)
			{
				auto rangeData = GC_StrSplit<3>(parts[j + 1], ' ');

				const uint64 dest = GC_Atoi64(rangeData[0]);
				const uint64 source = GC_Atoi64(rangeData[1]);
				const uint64 length = GC_Atoi64(rangeData[2]);

				for_range_v(s, Seeds.Count())
				{
					if (!Moved[s])
					{
						if (GC_InRange(Seeds[s], source, source + length))
						{
							Seeds[s] = Seeds[s] - source + dest;
							Moved[s] = true;
						}
					}
				}
			}
		}
	}

	return GC_Algorithm::Min(Seeds);
}

DEFINE_TEST_G(Part1, Day5)
{
	TEST_EQ(locPart1("AOC_Day5Test.txt"), 35);
	TEST_EQ(locPart1("AOC_Day5Part1.txt"), 389056265);
}

static auto locPart2(char const* aFile)
{
	using SeedRange = GC_Vector2<s64>;
	GC_DynamicArray<SeedRange> Seeds;
	GC_BitVector Moved;

	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);
	for_index(GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{
		if (i == 0)
		{
			for_index_v(s, auto seed : GC_StrSplit<32>(GC_StrSplit<2>(chunk, ": ")[1], ' '))
			{
				if (s & 1)
					Seeds.Last().y = Seeds.Last().x + GC_Atoi64(seed);
				else
					Seeds.Add({ GC_Atoi64(seed), 0 });
			}

			Moved.Resize(Seeds.Count());
		}
		else
		{
			GC_MemZero(Moved.GetBuffer(), Moved.SizeInBytes());

			auto parts = GC_StrSplit<32>(chunk, "\n");

			for_range_v(j, parts.Count() - 1)
			{
				auto rangeData = GC_StrSplit<3>(parts[j + 1], ' ');

				const s64 dest = GC_Atoi64(rangeData[0]);
				const s64 source = GC_Atoi64(rangeData[1]);
				const s64 sourceend = source + GC_Atoi64(rangeData[2]);

				for_range_v(s, Seeds.Count())
				{
					SeedRange& seed = Seeds[s];
					if (!Moved[s])
					{
						// test intersection
						if (seed.y >= source && seed.x < sourceend)
						{
							Moved[s] = true;

							SeedRange Left(-1), Right(-1);
							if (seed.x < source)
							{
								Left.x = seed.x;
								Left.y = source - 1;
								seed.x = source;
							}
							if (seed.y >= sourceend)
							{
								Right.x = sourceend;
								Right.y = seed.y;
								seed.y = sourceend - 1;
							}

							seed.x = seed.x - source + dest;
							seed.y = seed.y - source + dest;

							if (Left.x >= 0)
							{
								Seeds.Add(Left);
								Moved.Resize(Moved.Count() + 1, false);
							}
							if (Right.x >= 0)
							{
								Seeds.Add(Right);
								Moved.Resize(Moved.Count() + 1, false);
							}
						}
					}
				}
			}
		}
	}

	return GC_Algorithm::Min(Seeds, [](auto const& s) { return s.x; });
}

DEFINE_TEST_G(Part2, Day5)
{
	TEST_EQ(locPart2("AOC_Day5Test.txt"), 46);
	TEST_EQ(locPart2("AOC_Day5Part1.txt"), 137516820);
}
