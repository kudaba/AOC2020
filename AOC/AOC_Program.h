#pragma once

GC_DECLARE_ENUM(Op, nop, acc, jmp);

struct CPU
{
	uint Exec = 0;
	int Accum = 0;
};

struct Instruction
{
	Op op;
	int val;
	mutable int exec;
};

struct Program : GC_DynamicArray<Instruction>
{
	void Load(char const* aFile);
	bool Step(CPU& state) const;
	void ResetCounters() const;
};
