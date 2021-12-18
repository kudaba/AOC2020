#include "AOC_Precompiled.h"
#include "GC_ObjectPool.h"

struct Data
{
	union
	{
		Data* Left;
		uintptr_t LeftData;
	};
	union
	{
		Data* Right;
		uintptr_t RightData;
	};

	void Init(char const*& data);
	Data* Add(Data* anOther);
	bool Explode(uint aDepth, GC_Optional<uintptr_t> &aRemainderLeft, GC_Optional<uintptr_t> &aRemainderRight);
	bool Split();
	uint64 Magnitude();
	Data* Clone();
};

GC_ObjectPool<Data>* theDataPool;

void Data::Init(char const*& data)
{
	auto readBlock = [](char const*& data) {
		if (data[0] == '[')
		{
			auto b = theDataPool->Alloc();
			b->Init(data);
			return b;
		}
		return (Data*)(uintptr_t)GC_Atoi(data++);
	};

	++data;
	Left = readBlock(data);
	++data;
	Right = readBlock(data);
	++data;
}

Data* Data::Add(Data* anOther)
{
	Data* d = theDataPool->Alloc();
	d->Left = this;
	d->Right = anOther;

	while (1)
	{
		GC_Optional<uint64> oLeft, oRight;
		if (!d->Explode(0, oLeft, oRight) && !d->Split())
			break;
	}
	return d;
}

bool Data::Explode(uint aDepth, GC_Optional<uintptr_t> &aRemainderLeft, GC_Optional<uintptr_t> &aRemainderRight)
{
	bool exploded = false;

	auto asplode = [&](Data*& data)
	{
		if (data->LeftData < 100 && data->RightData < 100 && aDepth >= 3)
		{
			aRemainderLeft = data->LeftData;
			aRemainderRight = data->RightData;
			data = 0;
			exploded = true;
		}

		if (!exploded)
		{
			exploded = data->Explode(aDepth + 1, aRemainderLeft, aRemainderRight);
		}
	};

	if (LeftData > 100)
	{
		asplode(Left);

		if (aRemainderRight.IsSet())
		{
			if (RightData < 100)
				RightData += aRemainderRight.Get();
			else
				for (Data* d = Right; ; d = d->Left)
					if (d->LeftData < 100)
					{
						d->LeftData += aRemainderRight.Get();
						break;
					}

			aRemainderRight.Reset();
		}
	}

	if (!exploded && RightData > 100)
	{
		asplode(Right);

		if (aRemainderLeft.IsSet())
		{
			if (LeftData < 100)
				LeftData += aRemainderLeft.Get();
			else
				for (Data* d = Left; ; d = d->Right)
					if (d->RightData < 100)
					{
						d->RightData += aRemainderLeft.Get();
						break;
					}

			aRemainderLeft.Reset();
		}
	}

	return exploded;
}

bool Data::Split()
{
	auto split = [](Data*& data)
	{
		auto const d = (uintptr_t)data;
		if (d >= 10 && d < 100)
		{
			Data* nd = theDataPool->Alloc();
			nd->LeftData = d / 2;
			nd->RightData = (d + 1) / 2;
			data = nd;
			return true;
		}
		return (d > 100 && data->Split());
	};

	return split(Left) || split(Right);
}

uint64 Data::Magnitude()
{
	uint64 const l = LeftData < 100 ? LeftData : Left->Magnitude();
	uint64 const r = RightData < 100 ? RightData : Right->Magnitude();
	return l * 3 + r * 2;
}

Data* Data::Clone()
{
	Data* nd = theDataPool->Alloc();
	nd->Left = LeftData < 100 ? Left : Left->Clone();
	nd->Right = RightData < 100 ? Right : Right->Clone();
	return nd;
}

static auto locParseData(char const* aFile)
{
	// By line with parse function
	return GC_File::Parse<Data*>(aFile, [](auto aLine)
		{
			auto c = aLine.Buffer();
			Data* d = theDataPool->Alloc();
			d->Init(c);
			return d;
		});
}

static auto locPart1(char const* aFile)
{
	GC_ObjectPool<Data> dataPool;
	theDataPool = &dataPool;

	auto data = locParseData(aFile);
	Data* final = data[0];
	for_range(data.Count())
		if (i)
			final = final->Add(data[i]);

	return final->Magnitude();
}

DEFINE_TEST_G(Part1, Day18)
{
	TEST_EQ(locPart1("AOC_Day18Test.txt"), 4140);
	TEST_EQ(locPart1("AOC_Day18Part1.txt"), 4202);
}

static auto locPart2(char const* aFile)
{
	GC_ObjectPool<Data> dataPool;
	theDataPool = &dataPool;

	auto data = locParseData(aFile);

	uint64 max = 0;
	for (uint i = 0, ie = data.Count() - 1; i < ie; ++i)
		for (uint j = i + 1, je = data.Count(); j < je; ++j)
		{
			uint64 mag = data[i]->Clone()->Add(data[j]->Clone())->Magnitude();
			max = GC_Max(max, mag);
			mag = data[j]->Clone()->Add(data[i]->Clone())->Magnitude();
			max = GC_Max(max, mag);
		}

	return max;

}

DEFINE_TEST_G(Part2, Day18)
{
	TEST_EQ(locPart2("AOC_Day18Test.txt"), 3993);
	TEST_EQ(locPart2("AOC_Day18Part1.txt"), 4779);
}
