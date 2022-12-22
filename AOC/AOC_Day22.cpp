#include "AOC_Precompiled.h"

static auto locPart1(char const* aFile)
{
	auto linesData = GC_File::ReadAllLines(aFile);
	auto lines = linesData.GetAll();

	uint width = 0;
	for_range_v(x, lines.Count() - 2)
		width = GC_Max(width, lines[x].Count());

	GC_DynamicArray2D<char> map(width, lines.Count() - 2, 0);

	for_range_v(x, lines.Count() - 2)
	{
		GC_Memcpy(map.Row(x).Buffer(), lines[x].Buffer(), lines[x].Count());
	}

	GC_Vector2i pos(GC_Algorithm::IndexOf(lines[0], '.'), 0);
	GC_Cardinal dir = GC_Cardinal::East;

	uint dist = 0;

	auto move = [&]()
	{
		GC_Vector2i vdir = GC_CardinalDirection(dir) * GC_Vector2i(1,-1);
		for_range(dist)
		{
			GC_Vector2i next = pos + vdir;
			next.x = GC_LoopClamp<int>(next.x, 0, map.Width());
			next.y = GC_LoopClamp<int>(next.y, 0, map.Height());
			while (map(next) == 0 || map(next) == ' ')
			{
				next += vdir;
				next.x = GC_LoopClamp<int>(next.x, 0, map.Width());
				next.y = GC_LoopClamp<int>(next.y, 0, map.Height());
			}

			if (map(next) == '.')
				pos = next;
			else
				break;
		}
	};

	for (char c : lines.Last())
	{
		if (GC_IsDigit(c))
			dist = dist * 10 + c - '0';
		else
		{
			move();
			dir = c == 'R' ? GC_CardinalRotateRight(dir) : GC_CardinalRotateLeft(dir);
			dist = 0;
		}
	}

	if (dist) move();

	return (pos.x + 1) * 4 + (pos.y + 1)  * 1000 + (dir + 3) % 4;
}

DEFINE_TEST_G(Part1, Day22)
{
	TEST_EQ(locPart1("AOC_Day22Test.txt"), 6032);
	TEST_EQ(locPart1("AOC_Day22Part1.txt"), 11464);
}

GC_DECLARE_ENUM(Facing, Top, Back, Left, Front, Bottom, Right);

struct Neighbor
{
	GC_Function<GC_Vector2i(GC_Vector2i)> Pos;
	Facing Face;
	GC_Cardinal Dir;
};

uint locRunPart2(GC_DynamicArray2D<char> const& map,GC_StrSlice line, Neighbor const neighbors[6][4], GC_ArrayRange2D<char> sides[6])
{
	int const size = sides[0].Width();

	GC_Vector2i pos(0);
	Facing facing = Facing::Top;
	GC_Cardinal dir = GC_Cardinal::East;

	for (auto x : Facing::Range())
		for (auto y : GC_Cardinal::Range())
	{
		GC_Vector2i tpos = GC_Clamp(GC_CardinalDirection(y) * GC_Vector2i(size,-size), GC_Vector2i(0), GC_Vector2i(size - 1));
		Neighbor const& n = neighbors[x][y];
		GC_Vector2i npos = n.Pos(tpos);
		GC_Vector2i rpos = neighbors[n.Face][GC_CardinalOpposite(n.Dir)].Pos(npos);
		GC_ASSERT(tpos == rpos);
	}

	uint dist = 0;

	auto move = [&](bool skip = false)
	{
		GC_Cardinal nextDir = dir;
		Facing nextFacing = facing;
		for_range(dist)
		{
			GC_Vector2i next = pos + GC_CardinalDirection(nextDir) * GC_Vector2i(1, -1);
			if (!GC_InArrayRange<GC_Vector2i>(next, GC_Vector2i(0), GC_Vector2i(size)))
			{
				Neighbor const& n = neighbors[nextFacing][nextDir];
				next = n.Pos(next);
				nextDir = n.Dir;
				nextFacing = n.Face;
			}

			if (skip || sides[nextFacing](next) != '#')
			{
				//char repl[] = { '^', '>', 'v', '<' };
				//sides[nextFacing](next) = repl[nextFacing];
				pos = next;
				dir = nextDir;
				facing = nextFacing;
			}
			else
			{
				GC_ASSERT(sides[nextFacing](next) == '#');
				break;
			}
		}
	};

	for (auto x : Facing::Range())
		for (auto y : GC_Cardinal::Range())
	{
			GC_Vector2i bpos = pos = GC_Vector2i { 0 };
			Facing bfacing = facing = x;
			GC_Cardinal bdir = dir = y;

			dist = size * 4;
			move(true);

			GC_ASSERT(bpos == pos);
			GC_ASSERT(bfacing == facing);
			GC_ASSERT(bdir == dir);
	}

	dist = 0;
	facing = Facing::Top;
	dir = GC_Cardinal::East;

	for (char c : line)
	{
		if (GC_IsDigit(c))
			dist = dist * 10 + c - '0';
		else
		{
			move();
			dir = c == 'R' ? GC_CardinalRotateRight(dir) : GC_CardinalRotateLeft(dir);
			dist = 0;
		}
	}

	if (dist) move();

	GC_Vector2u finalPos = map.IndexOf(sides[facing](pos));

	return (finalPos.x + 1) * 4 + (finalPos.y + 1) * 1000 + (dir + 3) % 4;
}

static auto locPart2(char const* aFile, uint size)
{
	auto linesData = GC_File::ReadAllLines(aFile);
	auto lines = linesData.GetAll();

	uint width = 0;
	for_range_v(x, lines.Count() - 2)
		width = GC_Max(width, lines[x].Count());

	GC_DynamicArray2D<char> map(width, lines.Count() - 2, 0);

	for_range_v(x, lines.Count() - 2)
	{
		GC_Memcpy(map.Row(x).Buffer(), lines[x].Buffer(), lines[x].Count());
	}

	GC_Vector2u squares[] = {
		{size * 2, 0 },
		{0, size },
		{size, size },
		{size * 2, size },
		{size * 2, size * 2},
		{size * 3, size * 2 },
	};

	uint const inv = size - 1;

	// GC_DECLARE_ENUM(Facing, Top, Back, Left, Front, Bottom, Right);
	// GC_DECLARE_ENUM(GC_Cardinal, North, East, South, West);	
	Neighbor neighbors[Facing::Count][GC_Cardinal::Count] =
	{
		{ // Top
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv - p.x, 0); }, Facing::Back, GC_Cardinal::South },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, inv - p.y); }, Facing::Right, GC_Cardinal::West },
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, 0); }, Facing::Front, GC_Cardinal::South},
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.y, 0); }, Facing::Left, GC_Cardinal::South },
		},
		{ // Back
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv - p.x, 0); }, Facing::Top, GC_Cardinal::South },
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, p.y); }, Facing::Left, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv - p.x, inv); }, Facing::Bottom, GC_Cardinal::North},
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv - p.y, inv); }, Facing::Right, GC_Cardinal::North },
		},
		{ // Left
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, p.x); }, Facing::Top, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, p.y); }, Facing::Front, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, inv - p.x); }, Facing::Bottom, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, p.y); }, Facing::Back, GC_Cardinal::West },
		},
	// GC_DECLARE_ENUM(GC_Cardinal, North, East, South, West);	
		{ // Front
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, inv); }, Facing::Top, GC_Cardinal::North },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv - p.y, 0); }, Facing::Right, GC_Cardinal::South },
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, 0); }, Facing::Bottom, GC_Cardinal::South },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, p.y); }, Facing::Left, GC_Cardinal::West },
		},
		{ // Bottom
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, inv); }, Facing::Front, GC_Cardinal::North },
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, p.y); }, Facing::Right, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv - p.x, inv); }, Facing::Back, GC_Cardinal::North },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv - p.y, inv); }, Facing::Left, GC_Cardinal::North },
		},
		{ // Right
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, inv - p.x); }, Facing::Front, GC_Cardinal::West },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, inv - p.y); }, Facing::Top, GC_Cardinal::West },
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, inv - p.x); }, Facing::Back, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, p.y); }, Facing::Bottom, GC_Cardinal::West },
		},
	};

	GC_ArrayRange2D<char> sides[6];
	for_index(auto& a : sides)
		a = map.Area(squares[i], squares[i]+ GC_Vector2u(size));

	return locRunPart2(map, lines.Last(), neighbors, sides);
}

static auto locPart22(char const* aFile, uint size)
{
	auto linesData = GC_File::ReadAllLines(aFile);
	auto lines = linesData.GetAll();

	uint width = 0;
	for_range_v(x, lines.Count() - 2)
		width = GC_Max(width, lines[x].Count());

	GC_DynamicArray2D<char> map(width, lines.Count() - 2, 0);

	for_range_v(x, lines.Count() - 2)
	{
		GC_Memcpy(map.Row(x).Buffer(), lines[x].Buffer(), lines[x].Count());
	}

	// GC_DECLARE_ENUM(Facing, Top, Back, Left, Front, Bottom, Right);
	GC_Vector2u squares[] = {
		{size, 0 },
		{0, size*3 },
		{0, size*2 },
		{size, size },
		{size, size*2 },
		{size * 2, 0 },
	};

	uint const inv = size - 1;

	// GC_DECLARE_ENUM(Facing, Top, Back, Left, Front, Bottom, Right);
	// GC_DECLARE_ENUM(GC_Cardinal, North, East, South, West);	
	Neighbor neighbors[Facing::Count][GC_Cardinal::Count] =
	{
		{ // Top
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, p.x); }, Facing::Back, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, p.y); }, Facing::Right, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, 0); }, Facing::Front, GC_Cardinal::South},
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, inv - p.y); }, Facing::Left, GC_Cardinal::East },
		},
		{ // Back
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, inv); }, Facing::Left, GC_Cardinal::North },
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.y, inv); }, Facing::Bottom, GC_Cardinal::North},
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, 0); }, Facing::Right, GC_Cardinal::South},
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.y, 0); }, Facing::Top, GC_Cardinal::South },
		},
		{ // Left
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, p.x); }, Facing::Front, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, p.y); }, Facing::Bottom, GC_Cardinal::East },
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, 0); }, Facing::Back, GC_Cardinal::South},
			{ [=](GC_Vector2i p) { return GC_Vector2i(0, inv - p.y); }, Facing::Top, GC_Cardinal::East},
		},
	// GC_DECLARE_ENUM(GC_Cardinal, North, East, South, West);	
		{ // Front
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, inv); }, Facing::Top, GC_Cardinal::North },
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.y, inv); }, Facing::Right, GC_Cardinal::North },
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, 0); }, Facing::Bottom, GC_Cardinal::South },
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.y, 0); }, Facing::Left, GC_Cardinal::South },
		},
		{ // Bottom
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, inv); }, Facing::Front, GC_Cardinal::North },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, inv - p.y); }, Facing::Right, GC_Cardinal::West },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, p.x); }, Facing::Back, GC_Cardinal::West },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, p.y); }, Facing::Left, GC_Cardinal::West},
		},
		{ // Right
			{ [=](GC_Vector2i p) { return GC_Vector2i(p.x, inv); }, Facing::Back, GC_Cardinal::North},
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, inv - p.y); }, Facing::Bottom, GC_Cardinal::West},
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, p.x); }, Facing::Front, GC_Cardinal::West },
			{ [=](GC_Vector2i p) { return GC_Vector2i(inv, p.y); }, Facing::Top, GC_Cardinal::West },
		},
	};

	GC_ArrayRange2D<char> sides[6];
	for_index(auto & a : sides)
		a = map.Area(squares[i], squares[i] + GC_Vector2u(size));

	return locRunPart2(map, lines.Last(), neighbors, sides);
 }

DEFINE_TEST_G(Part2, Day22)
{
	TEST_EQ(locPart2("AOC_Day22Test.txt", 4), 5031);
	TEST_EQ(locPart22("AOC_Day22Part1.txt", 50), 197122);
}

// Attempt at generic solver

//static auto locPart23(char const* aFile, uint size)
//{
//	auto linesData = GC_File::ReadAllLines(aFile);
//	auto lines = linesData.GetAll();
//
//	uint width = 0;
//	for_range_v(x, lines.Count() - 2)
//		width = GC_Max(width, lines[x].Count());
//
//	GC_DynamicArray2D<char> map(width, lines.Count() - 2, ' ');
//
//	for_range_v(x, lines.Count() - 2)
//	{
//		GC_Memcpy(map.Row(x).Buffer(), lines[x].Buffer(), lines[x].Count());
//	}
//
//	struct Face;
//
//	struct Neighbor
//	{
//		Face* Face = nullptr;
//		GC_Cardinal Orientation = GC_Cardinal::North;
//	};
//
//	struct Face
//	{
//		GC_ArrayRange2D<char> Data;
//		Neighbor Neighbors[4] = {};
//		GC_Vector2u Coord;
//	};
//
//	Face faces[6];
//	uint faceIndex = 0;
//
//	for (uint y = 0; y < map.Height(); y += size)
//	{
//		for (uint x = 0; x < map.Width(); x += size)
//		{
//			if (map(x, y) != ' ')
//			{
//				Face& f = faces[faceIndex++];
//				f.Data = map.Area(x, y, x + size, y + size);
//				f.Coord = { x,y };
//			}
//		}
//	}
//
//	for (Face& f : faces)
//	{
//		for (Face& n : faces)
//		{
//			if (n.Coord.y == f.Coord.y &&
//				n.Coord.x == f.Coord.x + size)
//			{
//				f.Neighbors[GC_Cardinal::East].Face = &n;
//				n.Neighbors[GC_Cardinal::West].Face = &f;
//			}
//			if (n.Coord.y == f.Coord.y + size &&
//				n.Coord.x == f.Coord.x)
//			{
//				f.Neighbors[GC_Cardinal::South].Face = &n;
//				n.Neighbors[GC_Cardinal::North].Face = &f;
//			}
//		}
//	}
//
//	uint matched = 0;
//
//	bool changed = true;
//	while (changed)
//	{
//		changed = false;
//		for (Face const& f : faces)
//		{
//			for (auto rd : GC_Cardinal::Range())
//			{
//				auto sd = GC_CardinalRotateRight(rd);
//
//				auto const& rn = f.Neighbors[rd];
//				auto const& sn = f.Neighbors[sd];
//
//				Face* r = f.Neighbors[rd].Face;
//				Face* s = f.Neighbors[sd].Face;
//				if (r && s)
//				{
//					auto nrd = (rd + 4 - sn.Orientation) % GC_Cardinal::Count;
//					auto nsd = (sd + 4 - rn.Orientation) % GC_Cardinal::Count;
//
//					auto& nrn = r->Neighbors[nsd];
//					auto& nsn = s->Neighbors[nrd];
//
//					if (!nrn.Face)
//					{
//						++matched;
//
//						GC_ASSERT(!nsn.Face);
//
//						nsn.Face = r;
//						nsn.Orientation = (GC_Cardinal)((f.Neighbors[rd].Orientation + 1) % GC_Cardinal::Count);
//
//						nrn.Face = s;
//						nrn.Orientation = (GC_Cardinal)((f.Neighbors[sd].Orientation + 3) % GC_Cardinal::Count);
//						changed = true;
//					}
//
//					GC_ASSERT(nsn.Face == r && nrn.Face == s);
//					GC_ASSERT((nsn.Orientation == GC_Cardinal::North && nrn.Orientation == GC_Cardinal::North) ||
//							  (nsn.Orientation == GC_Cardinal::South && nrn.Orientation == GC_Cardinal::South) ||
//							  (nsn.Orientation == GC_Cardinal::West && nrn.Orientation == GC_Cardinal::East) ||
//							  (nsn.Orientation == GC_Cardinal::East && nrn.Orientation == GC_Cardinal::West));
//				}
//			}
//		}
//	}
//
//	for (Face& f : faces)
//		for (auto const& n : f.Neighbors)
//			GC_ASSERT(n.Face);
//
//	return 0;
//}
//
//DEFINE_TEST_G(Part3, Day22)
//{
//	TEST_EQ(locPart23("AOC_Day22Test.txt", 4), 5031);
//	TEST_EQ(locPart23("AOC_Day22Part1.txt", 50), 197122);
//}
