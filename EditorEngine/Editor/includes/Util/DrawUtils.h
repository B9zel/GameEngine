#pragma once
#include <imgui.h>
#include <Math/includes/Vector.h>

namespace Editor
{
	void DrawInt8(const String& Id, const String& Name, int8& Scalar, int8 Max, int8 Min, const float ColumnWidht=50);
	void DrawInt16(const String& Id, const String& Name, int16& Scalar, int16 Max, int16 Min, const float ColumnWidht=50);
	void DrawInt32(const String& Id, const String& Name, int32& Scalar, int32 Max, int32 Min, const float ColumnWidht=50);
	void DrawInt64(const String& Id, const String& Name, int64& Scalar, int64 Max, int64 Min, const float ColumnWidht=50);

	void DrawUInt8(const String& Id, const String& Name, uint8& Scalar, uint8 Max, uint8 Min, const float ColumnWidht=50);
	void DrawUInt16(const String& Id, const String& Name, uint16& Scalar, uint16 Max, uint16 Min, const float ColumnWidht=50);
	void DrawUInt32(const String& Id, const String& Name, uint32& Scalar, uint32 Max, uint32 Min, const float ColumnWidht=50);
	void DrawUInt64(const String& Id, const String& Name, uint64& Scalar, uint64 Max, uint64 Min, const float ColumnWidht=50);
	void DrawFloat(const String& Id, const String& Name, float& Scalar, float Max, float Min, const float ColumnWidht=50);
	void DrawDouble(const String& Id, const String& Name, double& Scalar, double Max, double Min, const float ColumnWidht=50);
	void DrawVector3(const String& Id, const String& NameOfVec, FVector& Vector, const ImVec4& Color, const float ColumnWidht=50);
	void DrawTransform(const String& Id, const String& NameOfTransform, FVector& Location, FVector& Rotation, FVector& Scale, const float ColumnWidth = 100);
}