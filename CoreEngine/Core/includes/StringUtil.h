#pragma once
#include <Core/includes/Base.h>

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