// This needed to be SolidAngleString.h to avoid conflicting with
// the Windows platform SDK string.h

#pragma once

#include "CoreTypes.h"
#include "Misc/VarArgs.h"
#include "Misc/OutputDevice.h"
#include "Misc/AssertionMacros.h"
#include "HAL/SolidAngleMemory.h"
#include "Templates/IsArithmetic.h"
#include "Templates/SolidAngleTypeTraits.h"
#include "Templates/SolidAngleTemplate.h"
#include "Math/NumericLimits.h"
#include "Containers/Array.h"
#include "Misc/CString.h"
#include "Misc/Crc.h"
#include "Math/SolidAngleMathUtility.h"

struct YStringFormatArg;
template<typename KeyType, typename ValueType, typename SetAllocator, typename KeyFuncs > class TMap;

/** Determines case sensitivity options for string comparisons. */
namespace ESearchCase
{
	enum Type
	{
		/** Case sensitive. Upper/lower casing must match for strings to be considered equal. */
		CaseSensitive,

		/** Ignore case. Upper/lower casing does not matter when making a comparison. */
		IgnoreCase,
	};
};

/** Determines search direction for string operations. */
namespace ESearchDir
{
	enum Type
	{
		/** Search from the start, moving forward through the string. */
		FromStart,

		/** Search from the end, moving backward through the string. */
		FromEnd,
	};
}

/**
* A dynamically sizeable string.
* @see https://docs.unrealengine.com/latest/INT/Programming/UnrealArchitecture/StringHandling/YString/
*/
class CORE_API YString
{
private:
	friend struct TContainerTraits<YString>;

	/** Array holding the character data */
	typedef TArray<TCHAR> DataType;
	DataType Data;

public:
	using ElementType = TCHAR;

#if PLATFORM_COMPILER_HAS_DEFAULTED_FUNCTIONS

	YString() = default;
	YString(YString&&) = default;
	YString(const YString&) = default;
	YString& operator=(YString&&) = default;
	YString& operator=(const YString&) = default;

#else

	FORCEINLINE YString()
	{
	}

	FORCEINLINE YString(YString&& Other)
		: Data(MoveTemp(Other.Data))
	{
	}

	FORCEINLINE YString(const YString& Other)
		: Data(Other.Data)
	{
	}

	FORCEINLINE YString& operator=(YString&& Other)
	{
		Data = MoveTemp(Other.Data);
		return *this;
	}

	FORCEINLINE YString& operator=(const YString& Other)
	{
		Data = Other.Data;
		return *this;
	}

#endif

	/**
	* Create a copy of the Other string with extra space for characters at the end of the string
	*
	* @param Other the other string to create a new copy from
	* @param ExtraSlack number of extra characters to add to the end of the other string in this string
	*/
	FORCEINLINE YString(const YString& Other, int32 ExtraSlack)
		: Data(Other.Data, ExtraSlack + ((Other.Data.Num() || !ExtraSlack) ? 0 : 1)) // Add 1 if the source string array is empty and we want some slack, because we'll need to include a null terminator which is currently missing
	{
	}

	/**
	* Create a copy of the Other string with extra space for characters at the end of the string
	*
	* @param Other the other string to create a new copy from
	* @param ExtraSlack number of extra characters to add to the end of the other string in this string
	*/
	FORCEINLINE YString(YString&& Other, int32 ExtraSlack)
		: Data(MoveTemp(Other.Data), ExtraSlack + ((Other.Data.Num() || !ExtraSlack) ? 0 : 1)) // Add 1 if the source string array is empty and we want some slack, because we'll need to include a null terminator which is currently missing
	{
	}

	/**
	* Constructor using an array of TCHAR
	*
	* @param In array of TCHAR
	*/
	template <typename CharType>
	FORCEINLINE YString(const CharType* Src, typename TEnableIf<TIsCharType<CharType>::Value>::Type* Dummy = nullptr) // This TEnableIf is to ensure we don't instantiate this constructor for non-char types, like id* in Obj-C
	{
		if (Src && *Src)
		{
			int32 SrcLen = TCString<CharType>::Strlen(Src) + 1;
			int32 DestLen = FPlatformString::ConvertedLength<TCHAR>(Src, SrcLen);
			Data.AddUninitialized(DestLen);

			FPlatformString::Convert(Data.GetData(), DestLen, Src, SrcLen);
		}
	}

	/**
	* Constructor to create YString with specified number of characters from another string with additional character zero
	*
	* @param InCount how many characters to copy
	* @param InSrc String to copy from
	*/
	FORCEINLINE explicit YString(int32 InCount, const TCHAR* InSrc)
	{
		Data.AddUninitialized(InCount ? InCount + 1 : 0);

		if (Data.Num() > 0)
		{
			FCString::Strncpy(Data.GetData(), InSrc, InCount + 1);
		}
	}

#ifdef __OBJC__
	/** Convert Objective-C NSString* to YString */
	FORCEINLINE YString(const NSString* In)
	{
		if (In &&[In length] > 0)
		{
			// Convert the NSString data into the native TCHAR format for UE4
			// This returns a buffer of bytes, but they can be safely cast to a buffer of TCHARs
#if PLATFORM_TCHAR_IS_4_BYTES
			const CYStringEncoding Encoding = kCYStringEncodingUTF32LE;
#else
			const CYStringEncoding Encoding = kCYStringEncodingUTF16LE;
#endif

			CFRange Range = CFRangeMake(0, CYStringGetLength((__bridge CYStringRef)In));
			CFIndex BytesNeeded;
			if (CYStringGetBytes((__bridge CYStringRef)In, Range, Encoding, '?', false, NULL, 0, &BytesNeeded) > 0)
			{
				const size_t Length = BytesNeeded / sizeof(TCHAR);
				Data.AddUninitialized(Length + 1);
				CYStringGetBytes((__bridge CYStringRef)In, Range, Encoding, '?', false, (uint8*)Data.GetData(), Length * sizeof(TCHAR) + 1, NULL);
				Data[Length] = 0;
			}
		}
	}
#endif

	/**
	* Copy Assignment from array of TCHAR
	*
	* @param Other array of TCHAR
	*/
	FORCEINLINE YString& operator=(const TCHAR* Other)
	{
		if (Data.GetData() != Other)
		{
			int32 Len = (Other && *Other) ? FCString::Strlen(Other) + 1 : 0;
			Data.Empty(Len);
			Data.AddUninitialized(Len);

			if (Len)
			{
				YMemory::Memcpy(Data.GetData(), Other, Len * sizeof(TCHAR));
			}
		}
		return *this;
	}

	/**
	* Return specific character from this string
	*
	* @param Index into string
	* @return Character at Index
	*/
	FORCEINLINE TCHAR& operator[](int32 Index)
	{
		checkf(IsValidIndex(Index), TEXT("String index out of bounds: Index %i from a string with a length of %i"), Index, Len());
		return Data.GetData()[Index];
	}

	/**
	* Return specific const character from this string
	*
	* @param Index into string
	* @return const Character at Index
	*/
	FORCEINLINE const TCHAR& operator[](int32 Index) const
	{
		checkf(IsValidIndex(Index), TEXT("String index out of bounds: Index %i from a string with a length of %i"), Index, Len());
		return Data.GetData()[Index];
	}

	/**
	* Iterator typedefs
	*/
	typedef TArray<TCHAR>::TIterator      TIterator;
	typedef TArray<TCHAR>::TConstIterator TConstIterator;

	/** Creates an iterator for the characters in this string */
	FORCEINLINE TIterator CreateIterator()
	{
		return Data.CreateIterator();
	}

	/** Creates a const iterator for the characters in this string */
	FORCEINLINE TConstIterator CreateConstIterator() const
	{
		return Data.CreateConstIterator();
	}

private:
	/**
	* DO NOT USE DIRECTLY
	* STL-like iterators to enable range-based for loop support.
	*/
	FORCEINLINE friend DataType::RangedForIteratorType      begin(YString& Str) { auto Result = begin(Str.Data);                                   return Result; }
	FORCEINLINE friend DataType::RangedForConstIteratorType begin(const YString& Str) { auto Result = begin(Str.Data);                                   return Result; }
	FORCEINLINE friend DataType::RangedForIteratorType      end(YString& Str) { auto Result = end(Str.Data); if (Str.Data.Num()) { --Result; } return Result; }
	FORCEINLINE friend DataType::RangedForConstIteratorType end(const YString& Str) { auto Result = end(Str.Data); if (Str.Data.Num()) { --Result; } return Result; }

public:
	FORCEINLINE uint32 GetAllocatedSize() const
	{
		return Data.GetAllocatedSize();
	}

	/**
	* Run slow checks on this string
	*/
	FORCEINLINE void CheckInvariants() const
	{
		int32 Num = Data.Num();
		checkSlow(Num >= 0);
		checkSlow(!Num || !Data.GetData()[Num - 1]);
		checkSlow(Data.GetSlack() >= 0);
	}

	/**
	* Create empty string of given size with zero terminating character
	*
	* @param Slack length of empty string to create
	*/
	FORCEINLINE void Empty(int32 Slack = 0)
	{
		Data.Empty(Slack);
	}

	/**
	* Test whether this string is empty
	*
	* @return true if this string is empty, otherwise return false.
	*/
	FORCEINLINE bool IsEmpty() const
	{
		return Data.Num() <= 1;
	}

	/**
	* Empties the string, but doesn't change memory allocation, unless the new size is larger than the current string.
	*
	* @param NewReservedSize The expected usage size (in characters, not including the terminator) after calling this function.
	*/
	FORCEINLINE void Reset(int32 NewReservedSize = 0)
	{
		const int32 NewSizeIncludingTerminator = (NewReservedSize > 0) ? (NewReservedSize + 1) : 0;
		Data.Reset(NewSizeIncludingTerminator);
	}

	/**
	* Remove unallocated empty character space from the end of this string
	*/
	FORCEINLINE void Shrink()
	{
		Data.Shrink();
	}

	/**
	* Tests if index is valid, i.e. greater than or equal to zero, and less than the number of characters in this string (excluding the null terminator).
	*
	* @param Index Index to test.
	*
	* @returns True if index is valid. False otherwise.
	*/
	FORCEINLINE bool IsValidIndex(int32 Index) const
	{
		return Index >= 0 && Index < Len();
	}

	/**
	* Get pointer to the string
	*
	* @Return Pointer to Array of TCHAR if Num, otherwise the empty string
	*/
	FORCEINLINE const TCHAR* operator*() const
	{
		return Data.Num() ? Data.GetData() : TEXT("");
	}

	/**
	*Get string as array of TCHARS
	*
	* @warning: Operations on the TArray<*CHAR> can be unsafe, such as adding
	*		non-terminating 0's or removing the terminating zero.
	*/
	FORCEINLINE DataType& GetCharArray()
	{
		return Data;
	}

	/** Get string as const array of TCHARS */
	FORCEINLINE const DataType& GetCharArray() const
	{
		return Data;
	}

#ifdef __OBJC__
	/** Convert YString to Objective-C NSString */
	FORCEINLINE NSString* GetNSString() const
	{
#if PLATFORM_TCHAR_IS_4_BYTES
		return[[[NSString alloc] initWithBytes:Data.GetData() length : Len() * sizeof(TCHAR) encoding : NSUTF32LittleEndianStringEncoding] autorelease];
#else
		return[[[NSString alloc] initWithBytes:Data.GetData() length : Len() * sizeof(TCHAR) encoding : NSUTF16LittleEndianStringEncoding] autorelease];
#endif
	}
#endif

	/**
	* Appends an array of characters to the string.
	*
	* @param Array A pointer to the start of an array of characters to append.  This array need not be null-terminated, and null characters are not treated specially.
	* @param Count The number of characters to copy from Array.
	*/
	FORCEINLINE void AppendChars(const TCHAR* Array, int32 Count)
	{
		check(Count >= 0);

		if (!Count)
			return;

		checkSlow(Array);

		int32 Index = Data.Num();

		// Reserve enough space - including an extra gap for a null terminator if we don't already have a string allocated
		Data.AddUninitialized(Count + (Index ? 0 : 1));

		TCHAR* EndPtr = Data.GetData() + Index - (Index ? 1 : 0);

		// Copy characters to end of string, overwriting null terminator if we already have one
		CopyAssignItems(EndPtr, Array, Count);

		// (Re-)establish the null terminator
		*(EndPtr + Count) = 0;
	}

	/**
	* Concatenate this with given string
	*
	* @param Str array of TCHAR to be concatenated onto the end of this
	* @return reference to this
	*/
	FORCEINLINE YString& operator+=(const TCHAR* Str)
	{
		checkSlow(Str);
		CheckInvariants();

		AppendChars(Str, FCString::Strlen(Str));

		return *this;
	}

	/**
	* Concatenate this with given char
	*
	* @param inChar other Char to be concatenated onto the end of this string
	* @return reference to this
	*/
	template <typename CharType>
	FORCEINLINE typename TEnableIf<TIsCharType<CharType>::Value, YString&>::Type operator+=(CharType InChar)
	{
		CheckInvariants();

		if (InChar != 0)
		{
			// position to insert the character.  
			// At the end of the string if we have existing characters, otherwise at the 0 position
			int32 InsertIndex = (Data.Num() > 0) ? Data.Num() - 1 : 0;

			// number of characters to add.  If we don't have any existing characters, 
			// we'll need to append the terminating zero as well.
			int32 InsertCount = (Data.Num() > 0) ? 1 : 2;

			Data.AddUninitialized(InsertCount);
			Data[InsertIndex] = InChar;
			Data[InsertIndex + 1] = 0;
		}
		return *this;
	}

	/**
	* Concatenate this with given char
	*
	* @param InChar other Char to be concatenated onto the end of this string
	* @return reference to this
	*/
	FORCEINLINE YString& AppendChar(const TCHAR InChar)
	{
		*this += InChar;
		return *this;
	}

	FORCEINLINE YString& Append(const YString& Text)
	{
		*this += Text;
		return *this;
	}

	YString& Append(const TCHAR* Text, int32 Count)
	{
		CheckInvariants();

		if (Count != 0)
		{
			// position to insert the character.  
			// At the end of the string if we have existing characters, otherwise at the 0 position
			int32 InsertIndex = (Data.Num() > 0) ? Data.Num() - 1 : 0;

			// number of characters to add.  If we don't have any existing characters, 
			// we'll need to append the terminating zero as well.
			int32 FinalCount = (Data.Num() > 0) ? Count : Count + 1;

			Data.AddUninitialized(FinalCount);

			for (int32 Index = 0; Index < Count; Index++)
			{
				Data[InsertIndex + Index] = Text[Index];
			}

			Data[Data.Num() - 1] = 0;
		}
		return *this;
	}

	/**
	* Removes characters within the string.
	*
	* @param Index           The index of the first character to remove.
	* @param Count           The number of characters to remove.
	* @param bAllowShrinking Whether or not to reallocate to shrink the storage after removal.
	*/
	FORCEINLINE void RemoveAt(int32 Index, int32 Count = 1, bool bAllowShrinking = true)
	{
		Data.RemoveAt(Index, Count, bAllowShrinking);
	}

	FORCEINLINE void InsertAt(int32 Index, TCHAR Character)
	{
		if (Character != 0)
		{
			if (Data.Num() == 0)
			{
				*this += Character;
			}
			else
			{
				Data.Insert(Character, Index);
			}
		}
	}

	FORCEINLINE void InsertAt(int32 Index, const YString& Characters)
	{
		if (Characters.Len())
		{
			if (Data.Num() == 0)
			{
				*this += Characters;
			}
			else
			{
				Data.Insert(Characters.Data.GetData(), Characters.Len(), Index);
			}
		}
	}

	/**
	* Removes the text from the start of the string if it exists.
	*
	* @param InPrefix the prefix to search for at the start of the string to remove.
	* @return true if the prefix was removed, otherwise false.
	*/
	bool RemoveFromStart(const YString& InPrefix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase);

	/**
	* Removes the text from the end of the string if it exists.
	*
	* @param InSuffix the suffix to search for at the end of the string to remove.
	* @return true if the suffix was removed, otherwise false.
	*/
	bool RemoveFromEnd(const YString& InSuffix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase);

	/**
	* Concatenate this path with given path ensuring the / character is used between them
	*
	* @param Str       Pointer to an array of TCHARs (not necessarily null-terminated) to be concatenated onto the end of this.
	* @param StrLength Exact number of characters from Str to append.
	*/
	void PathAppend(const TCHAR* Str, int32 StrLength);

	/**
	* Concatenate this with given string
	*
	* @param Str other string to be concatenated onto the end of this
	* @return reference to this
	*/
	FORCEINLINE YString& operator+=(const YString& Str)
	{
		CheckInvariants();
		Str.CheckInvariants();

		AppendChars(Str.Data.GetData(), Str.Len());

		return *this;
	}

	/**
	* Concatenates an YString with a TCHAR.
	*
	* @param Lhs The YString on the left-hand-side of the expression.
	* @param Rhs The char on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	template <typename CharType>
	FORCEINLINE friend typename TEnableIf<TIsCharType<CharType>::Value, YString>::Type operator+(const YString& Lhs, CharType Rhs)
	{
		Lhs.CheckInvariants();

		YString Result(Lhs, 1);
		Result += Rhs;

		return Result;
	}

	/**
	* Concatenates an YString with a TCHAR.
	*
	* @param Lhs The YString on the left-hand-side of the expression.
	* @param Rhs The char on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	template <typename CharType>
	FORCEINLINE friend typename TEnableIf<TIsCharType<CharType>::Value, YString>::Type operator+(YString&& Lhs, CharType Rhs)
	{
		Lhs.CheckInvariants();

		YString Result(MoveTemp(Lhs), 1);
		Result += Rhs;

		return Result;
	}

private:
	template <typename LhsType, typename RhsType>
	FORCEINLINE static YString ConcatYStrings(typename TIdentity<LhsType>::Type Lhs, typename TIdentity<RhsType>::Type Rhs)
	{
		Lhs.CheckInvariants();
		Rhs.CheckInvariants();

		if (Lhs.IsEmpty())
			return MoveTemp(Rhs);

		int32 RhsLen = Rhs.Len();

		YString Result(MoveTemp(Lhs), RhsLen);
		Result.AppendChars(Rhs.Data.GetData(), RhsLen);

		return Result;
	}

	template <typename RhsType>
	FORCEINLINE static YString ConcatTCHARsToYString(const TCHAR* Lhs, typename TIdentity<RhsType>::Type Rhs)
	{
		checkSlow(Lhs);
		Rhs.CheckInvariants();

		if (!Lhs || !*Lhs)
			return MoveTemp(Rhs);

		int32 LhsLen = FCString::Strlen(Lhs);
		int32 RhsLen = Rhs.Len();

		// This is not entirely optimal, as if the Rhs is an rvalue and has enough slack space to hold Lhs, then
		// the memory could be reused here without constructing a new object.  However, until there is proof otherwise,
		// I believe this will be relatively rare and isn't worth making the code a lot more complex right now.
		YString Result;
		Result.Data.AddUninitialized(LhsLen + RhsLen + 1);

		TCHAR* ResultData = Result.Data.GetData();
		CopyAssignItems(ResultData, Lhs, LhsLen);
		CopyAssignItems(ResultData + LhsLen, Rhs.Data.GetData(), RhsLen);
		*(ResultData + LhsLen + RhsLen) = 0;

		return Result;
	}

	template <typename LhsType>
	FORCEINLINE static YString ConcatYStringToTCHARs(typename TIdentity<LhsType>::Type Lhs, const TCHAR* Rhs)
	{
		Lhs.CheckInvariants();
		checkSlow(Rhs);

		if (!Rhs || !*Rhs)
			return MoveTemp(Lhs);

		int32 RhsLen = FCString::Strlen(Rhs);

		YString Result(MoveTemp(Lhs), RhsLen);
		Result.AppendChars(Rhs, RhsLen);

		return Result;
	}

public:
	/**
	* Concatenate two YStrings.
	*
	* @param Lhs The YString on the left-hand-side of the expression.
	* @param Rhs The YString on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	FORCEINLINE friend YString operator+(const YString& Lhs, const YString& Rhs)
	{
		return ConcatYStrings<const YString&, const YString&>(Lhs, Rhs);
	}

	/**
	* Concatenate two YStrings.
	*
	* @param Lhs The YString on the left-hand-side of the expression.
	* @param Rhs The YString on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	FORCEINLINE friend YString operator+(YString&& Lhs, const YString& Rhs)
	{
		return ConcatYStrings<YString&&, const YString&>(MoveTemp(Lhs), Rhs);
	}

	/**
	* Concatenate two YStrings.
	*
	* @param Lhs The YString on the left-hand-side of the expression.
	* @param Rhs The YString on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	FORCEINLINE friend YString operator+(const YString& Lhs, YString&& Rhs)
	{
		return ConcatYStrings<const YString&, YString&&>(Lhs, MoveTemp(Rhs));
	}

	/**
	* Concatenate two YStrings.
	*
	* @param Lhs The YString on the left-hand-side of the expression.
	* @param Rhs The YString on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	FORCEINLINE friend YString operator+(YString&& Lhs, YString&& Rhs)
	{
		return ConcatYStrings<YString&&, YString&&>(MoveTemp(Lhs), MoveTemp(Rhs));
	}

	/**
	* Concatenates a TCHAR array to an YString.
	*
	* @param Lhs The TCHAR array on the left-hand-side of the expression.
	* @param Rhs The YString on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	FORCEINLINE friend YString operator+(const TCHAR* Lhs, const YString& Rhs)
	{
		return ConcatTCHARsToYString<const YString&>(Lhs, Rhs);
	}

	/**
	* Concatenates a TCHAR array to an YString.
	*
	* @param Lhs The TCHAR array on the left-hand-side of the expression.
	* @param Rhs The YString on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	FORCEINLINE friend YString operator+(const TCHAR* Lhs, YString&& Rhs)
	{
		return ConcatTCHARsToYString<YString&&>(Lhs, MoveTemp(Rhs));
	}

	/**
	* Concatenates an YString to a TCHAR array.
	*
	* @param Lhs The YString on the left-hand-side of the expression.
	* @param Rhs The TCHAR array on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	FORCEINLINE friend YString operator+(const YString& Lhs, const TCHAR* Rhs)
	{
		return ConcatYStringToTCHARs<const YString&>(Lhs, Rhs);
	}

	/**
	* Concatenates an YString to a TCHAR array.
	*
	* @param Lhs The YString on the left-hand-side of the expression.
	* @param Rhs The TCHAR array on the right-hand-side of the expression.
	*
	* @return The concatenated string.
	*/
	FORCEINLINE friend YString operator+(YString&& Lhs, const TCHAR* Rhs)
	{
		return ConcatYStringToTCHARs<YString&&>(MoveTemp(Lhs), Rhs);
	}

	/**
	* Concatenate this path with given path ensuring the / character is used between them
	*
	* @param Str path array of TCHAR to be concatenated onto the end of this
	* @return reference to path
	*/
	FORCEINLINE YString& operator/=(const TCHAR* Str)
	{
		checkSlow(Str);

		PathAppend(Str, FCString::Strlen(Str));
		return *this;
	}

	/**
	* Concatenate this path with given path ensuring the / character is used between them
	*
	* @param Str path YString to be concatenated onto the end of this
	* @return reference to path
	*/
	FORCEINLINE YString& operator/=(const YString& Str)
	{
		PathAppend(Str.Data.GetData(), Str.Len());
		return *this;
	}

	/**
	* Concatenate this path with given path ensuring the / character is used between them
	*
	* @param Lhs Path to concatenate onto.
	* @param Rhs Path to concatenate.
	* @return new YString of the path
	*/
	FORCEINLINE friend YString operator/(const YString& Lhs, const TCHAR* Rhs)
	{
		checkSlow(Rhs);

		int32 StrLength = FCString::Strlen(Rhs);

		YString Result(Lhs, StrLength + 1);
		Result.PathAppend(Rhs, StrLength);
		return Result;
	}

	/**
	* Concatenate this path with given path ensuring the / character is used between them
	*
	* @param Lhs Path to concatenate onto.
	* @param Rhs Path to concatenate.
	* @return new YString of the path
	*/
	FORCEINLINE friend YString operator/(YString&& Lhs, const TCHAR* Rhs)
	{
		checkSlow(Rhs);

		int32 StrLength = FCString::Strlen(Rhs);

		YString Result(MoveTemp(Lhs), StrLength + 1);
		Result.PathAppend(Rhs, StrLength);
		return Result;
	}

	/**
	* Concatenate this path with given path ensuring the / character is used between them
	*
	* @param Lhs Path to concatenate onto.
	* @param Rhs Path to concatenate.
	* @return new YString of the path
	*/
	FORCEINLINE friend YString operator/(const YString& Lhs, const YString& Rhs)
	{
		int32 StrLength = Rhs.Len();

		YString Result(Lhs, StrLength + 1);
		Result.PathAppend(Rhs.Data.GetData(), StrLength);
		return Result;
	}

	/**
	* Concatenate this path with given path ensuring the / character is used between them
	*
	* @param Lhs Path to concatenate onto.
	* @param Rhs Path to concatenate.
	* @return new YString of the path
	*/
	FORCEINLINE friend YString operator/(YString&& Lhs, const YString& Rhs)
	{
		int32 StrLength = Rhs.Len();

		YString Result(MoveTemp(Lhs), StrLength + 1);
		Result.PathAppend(Rhs.Data.GetData(), StrLength);
		return Result;
	}

	/**
	* Concatenate this path with given path ensuring the / character is used between them
	*
	* @param Lhs Path to concatenate onto.
	* @param Rhs Path to concatenate.
	* @return new YString of the path
	*/
	FORCEINLINE friend YString operator/(const TCHAR* Lhs, const YString& Rhs)
	{
		int32 StrLength = Rhs.Len();

		YString Result(YString(Lhs), StrLength + 1);
		Result.PathAppend(Rhs.Data.GetData(), Rhs.Len());
		return Result;
	}

	/**
	* Lexicographically test whether the left string is <= the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically <= the right string, otherwise false
	* @note case insensitive
	*/
	FORCEINLINE friend bool operator<=(const YString& Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, *Rhs) <= 0;
	}

	/**
	* Lexicographically test whether the left string is <= the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically <= the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator<=(const YString& Lhs, const CharType* Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, Rhs) <= 0;
	}

	/**
	* Lexicographically test whether the left string is <= the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically <= the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator<=(const CharType* Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(Lhs, *Rhs) <= 0;
	}

	/**
	* Lexicographically test whether the left string is < the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically < the right string, otherwise false
	* @note case insensitive
	*/
	FORCEINLINE friend bool operator<(const YString& Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, *Rhs) < 0;
	}

	/**
	* Lexicographically test whether the left string is < the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically < the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator<(const YString& Lhs, const CharType* Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, Rhs) < 0;
	}

	/**
	* Lexicographically test whether the left string is < the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically < the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator<(const CharType* Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(Lhs, *Rhs) < 0;
	}

	/**
	* Lexicographically test whether the left string is >= the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically >= the right string, otherwise false
	* @note case insensitive
	*/
	FORCEINLINE friend bool operator>=(const YString& Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, *Rhs) >= 0;
	}

	/**
	* Lexicographically test whether the left string is >= the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically >= the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator>=(const YString& Lhs, const CharType* Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, Rhs) >= 0;
	}

	/**
	* Lexicographically test whether the left string is >= the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically >= the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator>=(const CharType* Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(Lhs, *Rhs) >= 0;
	}

	/**
	* Lexicographically test whether the left string is > the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically > the right string, otherwise false
	* @note case insensitive
	*/
	FORCEINLINE friend bool operator>(const YString& Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, *Rhs) > 0;
	}

	/**
	* Lexicographically test whether the left string is > the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically > the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator>(const YString& Lhs, const CharType* Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, Rhs) > 0;
	}

	/**
	* Lexicographically test whether the left string is > the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically > the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator>(const CharType* Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(Lhs, *Rhs) > 0;
	}

	/**
	* Lexicographically test whether the left string is == the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically == the right string, otherwise false
	* @note case insensitive
	*/
	FORCEINLINE friend bool operator==(const YString& Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, *Rhs) == 0;
	}

	/**
	* Lexicographically test whether the left string is == the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically == the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator==(const YString& Lhs, const CharType* Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, Rhs) == 0;
	}

	/**
	* Lexicographically test whether the left string is == the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically == the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator==(const CharType* Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(Lhs, *Rhs) == 0;
	}

	/**
	* Lexicographically test whether the left string is != the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically != the right string, otherwise false
	* @note case insensitive
	*/
	FORCEINLINE friend bool operator!=(const YString& Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, *Rhs) != 0;
	}

	/**
	* Lexicographically test whether the left string is != the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically != the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator!=(const YString& Lhs, const CharType* Rhs)
	{
		return FPlatformString::Stricmp(*Lhs, Rhs) != 0;
	}

	/**
	* Lexicographically test whether the left string is != the right string
	*
	* @param Lhs String to compare against.
	* @param Rhs String to compare against.
	* @return true if the left string is lexicographically != the right string, otherwise false
	* @note case insensitive
	*/
	template <typename CharType>
	FORCEINLINE friend bool operator!=(const CharType* Lhs, const YString& Rhs)
	{
		return FPlatformString::Stricmp(Lhs, *Rhs) != 0;
	}

	/** Get the length of the string, excluding terminating character */
	FORCEINLINE int32 Len() const
	{
		return Data.Num() ? Data.Num() - 1 : 0;
	}

	/** @return the left most given number of characters */
	FORCEINLINE YString Left(int32 Count) const
	{
		return YString(YMath::Clamp(Count, 0, Len()), **this);
	}

	/** @return the left most characters from the string chopping the given number of characters from the end */
	FORCEINLINE YString LeftChop(int32 Count) const
	{
		return YString(YMath::Clamp(Len() - Count, 0, Len()), **this);
	}

	/** @return the string to the right of the specified location, counting back from the right (end of the word). */
	FORCEINLINE YString Right(int32 Count) const
	{
		return YString(**this + Len() - YMath::Clamp(Count, 0, Len()));
	}

	/** @return the string to the right of the specified location, counting forward from the left (from the beginning of the word). */
	FORCEINLINE YString RightChop(int32 Count) const
	{
		return YString(**this + Len() - YMath::Clamp(Len() - Count, 0, Len()));
	}

	/** @return the substring from Start position for Count characters. */
	FORCEINLINE YString Mid(int32 Start, int32 Count = MAX_int32) const
	{
		check(Count >= 0);
		uint32 End = Start + Count;
		Start = YMath::Clamp((uint32)Start, (uint32)0, (uint32)Len());
		End = YMath::Clamp((uint32)End, (uint32)Start, (uint32)Len());
		return YString(End - Start, **this + Start);
	}

	/**
	* Searches the string for a substring, and returns index into this string
	* of the first found instance. Can search from beginning or end, and ignore case or not.
	*
	* @param SubStr			The string array of TCHAR to search for
	* @param StartPosition		The start character position to search from
	* @param SearchCase		Indicates whether the search is case sensitive or not
	* @param SearchDir			Indicates whether the search starts at the begining or at the end.
	*/
	int32 Find(const TCHAR* SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
		ESearchDir::Type SearchDir = ESearchDir::FromStart, int32 StartPosition = INDEX_NONE) const;

	/**
	* Searches the string for a substring, and returns index into this string
	* of the first found instance. Can search from beginning or end, and ignore case or not.
	*
	* @param SubStr			The string to search for
	* @param StartPosition		The start character position to search from
	* @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @param SearchDir			Indicates whether the search starts at the begining or at the end ( defaults to ESearchDir::FromStart )
	*/
	FORCEINLINE int32 Find(const YString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
		ESearchDir::Type SearchDir = ESearchDir::FromStart, int32 StartPosition = INDEX_NONE) const
	{
		return Find(*SubStr, SearchCase, SearchDir, StartPosition);
	}

	/**
	* Returns whether this string contains the specified substring.
	*
	* @param SubStr			Find to search for
	* @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @param SearchDir			Indicates whether the search starts at the begining or at the end ( defaults to ESearchDir::FromStart )
	* @return					Returns whether the string contains the substring
	**/
	FORCEINLINE bool Contains(const TCHAR* SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
		ESearchDir::Type SearchDir = ESearchDir::FromStart) const
	{
		return Find(SubStr, SearchCase, SearchDir) != INDEX_NONE;
	}

	/**
	* Returns whether this string contains the specified substring.
	*
	* @param SubStr			Find to search for
	* @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @param SearchDir			Indicates whether the search starts at the begining or at the end ( defaults to ESearchDir::FromStart )
	* @return					Returns whether the string contains the substring
	**/
	FORCEINLINE bool Contains(const YString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
		ESearchDir::Type SearchDir = ESearchDir::FromStart) const
	{
		return Find(*SubStr, SearchCase, SearchDir) != INDEX_NONE;
	}

	/**
	* Searches the string for a character
	*
	* @param InChar the character to search for
	* @param Index out the position the character was found at, INDEX_NONE if return is false
	* @return true if character was found in this string, otherwise false
	*/
	FORCEINLINE bool FindChar(TCHAR InChar, int32& Index) const
	{
		return Data.Find(InChar, Index);
	}

	/**
	* Searches the string for the last occurrence of a character
	*
	* @param InChar the character to search for
	* @param Index out the position the character was found at, INDEX_NONE if return is false
	* @return true if character was found in this string, otherwise false
	*/
	FORCEINLINE bool FindLastChar(TCHAR InChar, int32& Index) const
	{
		return Data.FindLast(InChar, Index);
	}

	/**
	* Searches the string for the last occurrence of a character
	*
	* @param Pred Predicate that takes TCHAR and returns true if TCHAR matches search criteria, false otherwise.
	* @param StartIndex Index of element from which to start searching. Defaults to last TCHAR in string.
	*
	* @return Index of found TCHAR, INDEX_NONE otherwise.
	*/
	template <typename Predicate>
	FORCEINLINE int32 FindLastCharByPredicate(Predicate Pred, int32 StartIndex) const
	{
		check(StartIndex >= 0 && StartIndex <= this->Len());
		return Data.FindLastByPredicate(Pred, StartIndex);
	}

	/**
	* Searches the string for the last occurrence of a character
	*
	* @param Pred Predicate that takes TCHAR and returns true if TCHAR matches search criteria, false otherwise.
	* @param StartIndex Index of element from which to start searching. Defaults to last TCHAR in string.
	*
	* @return Index of found TCHAR, INDEX_NONE otherwise.
	*/
	template <typename Predicate>
	FORCEINLINE int32 FindLastCharByPredicate(Predicate Pred) const
	{
		return Data.FindLastByPredicate(Pred, this->Len());
	}

	/**
	* Lexicographically tests whether this string is equivalent to the Other given string
	*
	* @param Other 	The string test against
	* @param SearchCase 	Whether or not the comparison should ignore case
	* @return true if this string is lexicographically equivalent to the other, otherwise false
	*/
	FORCEINLINE bool Equals(const YString& Other, ESearchCase::Type SearchCase = ESearchCase::CaseSensitive) const
	{
		if (SearchCase == ESearchCase::CaseSensitive)
		{
			return FCString::Strcmp(**this, *Other) == 0;
		}
		else
		{
			return FCString::Stricmp(**this, *Other) == 0;
		}
	}

	/**
	* Lexicographically tests how this string compares to the Other given string
	*
	* @param Other 	The string test against
	* @param SearchCase 	Whether or not the comparison should ignore case
	* @return 0 if equal, negative if less than, positive if greater than
	*/
	FORCEINLINE int32 Compare(const YString& Other, ESearchCase::Type SearchCase = ESearchCase::CaseSensitive) const
	{
		if (SearchCase == ESearchCase::CaseSensitive)
		{
			return FCString::Strcmp(**this, *Other);
		}
		else
		{
			return FCString::Stricmp(**this, *Other);
		}
	}

	/**
	* Splits this string at given string position case sensitive.
	*
	* @param InStr The string to search and split at
	* @param LeftS out the string to the left of InStr, not updated if return is false
	* @param RightS out the string to the right of InStr, not updated if return is false
	* @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @param SearchDir			Indicates whether the search starts at the begining or at the end ( defaults to ESearchDir::FromStart )
	* @return true if string is split, otherwise false
	*/
	bool Split(const YString& InS, YString* LeftS, YString* RightS, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
		ESearchDir::Type SearchDir = ESearchDir::FromStart) const
	{
		int32 InPos = Find(InS, SearchCase, SearchDir);

		if (InPos < 0) { return false; }

		if (LeftS) { *LeftS = Left(InPos); }
		if (RightS) { *RightS = Mid(InPos + InS.Len()); }

		return true;
	}

	/** @return a new string with the characters of this converted to uppercase */
	YString ToUpper() const;

	/** Converts all characters in this string to uppercase */
	void ToUpperInline();

	/** @return a new string with the characters of this converted to lowercase */
	YString ToLower() const;

	/** Converts all characters in this string to lowercase */
	void ToLowerInline();

	/** Pad the left of this string for ChCount characters */
	YString LeftPad(int32 ChCount) const;

	/** Pad the right of this string for ChCount characters */
	YString RightPad(int32 ChCount) const;

	/** @return true if the string only contains numeric characters */
	bool IsNumeric() const;

	/**
	* Constructs YString object similarly to how classic sprintf works.
	*
	* @param Format	Format string that specifies how YString should be built optionally using additional args. Refer to standard printf format.
	* @param ...		Depending on format function may require additional arguments to build output object.
	*
	* @returns YString object that was constructed using format and additional parameters.
	*/
	VARARG_DECL(static YString, static YString, return, Printf, VARARG_NONE, const TCHAR*, VARARG_NONE, VARARG_NONE);

	/**
	* Format the specified string using the specified arguments. Replaces instances of { Argument } with keys in the map matching 'Argument'
	* @param InFormatString		A string representing the format expression
	* @param InNamedArguments		A map of named arguments that match the tokens specified in InExpression
	* @return A string containing the formatted text
	*/
	static YString Format(const TCHAR* InFormatString, const TMap<YString, YStringFormatArg>& InNamedArguments);

	/**
	* Format the specified string using the specified arguments. Replaces instances of {0} with indices from the given array matching the index specified in the token
	* @param InFormatString		A string representing the format expression
	* @param InOrderedArguments	An array of ordered arguments that match the tokens specified in InExpression
	* @return A string containing the formatted text
	*/
	static YString Format(const TCHAR* InFormatString, const TArray<YStringFormatArg>& InOrderedArguments);

	// @return string with Ch character
	static YString Chr(TCHAR Ch);

	/**
	* Returns a string that is full of a variable number of characters
	*
	* @param NumCharacters Number of characters to put into the string
	* @param Char Character to put into the string
	*
	* @return The string of NumCharacters characters.
	*/
	static YString ChrN(int32 NumCharacters, TCHAR Char);

	/**
	* Serializes the string.
	*
	* @param Ar Reference to the serialization archive.
	* @param S Reference to the string being serialized.
	*
	* @return Reference to the Archive after serialization.
	*/
	friend CORE_API YArchive& operator<<(YArchive& Ar, YString& S);


	/**
	* Test whether this string starts with given string.
	*
	* @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @return true if this string begins with specified text, false otherwise
	*/
	bool StartsWith(const TCHAR* InSuffix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const;

	/**
	* Test whether this string starts with given string.
	*
	* @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @return true if this string begins with specified text, false otherwise
	*/
	bool StartsWith(const YString& InPrefix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const;

	/**
	* Test whether this string ends with given string.
	*
	* @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @return true if this string ends with specified text, false otherwise
	*/
	bool EndsWith(const TCHAR* InSuffix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const;

	/**
	* Test whether this string ends with given string.
	*
	* @param SearchCase		Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @return true if this string ends with specified text, false otherwise
	*/
	bool EndsWith(const YString& InSuffix, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const;

	/**
	* Searches this string for a given wild card
	*
	* @param Wildcard		*?-type wildcard
	* @param SearchCase	Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @return true if this string matches the *?-type wildcard given.
	* @warning This is a simple, SLOW routine. Use with caution
	*/
	bool MatchesWildcard(const YString& Wildcard, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const;

	/**
	* Removes whitespace characters from the front of this string.
	*/
	YString Trim();

	/**
	* Removes trailing whitespace characters
	*/
	YString TrimTrailing(void);

	/**
	* Trims the inner array after the null terminator.
	*/
	void TrimToNullTerminator();

	/**
	* Returns a copy of this string with wrapping quotation marks removed.
	*/
	YString TrimQuotes(bool* bQuotesRemoved = nullptr) const;

	/**
	* Breaks up a delimited string into elements of a string array.
	*
	* @param	InArray		The array to fill with the string pieces
	* @param	pchDelim	The string to delimit on
	* @param	InCullEmpty	If 1, empty strings are not added to the array
	*
	* @return	The number of elements in InArray
	*/
	int32 ParseIntoArray(TArray<YString>& OutArray, const TCHAR* pchDelim, bool InCullEmpty = true) const;

	/**
	* Breaks up a delimited string into elements of a string array, using any whitespace and an
	* optional extra delimter, like a ","
	* @warning Caution!! this routine is O(N^2) allocations...use it for parsing very short text or not at all!
	*
	* @param	InArray			The array to fill with the string pieces
	* @param	pchExtraDelim	The string to delimit on
	*
	* @return	The number of elements in InArray
	*/
	int32 ParseIntoArrayWS(TArray<YString>& OutArray, const TCHAR* pchExtraDelim = nullptr, bool InCullEmpty = true) const;

	/**
	* Breaks up a delimited string into elements of a string array, using line ending characters
	* @warning Caution!! this routine is O(N^2) allocations...use it for parsing very short text or not at all!
	*
	* @param	InArray			The array to fill with the string pieces
	*
	* @return	The number of elements in InArray
	*/
	int32 ParseIntoArrayLines(TArray<YString>& OutArray, bool InCullEmpty = true) const;

	/**
	* Breaks up a delimited string into elements of a string array, using the given delimiters
	* @warning Caution!! this routine is O(N^2) allocations...use it for parsing very short text or not at all!
	*
	* @param	InArray			The array to fill with the string pieces
	* @param	DelimArray		The strings to delimit on
	* @param	NumDelims		The number of delimiters.
	*
	* @return	The number of elements in InArray
	*/
	int32 ParseIntoArray(TArray<YString>& OutArray, const TCHAR** DelimArray, int32 NumDelims, bool InCullEmpty = true) const;

	/**
	* Takes an array of strings and removes any zero length entries.
	*
	* @param	InArray	The array to cull
	*
	* @return	The number of elements left in InArray
	*/
	static int32 CullArray(TArray<YString>* InArray);

	/**
	* Returns a copy of this string, with the characters in reverse order
	*/
	YString Reverse() const;

	/**
	* Reverses the order of characters in this string
	*/
	void ReverseString();

	/**
	* Replace all occurrences of a substring in this string
	*
	* @param From substring to replace
	* @param To substring to replace From with
	* @param SearchCase	Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	* @return a copy of this string with the replacement made
	*/
	YString Replace(const TCHAR* From, const TCHAR* To, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase) const;

	/**
	* Replace all occurrences of SearchText with ReplacementText in this string.
	*
	* @param	SearchText	the text that should be removed from this string
	* @param	ReplacementText		the text to insert in its place
	* @param SearchCase	Indicates whether the search is case sensitive or not ( defaults to ESearchCase::IgnoreCase )
	*
	* @return	the number of occurrences of SearchText that were replaced.
	*/
	int32 ReplaceInline(const TCHAR* SearchText, const TCHAR* ReplacementText, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase);

	/**
	* Returns a copy of this string with all quote marks escaped (unless the quote is already escaped)
	*/
	YString ReplaceQuotesWithEscapedQuotes() const;

	/**
	* Replaces certain characters with the "escaped" version of that character (i.e. replaces "\n" with "\\n").
	* The characters supported are: { \n, \r, \t, \', \", \\ }.
	*
	* @param	Chars	by default, replaces all supported characters; this parameter allows you to limit the replacement to a subset.
	*
	* @return	a string with all control characters replaced by the escaped version.
	*/
	YString ReplaceCharWithEscapedChar(const TArray<TCHAR>* Chars = nullptr) const;

	/**
	* Removes the escape backslash for all supported characters, replacing the escape and character with the non-escaped version.  (i.e.
	* replaces "\\n" with "\n".  Counterpart to ReplaceCharWithEscapedChar().
	* @return copy of this string with replacement made
	*/
	YString ReplaceEscapedCharWithChar(const TArray<TCHAR>* Chars = nullptr) const;

	/**
	* Replaces all instances of '\t' with TabWidth number of spaces
	* @param InSpacesPerTab - Number of spaces that a tab represents
	* @return copy of this string with replacement made
	*/
	YString ConvertTabsToSpaces(const int32 InSpacesPerTab);

	// Takes the number passed in and formats the string in comma format ( 12345 becomes "12,345")
	static YString FormatAsNumber(int32 InNumber);

	// To allow more efficient memory handling, automatically adds one for the string termination.
	FORCEINLINE void Reserve(const uint32 CharacterCount)
	{
		Data.Reserve(CharacterCount + 1);
	}

	/**
	* Serializes a string as ANSI char array.
	*
	* @param	String			String to serialize
	* @param	Ar				Archive to serialize with
	* @param	MinCharacters	Minimum number of characters to serialize.
	*/
	void SerializeAsANSICharArray(YArchive& Ar, int32 MinCharacters = 0) const;


	/** Converts an integer to a string. */
	static FORCEINLINE YString FromInt(int32 Num)
	{
		YString Ret;
		Ret.AppendInt(Num);
		return Ret;
	}

	/** appends the integer InNum to this string */
	void AppendInt(int32 InNum);

	/**
	* Converts a string into a boolean value
	*   1, "True", "Yes", GTrue, GYes, and non-zero integers become true
	*   0, "False", "No", GFalse, GNo, and unparsable values become false
	*
	* @return The boolean value
	*/
	bool ToBool() const;

	/**
	* Converts a buffer to a string
	*
	* @param SrcBuffer the buffer to stringify
	* @param SrcSize the number of bytes to convert
	*
	* @return the blob in string form
	*/
	static YString FromBlob(const uint8* SrcBuffer, const uint32 SrcSize);

	/**
	* Converts a string into a buffer
	*
	* @param DestBuffer the buffer to fill with the string data
	* @param DestSize the size of the buffer in bytes (must be at least string len / 3)
	*
	* @return true if the conversion happened, false otherwise
	*/
	static bool ToBlob(const YString& Source, uint8* DestBuffer, const uint32 DestSize);

	/**
	* Converts a buffer to a string by hex-ifying the elements
	*
	* @param SrcBuffer the buffer to stringify
	* @param SrcSize the number of bytes to convert
	*
	* @return the blob in string form
	*/
	static YString FromHexBlob(const uint8* SrcBuffer, const uint32 SrcSize);

	/**
	* Converts a string into a buffer
	*
	* @param DestBuffer the buffer to fill with the string data
	* @param DestSize the size of the buffer in bytes (must be at least string len / 2)
	*
	* @return true if the conversion happened, false otherwise
	*/
	static bool ToHexBlob(const YString& Source, uint8* DestBuffer, const uint32 DestSize);

	/**
	* Converts a float string with the trailing zeros stripped
	* For example - 1.234 will be "1.234" rather than "1.234000"
	*
	* @param	InFloat		The float to sanitize
	* @returns sanitized string version of float
	*/
	static YString SanitizeFloat(double InFloat);

	/**
	* Joins an array of 'something that can be concatentated to strings with +=' together into a single string with separators.
	*
	* @param	Array		The array of 'things' to concatenate.
	* @param	Separator	The string used to separate each element.
	*
	* @return	The final, joined, separated string.
	*/
	template <typename T, typename Allocator>
	static YString Join(const TArray<T, Allocator>& Array, const TCHAR* Separator)
	{
		YString Result;
		bool    First = true;
		for (const T& Element : Array)
		{
			if (First)
			{
				First = false;
			}
			else
			{
				Result += Separator;
			}

			Result += Element;
		}

		return Result;
	}
};

template<>
struct TContainerTraits<YString> : public TContainerTraitsBase<YString>
{
	enum { MoveWillEmptyContainer = TContainerTraits<YString::DataType>::MoveWillEmptyContainer };
};

template<> struct TIsZeroConstructType<YString> { enum { Value = true }; };
Expose_TNameOf(YString)

template <>
struct TIsContiguousContainer<YString>
{
	enum { Value = true };
};

inline TCHAR* GetData(YString& String)
{
	return String.GetCharArray().GetData();
}

inline const TCHAR* GetData(const YString& String)
{
	return String.GetCharArray().GetData();
}

inline SIZE_T GetNum(const YString& String)
{
	return String.GetCharArray().Num();
}

/** Case insensitive string hash function. */
FORCEINLINE uint32 GetTypeHash(const YString& S)
{
	return FCrc::Strihash_DEPRECATED(*S);
}

/**
* Convert an array of bytes to a TCHAR
* @param In byte array values to convert
* @param Count number of bytes to convert
* @return Valid string representing bytes.
*/
inline YString BytesToString(const uint8* In, int32 Count)
{
	YString Result;
	Result.Empty(Count);

	while (Count)
	{
		// Put the byte into an int16 and add 1 to it, this keeps anything from being put into the string as a null terminator
		int16 Value = *In;
		Value += 1;

		Result += TCHAR(Value);

		++In;
		Count--;
	}
	return Result;
}

/**
* Convert YString of bytes into the byte array.
* @param String		The YString of byte values
* @param OutBytes		Ptr to memory must be preallocated large enough
* @param MaxBufferSize	Max buffer size of the OutBytes array, to prevent overflow
* @return	The number of bytes copied
*/
inline int32 StringToBytes(const YString& String, uint8* OutBytes, int32 MaxBufferSize)
{
	int32 NumBytes = 0;
	const TCHAR* CharPos = *String;

	while (*CharPos && NumBytes < MaxBufferSize)
	{
		OutBytes[NumBytes] = (int8)(*CharPos - 1);
		CharPos++;
		++NumBytes;
	}
	return NumBytes - 1;
}

/** @return Char value of Nibble */
inline TCHAR NibbleToTChar(uint8 Num)
{
	if (Num > 9)
	{
		return TEXT('A') + TCHAR(Num - 10);
	}
	return TEXT('0') + TCHAR(Num);
}

/**
* Convert a byte to hex
* @param In byte value to convert
* @param Result out hex value output
*/
inline void ByteToHex(uint8 In, YString& Result)
{
	Result += NibbleToTChar(In >> 4);
	Result += NibbleToTChar(In & 15);
}

/**
* Convert an array of bytes to hex
* @param In byte array values to convert
* @param Count number of bytes to convert
* @return Hex value in string.
*/
inline YString BytesToHex(const uint8* In, int32 Count)
{
	YString Result;
	Result.Empty(Count * 2);

	while (Count)
	{
		ByteToHex(*In++, Result);
		Count--;
	}
	return Result;
}

/**
* Checks if the TChar is a valid hex character
* @param Char		The character
* @return	True if in 0-9 and A-F ranges
*/
inline const bool CheckTCharIsHex(const TCHAR Char)
{
	return (Char >= TEXT('0') && Char <= TEXT('9')) || (Char >= TEXT('A') && Char <= TEXT('F')) || (Char >= TEXT('a') && Char <= TEXT('f'));
}

/**
* Convert a TChar to equivalent hex value as a uint8
* @param Char		The character
* @return	The uint8 value of a hex character
*/
inline const uint8 TCharToNibble(const TCHAR Char)
{
	check(CheckTCharIsHex(Char));
	if (Char >= TEXT('0') && Char <= TEXT('9'))
	{
		return Char - TEXT('0');
	}
	else if (Char >= TEXT('A') && Char <= TEXT('F'))
	{
		return (Char - TEXT('A')) + 10;
	}
	return (Char - TEXT('a')) + 10;
}

/**
* Convert YString of Hex digits into the byte array.
* @param HexString		The YString of Hex values
* @param OutBytes		Ptr to memory must be preallocated large enough
* @return	The number of bytes copied
*/
inline int32 HexToBytes(const YString& HexString, uint8* OutBytes)
{
	int32 NumBytes = 0;
	const bool bPadNibble = (HexString.Len() % 2) == 1;
	const TCHAR* CharPos = *HexString;
	if (bPadNibble)
	{
		OutBytes[NumBytes++] = TCharToNibble(*CharPos++);
	}
	while (*CharPos)
	{
		OutBytes[NumBytes] = TCharToNibble(*CharPos++) << 4;
		OutBytes[NumBytes] += TCharToNibble(*CharPos++);
		++NumBytes;
	}
	return NumBytes;
}

/** Namespace that houses lexical conversion for various types. User defined conversions can be implemented externally */
namespace Lex
{
	/**
	*	Expected functions in this namespace are as follows:
	*		static bool		TryParseString(T& OutValue, const TCHAR* Buffer);
	*		static void 	FromString(T& OutValue, const TCHAR* Buffer);
	*		static YString	ToString(const T& OutValue);
	*
	*	Implement custom functionality externally.
	*/

	/** Covert a string buffer to intrinsic types */
	inline void FromString(int8& OutValue, const TCHAR* Buffer) { OutValue = FCString::Atoi(Buffer); }
	inline void FromString(int16& OutValue, const TCHAR* Buffer) { OutValue = FCString::Atoi(Buffer); }
	inline void FromString(int32& OutValue, const TCHAR* Buffer) { OutValue = FCString::Atoi(Buffer); }
	inline void FromString(int64& OutValue, const TCHAR* Buffer) { OutValue = FCString::Atoi64(Buffer); }
	inline void FromString(uint8& OutValue, const TCHAR* Buffer) { OutValue = FCString::Atoi(Buffer); }
	inline void FromString(uint16& OutValue, const TCHAR* Buffer) { OutValue = FCString::Atoi(Buffer); }
	inline void FromString(uint32& OutValue, const TCHAR* Buffer) { OutValue = FCString::Atoi64(Buffer); }	//64 because this unsigned and so Atoi might overflow
	inline void FromString(uint64& OutValue, const TCHAR* Buffer) { OutValue = FCString::Strtoui64(Buffer, nullptr, 0); }
	inline void FromString(float& OutValue, const TCHAR* Buffer) { OutValue = FCString::Atof(Buffer); }
	inline void FromString(double& OutValue, const TCHAR* Buffer) { OutValue = FCString::Atod(Buffer); }
	inline void FromString(bool& OutValue, const TCHAR* Buffer) { OutValue = FCString::ToBool(Buffer); }
	inline void FromString(YString& OutValue, const TCHAR* Buffer) { OutValue = Buffer; }

	/** Convert numeric types to a string */
	template<typename T>
	typename TEnableIf<TIsArithmetic<T>::Value, YString>::Type
		ToString(const T& Value)
	{
		return YString::Printf(TFormatSpecifier<T>::GetFormatSpecifier(), Value);
	}

	template<typename CharType>
	typename TEnableIf<TIsCharType<CharType>::Value, YString>::Type
		ToString(const CharType* Ptr)
	{
		return YString(Ptr);
	}

	inline YString ToString(bool Value)
	{
		return Value ? TEXT("true") : TEXT("false");
	}

	FORCEINLINE YString ToString(YString&& Str)
	{
		return MoveTemp(Str);
	}

	FORCEINLINE YString ToString(const YString& Str)
	{
		return Str;
	}

	/** Helper template to convert to sanitized strings */
	template<typename T>
	YString ToSanitizedString(const T& Value)
	{
		return ToString(Value);
	}

	/** Specialized for floats */
	template<>
	inline YString ToSanitizedString<float>(const float& Value)
	{
		return YString::SanitizeFloat(Value);
	}


	/** Parse a string into this type, returning whether it was successful */
	/** Specialization for arithmetic types */
	template<typename T>
	static typename TEnableIf<TIsArithmetic<T>::Value, bool>::Type
		TryParseString(T& OutValue, const TCHAR* Buffer)
	{
		if (FCString::IsNumeric(Buffer))
		{
			FromString(OutValue, Buffer);
			return true;
		}
		return false;
	}

	/** Try and parse a bool - always returns true */
	static bool TryParseString(bool& OutValue, const TCHAR* Buffer)
	{
		FromString(OutValue, Buffer);
		return true;
	}
}

// Deprecated alias for old LexicalConversion namespace.
// Namespace alias deprecation doesn't work on our compilers, so we can't actually mark it with the DEPRECATED() macro.
namespace LexicalConversion = Lex;

/** Shorthand legacy use for Lex functions */
template<typename T>
struct TTypeToString
{
	static YString ToString(const T& Value) { return Lex::ToString(Value); }
	static YString ToSanitizedString(const T& Value) { return Lex::ToSanitizedString(Value); }
};
template<typename T>
struct TTypeFromString
{
	static void FromString(T& Value, const TCHAR* Buffer) { return Lex::FromString(Value, Buffer); }
};

/*----------------------------------------------------------------------------
Special archivers.
----------------------------------------------------------------------------*/

//
// String output device.
//
class YStringOutputDevice : public YString, public YOutputDevice
{
public:
	YStringOutputDevice(const TCHAR* OutputDeviceName = TEXT("")) :
		YString(OutputDeviceName)
	{
		bAutoEmitLineTerminator = false;
	}
	virtual void Serialize(const TCHAR* InData, ELogVerbosity::Type Verbosity, const class FName& Category) override
	{
		YString::operator+=((TCHAR*)InData);
		if (bAutoEmitLineTerminator)
		{
			*this += LINE_TERMINATOR;
		}
	}

#if PLATFORM_COMPILER_HAS_DEFAULTED_FUNCTIONS

	YStringOutputDevice(YStringOutputDevice&&) = default;
	YStringOutputDevice(const YStringOutputDevice&) = default;
	YStringOutputDevice& operator=(YStringOutputDevice&&) = default;
	YStringOutputDevice& operator=(const YStringOutputDevice&) = default;

#else

	FORCEINLINE YStringOutputDevice(YStringOutputDevice&& Other)
		: YString((YString&&)Other)
		, YOutputDevice((YOutputDevice&&)Other)
	{
	}

	FORCEINLINE YStringOutputDevice(const YStringOutputDevice& Other)
		: YString((const YString&)Other)
		, YOutputDevice((const YOutputDevice&)Other)
	{
	}

	FORCEINLINE YStringOutputDevice& operator=(YStringOutputDevice&& Other)
	{
		(YString&)*this = (YString&&)Other;
		(YOutputDevice&)*this = (YOutputDevice&&)Other;
		return *this;
	}

	FORCEINLINE YStringOutputDevice& operator=(const YStringOutputDevice& Other)
	{
		(YString&)*this = (const YString&)Other;
		(YOutputDevice&)*this = (const YOutputDevice&)Other;
		return *this;
	}

#endif
	// Make += operator virtual.
	virtual YString& operator+=(const YString& Other)
	{
		return YString::operator+=(Other);
	}
};

//
// String output device.
//
class YStringOutputDeviceCountLines : public YStringOutputDevice
{
	typedef YStringOutputDevice Super;

	int32 LineCount;
public:
	YStringOutputDeviceCountLines(const TCHAR* OutputDeviceName = TEXT(""))
		: Super(OutputDeviceName)
		, LineCount(0)
	{}

	virtual void Serialize(const TCHAR* InData, ELogVerbosity::Type Verbosity, const class FName& Category) override
	{
		Super::Serialize(InData, Verbosity, Category);
		int32 TermLength = FCString::Strlen(LINE_TERMINATOR);
		for (;;)
		{
			InData = FCString::Strstr(InData, LINE_TERMINATOR);
			if (!InData)
			{
				break;
			}
			LineCount++;
			InData += TermLength;
		}

		if (bAutoEmitLineTerminator)
		{
			LineCount++;
		}
	}

	/**
	* Appends other YStringOutputDeviceCountLines object to this one.
	*/
	virtual YStringOutputDeviceCountLines& operator+=(const YStringOutputDeviceCountLines& Other)
	{
		YString::operator+=(Other);

		LineCount += Other.GetLineCount();

		return *this;
	}

	/**
	* Appends other YString (as well as it's specializations like YStringOutputDevice)
	* object to this.
	*/
	virtual YString& operator+=(const YString& Other) override
	{
		Log(Other);

		return *this;
	}

	int32 GetLineCount() const
	{
		return LineCount;
	}

#if PLATFORM_COMPILER_HAS_DEFAULTED_FUNCTIONS

	YStringOutputDeviceCountLines(const YStringOutputDeviceCountLines&) = default;
	YStringOutputDeviceCountLines& operator=(const YStringOutputDeviceCountLines&) = default;

#else

	FORCEINLINE YStringOutputDeviceCountLines(const YStringOutputDeviceCountLines& Other)
		: Super((const Super&)Other)
		, LineCount(Other.LineCount)
	{
	}

	FORCEINLINE YStringOutputDeviceCountLines& operator=(const YStringOutputDeviceCountLines& Other)
	{
		(Super&)*this = (const Super&)Other;
		LineCount = Other.LineCount;
		return *this;
	}

#endif

	FORCEINLINE YStringOutputDeviceCountLines(YStringOutputDeviceCountLines&& Other)
		: Super((Super&&)Other)
		, LineCount(Other.LineCount)
	{
		Other.LineCount = 0;
	}

	FORCEINLINE YStringOutputDeviceCountLines& operator=(YStringOutputDeviceCountLines&& Other)
	{
		if (this != &Other)
		{
			(Super&)*this = (Super&&)Other;
			LineCount = Other.LineCount;

			Other.LineCount = 0;
		}
		return *this;
	}
};

/**
* A helper function to find closing parenthesis that matches the first open parenthesis found. The open parenthesis
* referred to must be at or further up from the start index.
*
* @param TargetString      The string to search in
* @param StartSearch       The index to start searching at
* @return the index in the given string of the closing parenthesis
*/
CORE_API int32 FindMatchingClosingParenthesis(const YString& TargetString, const int32 StartSearch = 0);

#include "Misc/StringFormatArg.h"
