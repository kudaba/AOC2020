#pragma once

#include "GC_Include.h"
#include "GC_Mutex.h"
#include "GC_UnitTest.h"
#include "GC_DynamicArray2D.h"
#include "GC_CardinalDirections.h"
#include "GC_BitVector.h"

constexpr void for_range2d_next(uint& x, uint& y, uint maxx)
{
	++x;
	bool const xmax = x == maxx;
	y += xmax;
	x = x * !xmax;
}
#define for_range2d(maxx, maxy) for (uint x = 0, y = 0; x < maxx && y < maxy; for_range2d_next(x, y, maxx))

// Todo; add to graphium
namespace GC_File
{
	template <typename T>
	GC_DynamicArray<T> Parse(char const* aFile, GC_Function<bool (GC_StrSlice, T&)> aReadLine)
	{
		auto lines = GC_File::ReadAllLines(aFile);
		GC_DynamicArray<T> data;
		data.Reserve(lines.Count());

		for (auto line : lines)
			if (!aReadLine(line, data.Add()))
				data.PopBack();

		return data;
	}

	template <typename T>
	GC_DynamicArray<T> Parse(char const* aFile, GC_Function<T (GC_StrSlice)> aReadLine)
	{
		auto lines = GC_File::ReadAllLines(aFile);
		GC_DynamicArray<T> data;
		data.Reserve(lines.Count());

		for (auto line : lines)
			data.Add(aReadLine(line));

		return data;
	}

	template <typename T>
	GC_DynamicArray2D<T> Parse2d(char const* aFile, GC_Function<T (char c)> aRead)
	{
		auto lines = GC_File::ReadAllLines(aFile);
		GC_DynamicArray2D<T> data;
		data.SetSize({ lines[0].Count(), lines.Count() });

		for_index_v(y, GC_StrSlice const& l : lines)
			for_index_v(x, char c : l)
			data(x, y) = aRead(c);
		return data;
	}

	template <typename T>
	GC_DynamicArray2D<T> Parse2d(char const* aFile, char const* aSeparator, GC_Function<T (GC_StrSlice)> aRead)
	{
		auto lines = GC_File::ReadAllLines(aFile);
		GC_DynamicArray2D<T> data;
		data.SetSize({ lines[0].Count(), lines.Count() });

		for_index_v(y, GC_StrSlice const& l : lines)
		{
			GC_StrSlice p;
			uint x;
			while (GC_Strtok(l, aSeparator, p))
				data(x++, y) = aRead(p);
		}
		return data;
	}

	inline GC_String ReadAllText(char const* aFile)
	{
		GC_String text;
		GC_File::ReadAllText(aFile, text);
		return text;
	}
}

namespace GC_Algorithm
{
	template <typename T>
	T Sum(GC_ArrayRange<T> anInput)
	{
		T result = {};
		for (T const& i : anInput)
			result += i;
		return result;
	}

	template <typename Type, typename Predicate>
	void FloodFillRecursive(GC_ArrayRange2D<Type> const& aGrid, GC_BitVector& aVisited, GC_Vector2u aPosition, Predicate const& aPredicate)
	{
		uint const index = aPosition.y * aGrid.Width() + aPosition.x;
		if (aVisited.GetValue(index))
			return;

		aVisited.SetValue(index, true);

		if (aPredicate(aGrid(aPosition)))
		{
			if (aPosition.x > 0) FloodFillRecursive(aGrid, aVisited, aPosition - GC_Vector2i(1, 0), aPredicate);
			if (aPosition.y > 0) FloodFillRecursive(aGrid, aVisited, aPosition - GC_Vector2i(0, 1), aPredicate);
			if (aPosition.x < aGrid.Width() - 1) FloodFillRecursive(aGrid, aVisited, aPosition + GC_Vector2i(1, 0), aPredicate);
			if (aPosition.y < aGrid.Height() - 1) FloodFillRecursive(aGrid, aVisited, aPosition + GC_Vector2i(0, 1), aPredicate);
		}
	}

	template <typename Type, typename Predicate>
	void FloodFill(GC_ArrayRange2D<Type> const& aGrid, GC_Vector2u aStart, Predicate const& aPredicate)
	{
		GC_BitVector visited;
		visited.Resize(aGrid.Count());
		FloodFillRecursive(aGrid, visited, aStart, aPredicate);
	}
}

inline GC_DynamicArray<GC_StrSlice> GC_StrSplit(char const* aString, char const* aSeparator, bool anIncludeEmpty = false)
{
	GC_DynamicArray<GC_StrSlice> parts;

	GC_StrSlice part;
	while (GC_Strtok(aString, aSeparator, part, anIncludeEmpty))
		parts.Add(part);

	return parts;
}

inline GC_DynamicArray<GC_StrSlice> GC_StrSplit(GC_StrSlice aString, char const* aSeparator, bool anIncludeEmpty = false)
{
	GC_DynamicArray<GC_StrSlice> parts;

	GC_StrSlice part;
	while (GC_Strtok(aString, aSeparator, part, anIncludeEmpty))
		parts.Add(part);

	return parts;
}


//-------------------------------------------------------------------------------------------------
template<typename Type> inline
constexpr GC_Vector2<Type> operator*(Type aScalar, const GC_Vector2<Type>& anOther)
{
	return GC_Vector2(Type(anOther.x * aScalar), Type(anOther.y * aScalar));
}
//-------------------------------------------------------------------------------------------------
template<typename Type>
constexpr GC_Vector3<Type> operator*(Type aScalar, const GC_Vector3<Type>& anOther)
{
	return GC_Vector3(Type(anOther.x * aScalar), Type(anOther.y * aScalar), Type(anOther.z * aScalar));
}
//-------------------------------------------------------------------------------------------------
template<typename Type> inline
constexpr GC_Vector4<Type> operator*(Type aScalar, const GC_Vector4<Type>& anOther)
{
	return GC_Vector4(Type(anOther.x * aScalar), Type(anOther.y * aScalar), Type(anOther.z * aScalar), Type(anOther.w * aScalar));
}
