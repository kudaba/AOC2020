#include "AOC_Precompiled.h"

struct Board
{
	int Numbers[5][5];
};

static uint locPart1(char const* aFile)
{
	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);

	GC_DynamicArray<int> calls;
	GC_DynamicArray<Board> boards;
	
	bool first = true;
	for (GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{
		if (first)
		{
			for (auto s : GC_StrSplit(chunk, ","))
				calls.Add(GC_Atoi(s));

			first = false;
		}
		else
		{
			Board& b = boards.Add();
			for_index(auto line : GC_StrSplit(chunk, "\n"))
			{
				for_index_v(j, auto n : GC_StrSplit(line, " ", false))
					b.Numbers[i][j] = GC_Atoi(n);
			}
		}
	}

	for (int n : calls)
	{
		for (Board& b : boards)
		{
			for (uint i = 0; i < 5; ++i)
				for (uint j = 0; j < 5; ++j)
					if (b.Numbers[i][j] == n)
					{
						b.Numbers[i][j] = -1;

						int sx = 0, sy = 0;
						for (uint x = 0; x < 5; ++x)
						{
							sx += b.Numbers[i][x];
							sy += b.Numbers[x][j];
						}

						if (sx == -5 || sy == -5)
						{
							int sum = 0;
							for (uint x = 0; x < 5; ++x)
							{
								for (uint y = 0; y < 5; ++y)
									if (b.Numbers[x][y] != -1)
										sum += b.Numbers[x][y];
							}

							return sum * n;
						}
					}
		}
	}

	return 0;
}

DEFINE_TEST_G(Part1, Day4)
{
	TEST_EQ(locPart1("AOC_Day4Test.txt"), 4512);
	TEST_EQ(locPart1("AOC_Day4Part1.txt"), 65325);
}

static uint locPart2(char const* aFile)
{
	// By Block parsing (block of lines separate by two new lines)
	GC_String text;
	GC_File::ReadAllText(aFile, text);

	GC_DynamicArray<int> calls;
	GC_DynamicArray<Board> boards;

	bool first = true;
	for (GC_StrSlice chunk; GC_Strtok(text, "\n\n", chunk);)
	{
		if (first)
		{
			for (auto s : GC_StrSplit(chunk, ","))
				calls.Add(GC_Atoi(s));

			first = false;
		}
		else
		{
			Board& b = boards.Add();
			for_index(auto line : GC_StrSplit(chunk, "\n"))
			{
				for_index_v(j, auto n : GC_StrSplit(line, " ", false))
					b.Numbers[i][j] = GC_Atoi(n);
			}
		}
	}

	for (int n : calls)
	{
		for (uint bi = 0; bi < boards.Count(); ++bi)
		{
			Board& b = boards[bi];

			for (uint i = 0; i < 5; ++i)
				for (uint j = 0; j < 5; ++j)
					if (b.Numbers[i][j] == n)
					{
						b.Numbers[i][j] = -1;

						int sx = 0, sy = 0;
						for (uint x = 0; x < 5; ++x)
						{
							sx += b.Numbers[i][x];
							sy += b.Numbers[x][j];
						}

						if (sx == -5 || sy == -5)
						{
							if (boards.Count() > 1)
								boards.RemoveAt(bi);
							else
							{
								int sum = 0;
								for (uint x = 0; x < 5; ++x)
								{
									for (uint y = 0; y < 5; ++y)
										if (b.Numbers[x][y] != -1)
											sum += b.Numbers[x][y];
								}

								return sum * n;
							}
						}
					}
		}
	}

	return 0;
}

DEFINE_TEST_G(Part2, Day4)
{
	TEST_EQ(locPart2("AOC_Day4Test.txt"), 1924);
	TEST_EQ(locPart2("AOC_Day4Part1.txt"), 4624);
}
