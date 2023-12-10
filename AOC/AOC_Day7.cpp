#include "AOC_Precompiled.h"

struct Hand
{
	enum HandType
	{
		High,
		One,
		Two,
		Three,
		Full,
		Fourer,
		Fiver,
	};

	char Cards[5];
	uint Bid;
	HandType Type;
	int CardValues[5];

	void MakeType()
	{
		static const char CardLookup[] =
		{
			'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'
		};

		GC_HashMap<uint, uint> CardCounts;
		for_index(char c : Cards)
		{
			CardValues[i] = GC_Algorithm::IndexOf(CardLookup, c);
			++CardCounts.GetOrAdd(CardValues[i], 0);
		}

		switch (CardCounts.Count())
		{
		case 1: Type = Fiver; break;
		case 2:
			switch (CardCounts.First().Value)
			{
			case 1:
			case 4:
				Type = Fourer; break;
			case 2:
			case 3:
				Type = Full; break;
			}
			break;
		case 3:
			switch (GC_Algorithm::Max(CardCounts, [](auto iter) { return iter.Value; }))
			{
			case 3:
				Type = Three;
				break;
			case 2:
				Type = Two;
				break;
			}
			break;
		case 4:
			Type = One; break;
		case 5:
			Type = High; break;
		}
	}

	bool operator<(Hand const& anOther) const
	{
		if (Type != anOther.Type)
		{
			return Type < anOther.Type;
		}
		for_range(5)
		{
			if (CardValues[i] != anOther.CardValues[i])
				return CardValues[i] < anOther.CardValues[i];
		}
		return this < &anOther;
	}
};

static auto locPart1(char const* aFile)
{
	auto hands = GC_File::Parse<Hand>(aFile, [](auto aLine)
		{
			auto parts = GC_StrSplit<2>(aLine, ' ');

			Hand h;
			GC_Memcpy(h.Cards, parts[0].Buffer(), 5);
			h.Bid = GC_Atoi(parts[1]);
			h.MakeType();
			return h;
		});

	hands.Sort();

	uint64 result = 0;

	for_index(auto hand : hands)
	{
		result += hand.Bid * (i + 1);
	}

	return result;
}

DEFINE_TEST_G(Part1, Day7)
{
	TEST_EQ(locPart1("AOC_Day7Test.txt"), 6440);
	TEST_EQ(locPart1("AOC_Day7Part1.txt"), 253313241);
}

struct HandWild
{
	enum HandType
	{
		High,
		One,
		Two,
		Three,
		Full,
		Fourer,
		Fiver,
	};

	char Cards[5];
	uint Bid;
	HandType Type;
	int CardValues[5];

	void MakeType()
	{
		static const char CardLookup[] =
		{
			'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'
		};

		GC_HashMap<uint, uint> CardCounts;
		for_index(char c : Cards)
		{
			CardValues[i] = GC_Algorithm::IndexOf(CardLookup, c);
			++CardCounts.GetOrAdd(CardValues[i], 0);
		}

		if (CardCounts.Contains(0) && CardCounts.Count() > 1)
		{
			uint Count = CardCounts[0];
			CardCounts.Remove(0);

			uint index = 0;
			uint max = 0;
			for_index(auto iter : CardCounts)
			{
				if (iter.Value > max)
				{
					max = iter.Value;
					index = i;
				}
			}
			
			CardCounts.GetByIndex(index) += Count;
		}

		switch (CardCounts.Count())
		{
		case 1: Type = Fiver; break;
		case 2:
			switch (CardCounts.First().Value)
			{
			case 1:
			case 4:
				Type = Fourer; break;
			case 2:
			case 3:
				Type = Full; break;
			}
			break;
		case 3:
			switch (GC_Algorithm::Max(CardCounts, [](auto iter) { return iter.Value; }))
			{
			case 3:
				Type = Three;
				break;
			case 2:
				Type = Two;
				break;
			}
			break;
		case 4:
			Type = One; break;
		case 5:
			Type = High; break;
		}
	}

	bool operator<(HandWild const& anOther) const
	{
		if (Type != anOther.Type)
		{
			return Type < anOther.Type;
		}
		for_range(5)
		{
			if (CardValues[i] != anOther.CardValues[i])
				return CardValues[i] < anOther.CardValues[i];
		}
		return this < &anOther;
	}
};

static auto locPart2(char const* aFile)
{
	auto hands = GC_File::Parse<HandWild>(aFile, [](auto aLine)
		{
			auto parts = GC_StrSplit<2>(aLine, ' ');

			HandWild h;
			GC_Memcpy(h.Cards, parts[0].Buffer(), 5);
			h.Bid = GC_Atoi(parts[1]);
			h.MakeType();
			return h;
		});

	hands.Sort();

	uint64 result = 0;

	for_index(auto hand : hands)
	{
		result += hand.Bid * (i + 1);
	}

	return result;
}

DEFINE_TEST_G(Part2, Day7)
{
	TEST_EQ(locPart2("AOC_Day7Test.txt"), 5905);
	TEST_EQ(locPart2("AOC_Day7Part1.txt"), 253362743);
}
