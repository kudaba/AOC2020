#include "AOC_Precompiled.h"

struct Processor
{
	Processor(char const* aText)
		: myOffset(0)
		, myVersion(0)
	{
		myData.Resize(GC_Strlen(aText) * 4);

		uint bit = 0;
		for (char const* c = aText; *c; ++c)
		{
			char digit = GC_IsDigit(*c) ? *c - '0' : *c - 'A' + 10;
			for_range(4)
				myData[bit++] = (digit & (1 << (3 - i))) != 0;
		}
	}

	uint64 Run()
	{
		myVersion += ReadBits(3);
		switch (ReadBits(3))
		{
		case 4:
		{
			uint64 value = 0;
			uint64 bits = 0;
			do
			{
				bits = ReadBits(5);
				value = (value << 4) | bits & 0xf;
			} while (bits >= 0x10);
			return value;
		}
		case 0:
			return GC_Algorithm::Sum(ReadNumbers());
		case 1:
			return GC_Algorithm::Product(ReadNumbers());
		case 2:
			return GC_Algorithm::Min(ReadNumbers());
		case 3:
			return GC_Algorithm::Max(ReadNumbers());
		case 5:
		{
			auto numbers = ReadNumbers();
			return numbers[0] > numbers[1];
		}
		case 6:
		{
			auto numbers = ReadNumbers();
			return numbers[0] < numbers[1];
		}
		case 7:
		{
			auto numbers = ReadNumbers();
			return numbers[0] == numbers[1];
		}
		}
		return 0;
	}

	uint64 GetVersion()
	{
		Run();
		return myVersion;
	}

private:
	uint64 ReadBits(uint aBits)
	{
		uint64 result = 0;
		for_range(aBits)
			result = (result << 1) | (u64)myData[myOffset + i];
		myOffset  += aBits;
		return result;
	}
	GC_HybridArray<uint64, 16> ReadNumbers()
	{
		GC_HybridArray<uint64, 16> numbers;

		if (ReadBits(1))
		{
			auto count = ReadBits(11);
			for_range(count)
				numbers.Add(Run());
		}
		else
		{
			auto len = ReadBits(15);
			for (auto e = myOffset + len; myOffset < e; numbers.Add(Run()));
		}
		return numbers;
	}

	GC_BitVector myData;
	uint myOffset;
	uint64 myVersion;
};

DEFINE_TEST_G(Part1, Day16)
{
	TEST_EQ(Processor("8A004A801A8002F478").GetVersion(), 16);
	TEST_EQ(Processor("620080001611562C8802118E34").GetVersion(), 12);
	TEST_EQ(Processor("C0015000016115A2E0802F182340").GetVersion(), 23);
	TEST_EQ(Processor(GC_File::ReadAllText("AOC_Day16Test.txt")).GetVersion(), 31);
	TEST_EQ(Processor(GC_File::ReadAllText("AOC_Day16Part1.txt")).GetVersion(), 996);
}

DEFINE_TEST_G(Part2, Day16)
{
	TEST_EQ(Processor("CE00C43D881120").Run(), 9);
	TEST_EQ(Processor("9C0141080250320F1802104A08").Run(), 1);
	TEST_EQ(Processor(GC_File::ReadAllText("AOC_Day16Part1.txt")).Run(), 96257984154);
}
