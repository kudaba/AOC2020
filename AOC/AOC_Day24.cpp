#include "AOC_Precompiled.h"

GC_DECLARE_ENUM(action, add, mul, div, mod, eql);

struct instruction
{
	action a;

	int index1 = -1;
	int index2 = -1;
	int value2 = -1;
};

static auto locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<instruction>(aFile, [](auto aLine)
		{
			auto parts = GC_StrSplit<3>(aLine, " ");
			instruction i;
			for (action a : action::Range())
				if (GC_Memeq(parts[0].Buffer(), a.ToString(), parts[0].Count()))
				{
					i.a = a;
					break;
				}

			i.index1 = (parts[1][0] - 'x' + 4) % 4;

			if (parts.Count() > 2)
			{
				if (GC_IsAlpha(parts[2][0]))
					i.index2 = (parts[2][0] - 'x' + 4) % 4;
				else
					i.value2 = GC_Atoi(parts[2]);
			}

			return i;
		});
}

struct processor
{
	processor(char const* aFile)
	{
		instructions = locParseData(aFile);
	}

	GC_DynamicArray<instruction> instructions;

	template <typename T>
	void Step(GC_Vector4<T>& reg, uint instruction) const
	{
		auto i = instructions[instruction];
		T& left = reg[i.index1];
		T const right = i.index2 == -1 ? i.value2 : reg[i.index2];
		switch (i.a)
		{
			GC_ENUM_UNREACHABLE(i.a);
		case action::add:
			left += right;
			break;
		case action::mul:
			left *= right;
			break;
		case action::div:
			left /= right;
			break;
		case action::mod:
			left %= right;
			break;
		case action::eql:
			left = left == right;
			break;
		}
	}
};

static uint64 locGetMaxValue(processor const& p, uint64 z, uint ins, GC_HashMap<GC_Pair<uint64,uint>, uint64>& memo, char const* aRange)
{
	if (uint64 const* val = memo.Find({ z, ins }))
		return *val;

	for (char const* c = aRange; *c; ++c)
	{
		GC_Vector4<s64> reg(0, 0, z, *c - '0');
		for (uint i = ins + 1; i < ins + 18; ++i)
			p.Step(reg, i);

		if (ins == p.instructions.Count() - 18)
		{
			if (reg.z == 0)
			{
				GC_Pair<uint64, uint> key(z, ins);
				memo.Add(key, reg.w);
				return reg.w;
			}
		}
		else
		{
			if (uint64 sub = locGetMaxValue(p, (uint)reg.z, ins + 18, memo, aRange))
			{
				uint64 n = sub * 10 + reg.w;
				memo.Add({ z, ins }, n);
				return n;
			}
		}
	}

	memo.Add({ z, ins }, 0);
	return 0;
}

static auto locPart1(char const* aFile, char const* aRange)
{
	processor p(aFile);

	GC_HashMap<GC_Pair<uint64, uint>, uint64> memo;
	memo.Reserve(50000000);
	auto v = locGetMaxValue(p, 0, 0, memo, aRange);

	uint64 n = 0;
	for_range(14)
	{
		n = n * 10 + v % 10;
		v /= 10;
	}
	return n;
}

DEFINE_TEST_G(Part1, Day24)
{
	TEST_EQ(locPart1("AOC_Day24Part1.txt", "987654321"), 92793949489995);
}

DEFINE_TEST_G(Part2, Day24)
{
	TEST_EQ(locPart1("AOC_Day24Part1.txt", "123456789"), 51131616112781);
}
