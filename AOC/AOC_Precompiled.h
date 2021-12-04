#pragma once

#include "GC_Include.h"
#include "GC_Mutex.h"
#include "GC_UnitTest.h"
#include "GC_DynamicArray2D.h"

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
