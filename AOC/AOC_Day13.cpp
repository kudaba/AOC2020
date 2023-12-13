#include "AOC_Precompiled.h"

auto locPart1(char const* aFile, bool bFix = false)
{
	uint64 result = 0;

	GC_String text;
	GC_File::ReadAllText(aFile, text);
	for (GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{
		GC_DynamicArray2D<char> map = GC_File::Parse2d<char>(GC_StrSplit<32>(chunk, '\n'), [](char c) { return c; });

		GC_DynamicArray<uint> rows;
		GC_DynamicArray<uint> columns;

		for_range(map.Width())
		{
			uint& val = columns.Add();
			val = 0;
			for_index_v(b, char c : map.Column(i))
			{
				val |= (c == '#') << b;
			}
		}
		for_range(map.Height())
		{
			uint& val = rows.Add();
			val = 0;
			for_index_v(b, char c : map.Row(i))
			{
				val |= (c == '#') << b;
			}
		}

		GC_DynamicArray<uint>* dataSet[] = { &columns, &rows };
		const uint mults[] = { 1, 100 };

		GC_Vector2u fixup(0);
		for_index_v(m, const GC_DynamicArray<uint>*dataPtr : dataSet)
		{
			const GC_DynamicArray<uint>& data = *dataPtr;
			uint l = data.Count() - 1;

			for_range(l)
			{
				bool xored = bFix && GC_GetNumBitsSet(data[i] ^ data[i + 1]) == 1;
				if (xored || data[i] == data[i + 1])
				{
					uint s = i, e = i + 1;
					while (s > 0 && e < l && data[s - 1] == data[e + 1])
					{
						--s; ++e;
					}

					if (bFix && !xored)
					{
						if (s != 0 && e != l)
						{
							if (GC_GetNumBitsSet(data[s - 1] ^ data[e + 1]) == 1)
							{
								--s;
								++e;
								while (s > 0 && e < l && data[s - 1] == data[e + 1])
								{
									--s; ++e;
								}

								if (s == 0 || e == l)
								{
									result += (i + 1) * mults[m];
								}
							}
						}
					}
					else if (s == 0 || e == l)
					{
						result += (i + 1) * mults[m];
					}
				}
			}
		}
	}

	return result;
}

DEFINE_TEST_G(Part1, Day13)
{
	TEST_EQ(locPart1("AOC_Day13Test.txt"), 405);
	TEST_EQ(locPart1("AOC_Day13Part1.txt"), 34911);
}

DEFINE_TEST_G(Part2, Day13)
{
	TEST_EQ(locPart1("AOC_Day13Test.txt", true), 400);
	TEST_EQ(locPart1("AOC_Day13Part1.txt", true), 33183);
}
