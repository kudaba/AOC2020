#include "AOC_Precompiled.h"
#include "AOC_Day8.h"
#include "AOC_Program.h"

static uint locDay8Part1(char const* aFile)
{
	Program prog;
	prog.Load(aFile);

	CPU state;
	while (prog[state.Exec].exec != 1 && prog.Step(state));

	return state.Accum;
}

DEFINE_TEST_G(Part1, Day8)
{
	TEST_EQ(locDay8Part1("AOC_Day8Test.txt"), 5);
	TEST_EQ(locDay8Part1("AOC_Day8Part1.txt"), 1384);
}

static uint locDay8Part2(char const* aFile)
{
	Program prog;
	prog.Load(aFile);

	uint swap = prog.Count();
	while (1)
	{
		CPU state;
		while (prog[state.Exec].exec != 1 && prog.Step(state));

		if (state.Exec >= prog.Count())
			return state.Accum;

		prog.ResetCounters();

		if (swap < prog.Count())
		{
			if (prog[swap].op == Op::jmp)
				prog[swap].op = Op::nop;
			else
				prog[swap].op = Op::jmp;
		}

		while (prog[--swap].op == Op::acc);

		if (prog[swap].op == Op::jmp)
			prog[swap].op = Op::nop;
		else
			prog[swap].op = Op::jmp;
	}

	return 0;
}

DEFINE_TEST_G(Part2, Day8)
{
	TEST_EQ(locDay8Part2("AOC_Day8Test.txt"), 8);
	TEST_EQ(locDay8Part2("AOC_Day8Part1.txt"), 761);
}
