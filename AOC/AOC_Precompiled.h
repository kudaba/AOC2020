#pragma once

#define BASE_FIXTURE AOC_TestFixture

#include "GC_Include.h"
#include "GC_Mutex.h"
#include "GC_UnitTest.h"
#include "GC_DynamicArray2D.h"
#include "GC_CardinalDirections.h"
#include "GC_BitVector.h"
#include "GC_StringPrinter.h"

class AOC_TestFixture : public GC_TestFixture
{
protected:
	void OnSetup() override { myTimer.Reset(); }
	void OnTearDown() override
	{
		Printf(GC_Strfmt<256>("{}/{} Completed in {}.\n", TestGroup(), TestName(), GC_StringPrinter::TimeFromHighRes(myTimer.GetElapsed())));
	}
private:
	GC_HighResTimer myTimer;
};


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
	auto Sum(T const& anInput)
	{
		GC_TypeNoConstNoReference<decltype(anInput[0])> result = {};
		for (auto const& i : anInput)
			result += i;
		return result;
	}

	template <typename T, typename P>
	auto Sum(T const& anInput, P aPredicate)
	{
		GC_TypeNoConstNoReference<decltype(aPredicate(anInput[0]))> result = {};
		for (auto const& i : anInput)
			result += aPredicate(i);
		return result;
	}

	template <typename T>
	auto Product(T const& anInput)
	{
		GC_TypeNoConstNoReference<decltype(anInput[0])> result = 1;
		for (auto const& i : anInput)
			result *= i;
		return result;
	}

	template <typename T>
	auto Min(T const& anInput)
	{
		auto result = anInput[0];
		for (auto const& i : anInput)
			result = GC_Min(result, i);
		return result;
	}
	template <typename T>
	auto Max(T const& anInput)
	{
		auto result = anInput[0];
		for (auto const& i : anInput)
			result = GC_Max(result, i);
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

	template <typename Type>
	void Resize(GC_DynamicArray2D<Type>& anArray, GC_Vector2u aSize, Type const& aDefaultValue)
	{
		GC_Vector2u const ogSize = anArray.Size();
		anArray.SetSize(aSize);

		if (aSize.x > ogSize.x)
		{
			uint const count = aSize.x - ogSize.x;
			for_range_v(y, GC_Min(ogSize.y, aSize.y))
				GC_ArraySet(anArray.Row(y).Right(count).Buffer(), aDefaultValue, count);
		}

		if (aSize.y > ogSize.y)
		{
			GC_ArraySet(anArray.Row(ogSize.y).Buffer(), aDefaultValue, (aSize.y - ogSize.y) * aSize.x);
		}
	}
}

template <uint Size>
struct GC_StrSplitType
{
	typedef GC_HybridArray<GC_StrSlice, Size> Parts;
};
template <>
struct GC_StrSplitType<0>
{
	typedef GC_DynamicArray<GC_StrSlice> Parts;
};

template <uint Size = 0, typename CharIn = GC_StrSlice, typename CharSep = char const*>
inline auto GC_StrSplit(CharIn const& aString, CharSep const& aSeparator, bool anIncludeEmpty = false)
{
	typename GC_StrSplitType<Size>::Parts parts;
	for (GC_StrSlice part; GC_Strtok(aString, aSeparator, part, anIncludeEmpty);) parts.Add(part);
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
