// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GenericPlatform/GenericPlatformString.h"
#include "HAL/SolidAngleMemory.h"
#include "Misc/Char.h"
#include "Containers/SolidAngleString.h"
#include "Logging/LogMacros.h"


DEFINE_LOG_CATEGORY_STATIC(LogGenericPlatformString, Log, All);

template <> const TCHAR* YGenericPlatformString::GetEncodingTypeName<ANSICHAR>() { return TEXT("ANSICHAR"); }
template <> const TCHAR* YGenericPlatformString::GetEncodingTypeName<WIDECHAR>() { return TEXT("WIDECHAR"); }
template <> const TCHAR* YGenericPlatformString::GetEncodingTypeName<UCS2CHAR>() { return TEXT("UCS2CHAR"); }


void* YGenericPlatformString::Memcpy(void* Dest, const void* Src, SIZE_T Count)
{
	return YMemory::Memcpy(Dest, Src, Count);
}

namespace
{
	void TrimStringAndLogBogusCharsError(YString& SrcStr, const TCHAR* SourceCharName, const TCHAR* DestCharName)
	{
		SrcStr.Trim();
		// @todo: Put this back in once GLog becomes a #define, or is replaced with GLog::GetLog()
		//UE_LOG(LogGenericPlatformString, Warning, TEXT("Bad chars found when trying to convert \"%s\" from %s to %s"), *SrcStr, SourceCharName, DestCharName);
	}
}

template <typename DestEncoding, typename SourceEncoding>
void YGenericPlatformString::LogBogusChars(const SourceEncoding* Src, int32 SrcSize)
{
	YString SrcStr;
	bool    bFoundBogusChars = false;
	for (; SrcSize; --SrcSize)
	{
		SourceEncoding SrcCh = *Src++;
		if (!CanConvertChar<DestEncoding>(SrcCh))
		{
			SrcStr += YString::Printf(TEXT("[0x%X]"), (int32)SrcCh);
			bFoundBogusChars = true;
		}
		else if (CanConvertChar<TCHAR>(SrcCh))
		{
			if (TChar<SourceEncoding>::IsLinebreak(SrcCh))
			{
				if (bFoundBogusChars)
				{
					TrimStringAndLogBogusCharsError(SrcStr, GetEncodingTypeName<SourceEncoding>(), GetEncodingTypeName<DestEncoding>());
					bFoundBogusChars = false;
				}
				SrcStr.Empty();
			}
			else
			{
				SrcStr.AppendChar((TCHAR)SrcCh);
			}
		}
		else
		{
			SrcStr.AppendChar((TCHAR)'?');
		}
	}

	if (bFoundBogusChars)
	{
		TrimStringAndLogBogusCharsError(SrcStr, GetEncodingTypeName<SourceEncoding>(), GetEncodingTypeName<DestEncoding>());
	}
}

#if !UE_BUILD_DOCS
template CORE_API void YGenericPlatformString::LogBogusChars<ANSICHAR, WIDECHAR>(const WIDECHAR* Src, int32 SrcSize);
template CORE_API void YGenericPlatformString::LogBogusChars<ANSICHAR, UCS2CHAR>(const UCS2CHAR* Src, int32 SrcSize);
template CORE_API void YGenericPlatformString::LogBogusChars<WIDECHAR, ANSICHAR>(const ANSICHAR* Src, int32 SrcSize);
template CORE_API void YGenericPlatformString::LogBogusChars<WIDECHAR, UCS2CHAR>(const UCS2CHAR* Src, int32 SrcSize);
template CORE_API void YGenericPlatformString::LogBogusChars<UCS2CHAR, ANSICHAR>(const ANSICHAR* Src, int32 SrcSize);
template CORE_API void YGenericPlatformString::LogBogusChars<UCS2CHAR, WIDECHAR>(const WIDECHAR* Src, int32 SrcSize);
#endif
