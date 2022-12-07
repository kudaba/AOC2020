#include "AOC_Precompiled.h"

struct Folder
{
	Folder* Parent = nullptr;
	GC_HashMap<GC_FilePath, uint64> Files;
	GC_HashMap<GC_FilePath, Folder> Dirs;

	uint64 Size() const
	{
		return
			GC_Algorithm::Sum(Files, [](auto const& kv) -> uint64 { return kv.Value; }) +
			GC_Algorithm::Sum(Dirs, [](auto const& kv) -> uint64 { return kv.Value.Size(); });
	}

	template <typename T>
	void Walk(T Pred) const
	{
		Pred(this);
		for (auto const& kv : Dirs)
		{
			kv.Value.Walk(Pred);
		}
	}

	void Build(char const* aFile)
	{
		Folder* cur = nullptr;

		// By line parsing
		for (auto line : GC_File::ReadAllLines(aFile))
		{
			if (line[0] == '$')
			{
				if (line[2] == 'c')
				{
					GC_FilePath np(line.Range(5));

					if (np[0] == '/')
					{
						cur = this;
					}
					else if (np[0] == '.' && np[1] == '.')
					{
						cur = cur->Parent ? cur->Parent : this;
					}
					else
					{
						cur = cur->Dirs.Find(np);
					}
				}
				continue;
			}
			else if (line[0] == 'd')
			{
				cur->Dirs.Add(line.Range(4), { cur });
			}
			else
			{
				auto part = GC_StrSplit<2>(line, ' ');
				cur->Files.Add(part[1], GC_Atoi(part[0]));
			}
		}
	}
};

static auto locPart1(char const* aFile)
{
	Folder root;
	root.Build(aFile);

	uint64 result = 0;

	root.Walk([&](Folder const* f)
		{
			uint64 size = f->Size();
			if (size <= 100000)
				result += size;
		});

	return result;
}

DEFINE_TEST_G(Part1, Day7)
{
	TEST_EQ(locPart1("AOC_Day7Test.txt"), 95437);
	TEST_EQ(locPart1("AOC_Day7Part1.txt"), 1644735);
}

static auto locPart2(char const* aFile)
{
	Folder root;
	root.Build(aFile);

	uint64 result = UINT_MAX;
	uint64 free = 70000000 - root.Size();
	uint64 required = 30000000 - free;

	root.Walk([&](Folder const* f)
		{
			uint64 size = f->Size();
			if (size > required && size < result)
				result = size;
		});

	return result;
}

DEFINE_TEST_G(Part2, Day7)
{
	TEST_EQ(locPart2("AOC_Day7Test.txt"), 24933642);
	TEST_EQ(locPart2("AOC_Day7Part1.txt"), 1300850);
}
