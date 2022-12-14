#include "AOC_Precompiled.h"
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max

static auto locPart1(char const* aFile, bool shouldAddFloor, bool shouldVisualize = false)
{
	const char space = '.';

	uint64 result = 0;

	GC_DynamicArray2D<char> map(1000, 1000, space);

	GC_Vector2u min(map.Width());
	GC_Vector2u max(0);

	for (char& c : map.Column(999))
		c = '\n';

	// By line parsing
	for (auto line : GC_File::ReadAllLines(aFile))
	{
		GC_Vector2u pos = map.Size();
		for (auto rock : GC_TokenizeString(line, " -> "))
		{
			auto coord = GC_StrSplit<2>(rock, ',');
			GC_Vector2u next(GC_Atoi(coord[0]), GC_Atoi(coord[1]));

			if (pos != map.Size())
			{
				uint const off = pos.x == next.x;
				GC_Vector2u p = pos;
				for (uint i = GC_Min(pos[off], next[off]), e = GC_Max(pos[off], next[off]); i <= e; ++i)
				{
					p[off] = i;
					map(p) = '#';
				}
			}

			pos = next;
			min = GC_Min(pos, min);
			max = GC_Max(pos, max);
		}
	}

	min.y = 0;
	max.y += 2;
	min.x = 500 - max.y;
	max.x = 500 + max.y + 1;

	if (shouldAddFloor)
	{
		for_range(map.Width())
		{
			map(i, max.y) = '#';
		}
	}

	auto visualArea = map.Area(min, max + GC_Vector2u::Up);
	if (shouldVisualize)
	{
		system("cls");
		for_range(visualArea.Height())
		{
			printf("%.*s\n", visualArea.Width(), visualArea.Row(i).Buffer());
		}
	}

	while (1)
	{
		GC_Vector2u start(500 - min.x, 0);

		if (visualArea(start) != space)
			break;

		while (1)
		{
			if (shouldVisualize)
			{
				Sleep(1);
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (short)start.x, (short)start.y });
				printf("+");
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (short)start.x, (short)start.y });
			}

			if (visualArea(start + GC_Vector2u::Up) == space)
			{
				start.y += 1;
			}
			else if (visualArea(start + GC_Vector2u::Up + GC_Vector2u::Left) == space)
			{
				start.y += 1;
				start.x -= 1;
			}
			else if (visualArea(start + GC_Vector2u::Up + GC_Vector2u::Right) == space)
			{
				start.y += 1;
				start.x += 1;
			}
			else
			{
				if (shouldVisualize)
					printf("O");
				visualArea(start) = 'O';
				result++;
				break;
			}
			if (shouldVisualize)
				printf(".");

			if (start.y >= max.y)
				goto end;
		}
	}
end:

	return result;
}

DEFINE_TEST_G(Part1, Day14)
{
	TEST_EQ(locPart1("AOC_Day14Test.txt", false), 24);
	TEST_EQ(locPart1("AOC_Day14Part1.txt", false), 817);
}

DEFINE_TEST_G(Part2, Day14)
{
	TEST_EQ(locPart1("AOC_Day14Test.txt", true), 93);
	TEST_EQ(locPart1("AOC_Day14Part1.txt", true), 23416);
}
