#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, int iters)
{

	GC_String text = GC_File::ReadAllText(aFile);
	auto parts = GC_StrSplit<2>(text, "\n\n");

	auto converter = parts[0];

	auto imageData = GC_StrSplit(parts[1], "\n");

	GC_Vector2u const isize = GC_Vector2u(imageData.Count(), imageData[0].Count());
	GC_Vector2u const extra = GC_Vector2u(iters * 2 + 4);
	GC_Vector2u const size = isize + extra;
	GC_Vector2u const hsize = extra / 2;

	GC_DynamicArray2D<bool> image;
	image.SetSize(size);
	GC_MemZero(image.Buffer(), image.SizeInBytes());

	for_range2d(imageData.Count(), imageData[0].Count())
		image(x + hsize.x, y + hsize.y) = imageData[y][x] == '#';

	GC_DynamicArray2D<bool> output;
	output.SetSize(size);

	for_range(iters)
	{
		for_range2d(size.x, size.y)
		{
			uint value = 0;

			for_range2d_v(xx, yy, 3, 3)
			{
				GC_Vector2i const pos(x + xx - 1, y + yy - 1);
				bool const lit = (pos >= GC_Vector2i(0) && pos < (GC_Vector2i)size) ? image(pos) : image(0,0); // use image 0 as the flashing indicator for the infinite
				value = (value << 1) | int(lit);
			}

			output(x, y) = converter[value] == '#';
		}

		image.Swap(output);
	}

	return GC_Algorithm::Sum(image.AsRange(), [](bool b) {return int(b); });
}

DEFINE_TEST_G(Part1, Day20)
{
	TEST_EQ(locPart1("AOC_Day20Test.txt", 2), 35);
	TEST_EQ(locPart1("AOC_Day20Part1.txt", 2), 5765);
}

DEFINE_TEST_G(Part2, Day20)
{
	TEST_EQ(locPart1("AOC_Day20Test.txt", 50), 3351);
	TEST_EQ(locPart1("AOC_Day20Part1.txt", 50), 18509);
}
