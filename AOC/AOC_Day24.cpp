#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile, bool second = false)
{
	struct Bliz : GC_HybridArray<char, 32>
	{
		uint Turn = 0;
	};
	
	auto map = GC_File::Parse2d<Bliz>(aFile, [](char c) { Bliz b; if (!(c == '.' || c == '#')) b.Add(c); return b; });

	GC_Vector2u const size = map.Size();

	GC_Vector2u const s = { 1, 0 };
	GC_Vector2u const e = { size.x - 2, size.y - 1};

	struct State
	{
		GC_Vector2u Position;
		uint Turn;
		bool End;

		bool operator==(State const& s) const
		{
			return Position == s.Position && (s.End || Turn == s.Turn);
		}
	};

	auto nextP = [size](GC_Vector2u p, char c)
	{
		GC_Vector2i dir = {};
		switch (c)
		{
		case '<': dir = { -1,  0 }; break;
		case '>': dir = { 1,  0 }; break;
		case '^': dir = { 0, -1 }; break;
		case 'v': dir = { 0,  1 }; break;
		default: GC_UNREACHABLE();
		}

		GC_Vector2u nb = p + dir;
		if (nb.x == 0)
			nb.x = size.x - 2;
		if (nb.x == size.x - 1)
			nb.x = 1;
		if (nb.y == 0)
			nb.y = size.y - 2;
		if (nb.y == size.y - 1)
			nb.y = 1;
		return nb;
	};

	auto runDj = [&](GC_Vector2u start, GC_Vector2u end)
	{
		GC_DynamicArray<GC_Vector2u> positions;
		positions.Reserve(10000);
		positions.Add(start);

		GC_DynamicArray<GC_Vector2u> p2;
		p2.Reserve(10000);

		uint turn = 0;
		decltype(map) b2;
		b2.SetSize(size);

		while (1)
		{
			++turn;

			for (uint y = 1, ye = map.Height() - 1; y < ye; ++y)
				for (uint x = 1, xe = map.Width() - 1; x < xe; ++x)
			{
				auto& b = map(x, y);
				while (b.Count())
				{
					b2(nextP({x,y}, b.Last())).Add(b.Last());
					b.PopBack();
				}
			}

			map.Swap(b2);

			for (auto p : positions)
			{
				{
					auto& b = map(p);
					if (b.Turn != turn && b.IsEmpty())
					{
						p2.Add(p);
						b.Turn = turn;
					}
				}

				for (GC_Cardinal d : GC_Cardinal::Range())
				{
					GC_Vector2u np = p + GC_CardinalDirection(d);
					if (np == end)
						return turn;

					if (GC_InRange<GC_Vector2u>(np, { 1,1 }, size - GC_Vector2u(2)))
					{
						auto& b = map(np);
						if (b.Turn != turn && b.IsEmpty())
						{
							p2.Add(np);
							b.Turn = turn;
						}
					}
				}
			}

			positions.Clear();
			p2.Swap(positions);
		}
	};

	auto result = runDj(s, e);

	if (second)
	{
		for (auto& b : map) b.Turn = 0;
		result += runDj(e, s);
		for (auto& b : map) b.Turn = 0;
		result += runDj(s, e);
	}
	return result;
}

DEFINE_TEST_G(Part1, Day24)
{
	TEST_EQ(locPart1("AOC_Day24Test.txt"), 18);
	TEST_EQ(locPart1("AOC_Day24Part1.txt"), 269);
}

DEFINE_TEST_G(Part2, Day24)
{
	TEST_EQ(locPart1("AOC_Day24Test.txt", true), 54);
	TEST_EQ(locPart1("AOC_Day24Part1.txt", true), 825);
}
