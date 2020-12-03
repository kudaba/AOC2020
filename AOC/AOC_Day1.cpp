#include "AOC_Precompiled.h"
#include "AOC_Day1.h"

template <uint Count>
uint locGetProduct(uint(&data)[Count])
{
	for_index(uint v : data)
		for (uint j = i+1; j < Count; ++j)
			if (v + data[j] == 2020)
				return v*data[j];

	return 0;
}

DEFINE_TEST_G(Part1, Day1)
{
	TEST_EQ(locGetProduct(theTestData), 514579);
	TEST_EQ(locGetProduct(thePart1Data), 982464);
}

template <uint Count>
uint locGet3Product(uint(&data)[Count])
{
	for_index(uint v : data)
		for (uint j = i+1; j < Count; ++j)
			if (uint s = v + data[j]; s < 2020)
				for (uint k = j + 1; k < Count; ++k)
					if (s + data[k] == 2020)
						return v * data[j] * data[k];

	return 0;
}

DEFINE_TEST_G(Part2, Day1)
{
	TEST_EQ(locGet3Product(theTestData), 241861950);
	TEST_EQ(locGet3Product(thePart1Data), 162292410);
}
