#pragma once
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <imgui.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Math/includes/Vector.h>
#include <Render/includes/Types/Color.h>
#include <Math/includes/Matrix.h>

class Object;
class EditorEngine;

namespace Editor
{
	uint64 CalculateSizeOfPropertyName(const String& Str, const float MaxWidth);

	void DrawInt8(const String& Id, const String& Name, int8& Scalar, int8 Max, int8 Min, const float ColumnWidht = 50);
	void DrawInt16(const String& Id, const String& Name, int16& Scalar, int16 Max, int16 Min, const float ColumnWidht = 50);

	void DrawInt32(const String& Id, const String& Name, int32& Scalar, int32 Max = std::numeric_limits<int32>::max(),
				   int32 Min = std::numeric_limits<int32>::lowest(), const float ColumnWidht = 50);
	void DrawInt64(const String& Id, const String& Name, int64& Scalar, int64 Max = std::numeric_limits<int64>::max(),
				   int64 Min = std::numeric_limits<int64>::lowest(), const float ColumnWidht = 50);

	void DrawUInt8(const String& Id, const String& Name, uint8& Scalar, uint8 Max, uint8 Min, const float ColumnWidht = 50);
	void DrawUInt16(const String& Id, const String& Name, uint16& Scalar, uint16 Max, uint16 Min, const float ColumnWidht = 50);

	void DrawUInt32(const String& Id, const String& Name, uint32& Scalar, uint32 Max = std::numeric_limits<uint32>::max(),
					uint32 Min = std::numeric_limits<uint32>::lowest(), const float ColumnWidht = 50);

	void DrawUInt64(const String& Id, const String& Name, uint64& Scalar, uint64 Max = std::numeric_limits<uint64>::max(),
					uint64 Min = std::numeric_limits<uint64>::lowest(), const float ColumnWidht = 50);

	void DrawFloat(const String& Id, const String& Name, float& Scalar, float Max = std::numeric_limits<float>::max(),
				   float Min = std::numeric_limits<float>::lowest(), const float ColumnWidht = 50);

	void DrawDouble(const String& Id, const String& Name, double& Scalar, double Max = std::numeric_limits<double>::max(),
					double Min = std::numeric_limits<double>::lowest(), const float ColumnWidht = 50);

	void DrawVector3(const String& Id, const String& NameOfVec, FVector& Vector, const float ColumnWidht = 50);
	void DrawTransform(const String& Id, const String& NameOfTransform, FVector& Location, FVector& Rotation, FVector& Scale, const float ColumnWidth = 100);
	void DrawString(const String& Id, const String& NameString, String& SourceStr, const uint32 MaxBufferSize, const float ColumnWidth = 100);
	void DrawBool(const String& Id, const String& NameString, bool& Value, const float ColumnWidth = 100);
	void DrawColor(const String& Id, const String& NameString, LinearColor& Value, const float ColumnWidth = 100);

	void DrawMatrix4x4(const String& Id, const String& NameString, FMatrix4x4& Value, const float ColumnWidth = 100);

	template <class T>
	int32 DrawComboBox(const String& Id, const String& NameString, const String& DefaultValue, const DArray<T*>& Values, int32 SelectedIndex,
					   const float ColumnWidth = 100);

	bool DrawComponentContextDraw(EditorEngine* Engine, Object* SelectedObject);
	void PushColorTree();

	template <class T>
	int32 DrawComboBox(const String& Id, const String& NameString, const String& DefaultValue, const DArray<T*>& Values, int32 SelectedIndex,
					   const float ColumnWidth)
	{
		if (Values.empty() || !Values.front()->GetClass()->IsChildClassOf(Asset::GetStaticClass())) return -1;

		int32 SelectItem = -1;

		ImGui::PushID(Id.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, ColumnWidth);

		float ColWidth = ImGui::GetColumnWidth(0);
		ImVec2 TextSize = ImGui::CalcTextSize(NameString.c_str());
		uint64 Size = CalculateSizeOfPropertyName(NameString, ColWidth);
		if (Size == NameString.size())
		{
			ImGui::TextUnformatted(NameString.c_str());
		}
		else
		{
			ImGui::TextUnformatted((NameString.substr(0, Size) + "...").c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("%s", NameString.c_str());
			}
		}
		ImGui::NextColumn();
		ImGui::PushItemWidth(-FLT_MIN);

		if (ImGui::BeginCombo(("##" + NameString).c_str(), DefaultValue.c_str()))
		{
			for (uint64 i = 0; i < Values.size(); i++)
			{
				{
					bool IsSelected = (i == SelectedIndex);
					if (ImGui::Selectable(Values[i]->GetName().c_str(), IsSelected))
					{
						SelectItem = i;
					}
					if (IsSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Columns(1);
		ImGui::PopID();

		return SelectItem;
	}

} // namespace Editor
