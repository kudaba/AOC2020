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
		uint64 elapsed = myTimer.GetElapsed();
		Printf(GC_Strfmt<256>("{}/{} Completed in {}.\n", TestGroup(), TestName(), GC_StringPrinter::TimeFromHighRes(elapsed)));
		theHighResTime += elapsed;
	}
private:
	GC_HighResTimer myTimer;
public:
	static uint64 theHighResTime;
};


constexpr void for_range2d_next(uint& x, uint& y, uint maxx)
{
	++x;
	bool const xmax = x == maxx;
	y += xmax;
	x = x * !xmax;
}
#define for_range2d_v(x, y, maxx, maxy) for (uint x = 0, y = 0; x < maxx && y < maxy; for_range2d_next(x, y, maxx))
#define for_range2d(maxx, maxy) for_range2d_v(x, y, maxx, maxy)

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
	constexpr uint64 SumOfRange(uint64 aValue) { return aValue * (aValue + 1) / 2; }

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
//-------------------------------------------------------------------------------------------------
// Incorporate specializations for float/double vs for integers
//-------------------------------------------------------------------------------------------------
template<>
constexpr GC_Vector2<u64> GC_Vector2<u64>::operator/(u64 aScalar) const
{
	return GC_Vector2(x / aScalar, y / aScalar);
}

//-------------------------------------------------------------------------------------------------
template <typename T>
struct GC_Cube
{
	GC_Vector3<T> Min;
	GC_Vector3<T> Max;

	bool operator==(GC_Cube const& anOther) const { return Min == anOther.Min && Max == anOther.Max; }
	bool operator!=(GC_Cube const& anOther) const { return Min != anOther.Min || Max != anOther.Max; }

	T Volume() const
	{
		auto volume = Max - Min;
		return volume.x * volume.y * volume.z;
	}
	T VolumeInclusive() const
	{
		auto volume = Max - Min + GC_Vector3i(1);
		return volume.x * volume.y * volume.z;
	}

	bool Overlaps(GC_Cube const& anOther)
	{
		return anOther.Max > Min && anOther.Min < Max;
	}
	bool Overlaps(GC_Cube const& anOther, GC_Cube& anOverlappingRegion)
	{
		if (anOther.Max >= Min && anOther.Min <= Max)
		{
			anOverlappingRegion = { GC_Max(Min, anOther.Min), GC_Min(Max, anOther.Max) };
			return true;
		}

		return false;
	}

	GC_HybridArray<GC_Cube, 6> Split(GC_Cube const& anOther)
	{
		GC_HybridArray<GC_Cube, 6> newCubes;

		GC_Cube overlap;
		if (Overlaps(anOther, overlap))
		{
			if (overlap != *this)
			{
				GC_Cube newCube = *this;

				for_range(3)
				{
					if (overlap.Min[i] > Min[i])
					{
						newCube.Min[i] = Min[i];
						newCube.Max[i] = overlap.Min[i] - 1;
						newCubes.Add(newCube);
					}
					if (overlap.Max[i] < Max[i])
					{
						newCube.Min[i] = overlap.Max[i] + 1;
						newCube.Max[i] = Max[i];
						newCubes.Add(newCube);
					}

					newCube.Min[i] = overlap.Min[i];
					newCube.Max[i] = overlap.Max[i];
				}
			}
			else
			{
				if (newCubes.Count() > 6)
					newCubes.PopBack();
			}
		}
		else
			newCubes.Add(*this);

		return newCubes;
	}
};
