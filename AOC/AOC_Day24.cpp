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
		Blocks = instructions.Count() / 14;
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

	const uint Div = 4;
	const uint ModAdd = 5;
	const uint ValAdd = 13;
	uint Blocks;

	int GetDiv(uint block) const { return instructions[block*Blocks + Div].value2; }
	int GetModAdd(uint block) const { return instructions[block*Blocks + ModAdd].value2; }
	int GetValAdd(uint block) const { return instructions[block*Blocks + ValAdd].value2; }

	template <typename T>
	T Run(uint block, T z, int val) const
	{
		GC_Vector4<T> reg(0, 0, z, val);
		for (int i = block*Blocks + 1, e = block*Blocks + 18; i < e; ++i)
			Step(reg, i);
		return reg.z;
	}
};

static auto locPart1(char const* aFile, char const* someDigits)
{
	processor p(aFile);

	GC_HybridArray<GC_Pair<int, int>, 7> pairs;
	GC_HybridArray<int, 7> pairStack;
	GC_HybridArray<int, 9> orderedDigits;
	for_range(9) orderedDigits.Add(someDigits[i] - '0');

	for_range(14)
	{
		if (p.GetDiv(i) == 1)
			pairStack.PushBack(i);
		else
		{
			pairs.Add({ pairStack.Last(), i });
			pairStack.PopBack();
		}
	}

	GC_ASSERT(pairStack.Count() == 0 && pairs.Count() == 7);

	uint64 digits[14];

	for (auto pair : pairs)
	{
		for_range(9)
		{
			int z = p.Run(pair.myFirst, 0, orderedDigits[i]);
			bool found = false;

			for_range_v(j, 9)
			{
				if (p.Run(pair.mySecond, z, orderedDigits[j]) == 0)
				{
					found = true;
					digits[pair.myFirst] = orderedDigits[i];
					digits[pair.mySecond] = orderedDigits[j];
				}
			}

			if (found) break;
		}
	}

	uint64 result = 0;
	for (uint64 d : digits)
		result = result * 10 + d;
	return result;
}

DEFINE_TEST_G(Part1, Day24)
{
	TEST_EQ(locPart1("AOC_Day24Part1.txt", "987654321"), 92793949489995);
}

DEFINE_TEST_G(Part2, Day24)
{
	TEST_EQ(locPart1("AOC_Day24Part1.txt", "123456789"), 51131616112781);
}
