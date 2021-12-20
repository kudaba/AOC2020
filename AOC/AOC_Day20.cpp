#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, int iters, bool floppy = false)
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
	GC_Algorithm::Resize(image, size, false);
	GC_DynamicArray2D<bool> temp;
	GC_Algorithm::Resize(temp, size, false);

	for_range2d(imageData.Count(), imageData[0].Count())
		image(x + hsize.x, y + hsize.y) = imageData[y][x] == '#';

	for_range(iters)
	{
		auto input = image.Area(hsize - GC_Vector2u(i), size - hsize + GC_Vector2u(i));
		auto output = temp.Area(hsize - GC_Vector2u(i+1), size - hsize + GC_Vector2u(i+1));

		for_range2d(output.Width(), output.Height())
		{
			uint value = 0;

			for_range2d_v(xx, yy, 3, 3)
			{
				GC_Vector2u const pos(x + xx - 2, y + yy - 2);
				bool const lit = (pos < input.Size()) ? input(pos) : (floppy && (i&1)); // use image 0 as the flashing indicator for the infinite
				value = (value << 1) | int(lit);
			}

			output(x, y) = converter[value] == '#';
		}

		image.Swap(temp);
	}

	return GC_Algorithm::Sum(image.AsRange(), [](bool b) {return int(b); });
}

DEFINE_TEST_G(Part1, Day20)
{
	TEST_EQ(locPart1("AOC_Day20Test.txt", 2), 35);
	TEST_EQ(locPart1("AOC_Day20Part1.txt", 2, true), 5765);
}

DEFINE_TEST_G(Part2, Day20)
{
	TEST_EQ(locPart1("AOC_Day20Test.txt", 50), 3351);
	TEST_EQ(locPart1("AOC_Day20Part1.txt", 50, true), 18509);
}
