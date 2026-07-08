#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/Platform.h>

namespace Utils
{

	String ExtractStrBetweenStr(const String& SourceStr, const StringView& Left, const StringView& Right);

	String ExtractLeftSubStrFindLast(const String& SourceStr, const StringView& Chr);

	String ExtractRightSubStrFindLast(const String& SourceStr, const StringView& Chr);

	// int32 ConverToInt32(const String& StrNumber);
	// int64 ConverToInt64(const String& StrNumber);

	// String ConvertToString(int32 Number);
	// String ConvertToString(int64 Number);
	// String ConvertToString(uint32 Number);
	// String ConvertToString(uint64 Number);
	// String ConvertToString(float Number);
	// String ConvertToString(double Number);

	int32 ConverToInt32(const String& StrNumber);

	int64 ConverToInt64(const String& StrNumber);

	String ConvertToString(int32 Number);
	String ConvertToString(int64 Number);

	String ConvertToString(uint32 Number);

	String ConvertToString(uint64 Number);

	String ConvertToString(float Number);

	String ConvertToString(double Number);
} // namespace Utils
