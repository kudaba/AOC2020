#pragma once

#include "GC_Include.h"
#include "GC_Mutex.h"
#include "GC_UnitTest.h"
#include "GC_DynamicArray2D.h"

// Todo; add to graphium
namespace GC_File
{
	template <typename T>
	GC_DynamicArray<T> Parse(char const* aFile, GC_Func<bool (GC_StrSlice, T&)> aReadLine)
	{
		auto lines = GC_File::ReadAllLines(aFile);
		GC_DynamicArray<T> data;
		data.Reserve(lines.Count());

		for (auto line : lines)
			if (!aReadLine(line, data.Add()))
				data.PopBack();

		return data;
	}
}
