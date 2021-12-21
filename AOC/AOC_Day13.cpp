#include "AOC_Precompiled.h"
#include "GC_StringBuilder.h"

static auto locPart1(char const* aFile, bool isPart1)
{
	GC_DynamicArray2D<bool> data;

	// By Block parsing (block of lines separate by two new lines)
	GC_String const text = GC_File::ReadAllText(aFile);
	auto const chunks = GC_StrSplit<2>(text, "\n\n");

	for (GC_StrSlice p; GC_StrLine(chunks[0], p);)
	{
		GC_Vector2u pos;

		for_index(GC_StrSlice n; GC_Strtok(p, ",", n);)
			pos[i] = GC_Atoi(n);

		GC_Algorithm::Resize(data, GC_Max(pos + GC_Vector2u(1), data.Size()), false);
		data(pos) = true;
	}

	for (GC_StrSlice ln; GC_StrLine(chunks[1], ln);)
	{
		auto const fold = GC_StrSplit<2>(ln, '=');
		bool const isX = fold[0].Last() == 'x';
		uint const num = GC_Atoi(fold[1]);

		if (isX)
		{
			for (uint i = num - 1, j = num + 1; i < data.Width() && j < data.Width(); --i, ++j)
			{
				for_range_v(y, data.Height())
					data(GC_Vector2u(i, y)) |= data(GC_Vector2u(j, y));
			}

			data.SetSize(GC_Vector2u(num, data.Height()));
		}
		else
		{
			for (uint i = num - 1, j = num + 1; i < data.Height() && j < data.Height(); --i, ++j)
			{
				for_range_v(x, data.Width())
					data(GC_Vector2u(x, i)) |= data(GC_Vector2u(x, j));
			}

			data.SetSize(GC_Vector2u(data.Width(), num));
		}

		if (isPart1)
		{
			uint64 result = 0;

			for_range2d(data.Width(), data.Height())
				if (data(x, y))
					++result;

			return result;
		}
	}

	GC_StringBuilderLarge output;
	for_range_v(y, data.Height())
	{
		for_range_v(x, data.Width())
			output.Append(data(x, y) ? "#" : " ");
		output.Append("\n");
	}

	printf(output.ToString());

	return (u64)0;
}

DEFINE_TEST_G(Part1, Day13)
{
	TEST_EQ(locPart1("AOC_Day13Test.txt", true), 17);
	TEST_EQ(locPart1("AOC_Day13Part1.txt", true), 693);
}

DEFINE_TEST_G(Part2, Day13)
{
	TEST_EQ(locPart1("AOC_Day13Test.txt", false), 0);
	TEST_EQ(locPart1("AOC_Day13Part1.txt", false), 0);
}
