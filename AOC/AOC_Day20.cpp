#include "AOC_Precompiled.h"
#include "AOC_Day20.h"

struct Side
{
	Side() = default;
	Side(Side const& o) = default;
	Side(Side&& o) = default;

	bool operator==(Side const& o) const { return (o.A == A) || (o.A == B); }
	bool operator!=(Side const& o) const { return (o.A != A) && (o.A != B); }

	Side& operator=(Side const& o) { A = o.A; B = o.B; }
	Side& operator=(Side&& o) = default;

	uint A;
	uint B;
	GC_HybridArray<struct Tile*, 1> Possibilities;
};

GC_DynamicArray2D<bool> tempBits;

struct Tile
{
	uint64 ID;
	GC_DynamicArray2D<bool> Bits;

	Side Sides[4]; // top, right, bottom, left
	bool Arranged = false;

	template <typename Range>
	Side CalcSide(Range r)
	{
		Side side = { 0,0 };
		for_index (bool b : r)
		{
			side.A |= b << i;
			side.B = (side.B << 1) | (uint)b;
		}
		return side;
	}

	void MakeSides()
	{
		Sides[0] = CalcSide(Bits.Row(0));
		Sides[1] = CalcSide(Bits.Column(Bits.Width() - 1));
		Sides[2] = CalcSide(Bits.Row(Bits.Height() - 1));
		Sides[3] = CalcSide(Bits.Column(0));
	}

	void Rotate()
	{
		Side s = GC_Move(Sides[0]);
		for_range(3) Sides[i] = GC_Move(Sides[i + 1]);
		Sides[3] = GC_Move(s);

		GC_Memcpy(tempBits.Buffer(), Bits.Buffer(), Bits.SizeInBytes());
		for_range(tempBits.Height())
		{
			for_index_v(j, bool b : tempBits.Row(i))
			{
				Bits(i, tempBits.Height() - (j + 1)) = b;
			}
		}
	}

	template <typename T> 
	void Reverse(T const& range)
	{
		if (range.Count())
			for (uint i = 0, e = range.Count() - 1; i < e; ++i, --e)
				GC_Swap(range[i], range[e]);
	}

	void FlipX()
	{
		GC_Swap(Sides[1], Sides[3]);
		for_range(Bits.Height())
			Reverse(Bits.Row(i));
	}
	void FlipY()
	{
		GC_Swap(Sides[0], Sides[2]);
		for_range(Bits.Width())
			Reverse(Bits.Column(i));
	}
};

static void locLoadTiles(char const* aFile, GC_DynamicArray<Tile>& tiles)
{
	GC_String text;
	GC_File::ReadAllText(aFile, text);

	for (GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{
		GC_StrSlice line;
		GC_StrLine(chunk, line);

		Tile& tile = tiles.Add();
		tile.ID = GC_Atoi(line.Buffer() + GC_STRING_LENGTH("Tile "));
		while (GC_StrLine(chunk, line))
		{
			uint row = tile.Bits.Height();
			tile.Bits.SetSize({ line.Count(), row + 1 });
			for_index(char c : line)
				tile.Bits(i, row) = c == '#';
		}

		tile.MakeSides();
	}

	for_index_v(t1i, Tile& t1 : tiles)
		for (uint t2i = t1i + 1; t2i < tiles.Count(); ++t2i)
		{
			Tile& t2 = tiles[t2i];
			for_range(4)
			{
				for_range_v(j, 4)
				{
					if (t1.Sides[i] == t2.Sides[j])
					{
						t1.Sides[i].Possibilities.Add(&t2);
						t2.Sides[j].Possibilities.Add(&t1);

						break;
					}
				}
			}
		}
}

static uint64 locDay20Part1(char const* aFile)
{
	GC_DynamicArray<Tile> tiles;
	locLoadTiles(aFile, tiles);

	uint64 result = 1;
	for (Tile const& tile : tiles)
	{
		uint sides = 0;
		for (Side const& s : tile.Sides)
			sides += s.Possibilities.Count();

		if (sides == 2)
			result *= tile.ID;
	}

	return result;
}

DEFINE_TEST_G(Part1, Day20)
{
	TEST_EQ(locDay20Part1("AOC_Day20Test.txt"), 20899048083289);
	TEST_EQ(locDay20Part1("AOC_Day20Part1.txt"), 8581320593371);
}

static bool locIsOriented(Tile* tile, Side* left, Side* top)
{
	return
		(left ? tile->Sides[3] == *left : tile->Sides[3].Possibilities.Count() == 0) &&
		(top ? tile->Sides[0] == *top : tile->Sides[0].Possibilities.Count() == 0);
}

static void locOrientTile(Tile* tile, Side* left, Side* top)
{
	for_range(4)
	{
		if (locIsOriented(tile, left, top))
			break;

		tile->FlipX();

		if (locIsOriented(tile, left, top))
			break;

		tile->FlipX();
		tile->FlipY();

		if (locIsOriented(tile, left, top))
			break;

		tile->FlipY();
		tile->Rotate();
	}

	GC_ASSERT(locIsOriented(tile, left, top));
}

static void locBuildTileImage(GC_DynamicArray<Tile> &tiles, GC_DynamicArray2D<Tile*>& tileImage)
{
	uint size = (uint)GC_Sqrt((float)tiles.Count());
	tileImage.SetSize({ size,size });

	for (Tile& tile : tiles)
	{
		uint sides = 0;
		for (Side const& s : tile.Sides)
			sides += s.Possibilities.Count();

		if (sides == 2)
		{
			tileImage(0, 0) = &tile;
			locOrientTile(&tile, nullptr, nullptr);
			break;
		}
	}

	GC_Vector2u index(1, 0);
	while (index.y != tileImage.Height())
	{
		Side* left = nullptr;
		Side* top = nullptr;

		if (index.x)
			left = &tileImage(index.x - 1, index.y)->Sides[1];
		if (index.y)
			top = &tileImage(index.x, index.y - 1)->Sides[2];

		Tile* nextTile = (left ? left : top)->Possibilities.First();
		tileImage(index) = nextTile;
		locOrientTile(nextTile, left, top);

		++index.x;
		if (index.x == tileImage.Width())
		{
			index.x = 0;
			++index.y;
		}
	}
}

static void locBuildImage(GC_DynamicArray2D<Tile*> &tiles, GC_DynamicArray2D<bool>& image)
{
	uint tileCount = tiles.Width();
	uint tileSize = tiles(0, 0)->Bits.Width() - 2;
	uint size = tileCount * tileSize;
	image.SetSize({ size, size });

	GC_Vector2u offset(0);
	for (Tile* t : tiles)
	{
		GC_Vector2u areaMin(1);
		GC_Vector2u areaMax(t->Bits.Size() - areaMin);

		GC_ArrayOperations2D::Copy(image.AsArea(), t->Bits.Area(areaMin, areaMax), offset);
		offset.x += areaMax.x - areaMin.x;
		if (offset.x == image.Width())
		{
			offset.x = 0;
			offset.y += areaMax.y - areaMin.y;
		}
	}

	//for_range(image.Height())
	//{
	//	for_index_v(j, bool b : image.Row(i))
	//	{
	//		GC_TestFixture::GetCurrentTest()->Printf(b ? "#" : ".");
	//	}

	//	GC_TestFixture::GetCurrentTest()->Printf("\n");
	//}
}

static uint locFindDragons(GC_DynamicArray2D<bool>& image, GC_DynamicArray<GC_Vector2u> const& dragonMarkers, GC_Vector2u dragonSize)
{
	uint dragons = 0;
	for_range_v(x, image.Width() - dragonSize.x)
	{
		for_range_v(y, image.Height() - dragonSize.y)
		{
			bool match = true;
			for (GC_Vector2u v : dragonMarkers)
				if (!image(v + GC_Vector2u(x,y)))
				{
					match= false;
					break;
				}

			if (match)
			{
				++dragons;
			}
		}
	}
	return dragons;
}

static uint locFindDragons(Tile* tile, GC_DynamicArray<GC_Vector2u> const& dragonMarkers, GC_Vector2u dragonSize)
{
	for_range(4)
	{
		if (uint dragons = locFindDragons(tile->Bits, dragonMarkers, dragonSize))
			return dragons;

		tile->FlipX();

		if (uint dragons = locFindDragons(tile->Bits, dragonMarkers, dragonSize))
			return dragons;

		tile->FlipX();
		tile->FlipY();

		if (uint dragons = locFindDragons(tile->Bits, dragonMarkers, dragonSize))
			return dragons;

		tile->FlipY();
		tile->Rotate();
	}
	return 0;
}

static uint locDay20Part2(char const* aFile)
{
	GC_DynamicArray<Tile> tiles;
	locLoadTiles(aFile, tiles);

	tempBits.SetSize(tiles[0].Bits.Size());

	GC_DynamicArray2D<Tile*> tileImage;
	locBuildTileImage(tiles, tileImage);

	Tile image;
	locBuildImage(tileImage, image.Bits);

	char const* dragonText[] =
	{
"                  # ",
"#    ##    ##    ###",
" #  #  #  #  #  #   ",
	};

	GC_Vector2u dragonSize = { GC_Strlen(dragonText[0]), GC_ARRAY_COUNT(dragonText) };
	GC_DynamicArray<GC_Vector2u> dragonMarkers;
	for_range_v(y, dragonSize.y)
		for_range_v(x, dragonSize.x)
		if (dragonText[y][x] == '#')
			dragonMarkers.Add({ x, y });

	tempBits.SetSize(image.Bits.Size());
	uint dragons = locFindDragons(&image, dragonMarkers, dragonSize);

	uint mapLength = 0;
	for (bool b : image.Bits)
		if (b) ++mapLength;

	return mapLength - dragonMarkers.Count() * dragons;
}

DEFINE_TEST_G(Part2, Day20)
{
	TEST_EQ(locDay20Part2("AOC_Day20Test.txt"), 273);
	TEST_EQ(locDay20Part2("AOC_Day20Part1.txt"), 2031);

	tempBits.Reset();
}
