#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	auto data = GC_File::Parse2d<char>(aFile, [](char c) {return c; });
	auto const size = data.Size();

	GC_DynamicArray<GC_Vector2u> seac[2];

	for_range2d(size.x,size.y)
		switch (data(x, y))
		{
		case '>': seac[0].Add({ x,y }); break;
		case 'v': seac[1].Add({ x,y }); break;
		}

	GC_DynamicArray<GC_Pair<GC_Vector2u, GC_Vector2u>> moves;
	moves.Reserve(size.Area());
	const GC_Vector2u dirs[] = { {1, 0}, {0, 1} };
	const char mdir[] = { '>', 'v' };

	uint turn = 0;
	while (1)
	{
		++turn;
		uint totalMoves = 0;

		for_range(2)
		{
			moves.Clear();
			for (auto& c : seac[i])
				if (GC_Vector2u m2 = GC_Modulo(c + dirs[i], size); data(m2) == '.')
				{
					moves.Add({ c, m2 });
					c = m2;
				}

			totalMoves += moves.Count();
			for (auto m : moves)
				GC_Swap(data(m.myFirst), data(m.mySecond));
		}

		if (totalMoves == 0)
			break;
	}

	return turn;
}

DEFINE_TEST_G(Part1, Day25)
{
	TEST_EQ(locPart1("AOC_Day25Test.txt"), 58);
	TEST_EQ(locPart1("AOC_Day25Part1.txt"), 471);
}
