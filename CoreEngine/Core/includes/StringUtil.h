#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/Platform.h>

namespace Utils
{

	String ExtractStrBetweenStr(const String& SourceStr, const StringView& Left, const StringView& Right)
	{
		int64 LeftPos = SourceStr.find(Left);
		int64 RightPos = SourceStr.find(Right);
		if (LeftPos < 0) return SourceStr.substr(0, RightPos);
		if (RightPos < 0) return SourceStr.substr(LeftPos);

		// For exclude Left and Right from substr
		LeftPos++;

		return SourceStr.substr(LeftPos, (RightPos - LeftPos));
	}

	String ExtractLeftSubStrFindLast(const String& SourceStr, const StringView& Chr)
	{
		int64 Pos = SourceStr.find_last_of(Chr);
		if (Pos < 0) return "";

		return SourceStr.substr(0, Pos);
	}

	String ExtractRightSubStrFindLast(const String& SourceStr, const StringView& Chr)
	{
		int64 Pos = SourceStr.find_last_of(Chr);
		if (Pos < 0) return "";

		return SourceStr.substr(Pos + 1);
	}

	//int32 ConverToInt32(const String& StrNumber);
	//int64 ConverToInt64(const String& StrNumber);

	//String ConvertToString(int32 Number);
	//String ConvertToString(int64 Number);
	//String ConvertToString(uint32 Number);
	//String ConvertToString(uint64 Number);
	//String ConvertToString(float Number);
	//String ConvertToString(double Number);

	int32 ConverToInt32(const String& StrNumber)
	{
		return std::stoi(StrNumber);
	}

	int64 ConverToInt64(const String& StrNumber)
	{

		return std::stoll(StrNumber);
	}

	String ConvertToString(int32 Number)
	{
		return std::to_string(Number);
	}

	String ConvertToString(int64 Number)
	{
		return std::to_string(Number);
	}

	String ConvertToString(uint32 Number)
	{
		return std::to_string(Number);
	}
	
	String ConvertToString(uint64 Number)
	{
		return std::to_string(Number);
	}
	
	String ConvertToString(float Number)
	{
		return std::to_string(Number);
	}
	
	String ConvertToString(double Number)
	{
		return std::to_string(Number);
	}
}