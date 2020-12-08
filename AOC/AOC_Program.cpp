#include "AOC_Precompiled.h"
#include "AOC_Program.h"

void Program::Load(char const* aFile)
{
	auto lines = GC_File::ReadAllLines(aFile);

	Clear();
	Reserve(lines.Count());

	for (auto line : lines)
	{
		GC_StrSlice sl;
		GC_Strtok(line, " ", sl);

		Instruction& ins = Add();

		ins.exec = 0;
		ins.op = Op::FromString(GC_String(sl));

		GC_Strtok(line, " ", sl);
		if (sl[0] == '+')
			sl = sl.Range(1);

		ins.val = GC_Atoi(sl);
	}
}
bool Program::Step(CPU& state) const
{
	Instruction const& ins = (*this)[state.Exec];
	++ins.exec;

	switch (ins.op)
	{
	case Op::acc:
		state.Accum += ins.val;
		++state.Exec;
		break;
	case Op::jmp:
		state.Exec += ins.val;
		break;
	default:
		++state.Exec;
		break;
	}

	return state.Exec < Count();
}

void Program::ResetCounters() const
{
	for (Instruction const& ins : *this)
		ins.exec = 0;
}
