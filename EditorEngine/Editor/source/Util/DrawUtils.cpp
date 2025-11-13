#include "Editor/includes/Util/DrawUtils.h"
#include "Math/includes/Matrix.h"
#include "imgui_internal.h"


namespace Editor
{
	template<class T>
	static void DrawScalar(const String& Id, ImGuiDataType Type, const String& Name, T& Scalar, T Max, T Min, const float ColumnWidth)
	{
		ImGui::PushID(Id.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, (8.9) * Name.size());

		//ImGui::SetNextItemWidth(ColumnWidht);
		ImGui::Text(Name.c_str());
		ImGui::NextColumn();
		//ImGui::SetColumnWidth(1, 150);
		ImGui::AlignTextToFramePadding();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 0));


		ImGui::DragScalar(("##" + Name).c_str(), Type, &Scalar, 1.0f, &Min, &Max);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::Columns(1);

		ImGui::PopID();
	}




	void DrawInt8(const String& Id, const String& Name, int8& Scalar, int8 Max, int8 Min, const float ColumnWidht)
	{
		DrawScalar(Id, ImGuiDataType_S8, Name, Scalar, Max, Min, ColumnWidht);
	}

	void DrawInt16(const String& Id, const String& Name, int16& Scalar, int16 Max, int16 Min, const float ColumnWidht)
	{
		DrawScalar(Id, ImGuiDataType_S16, Name, Scalar, Max, Min, ColumnWidht);
	}
	void DrawInt32(const String& Id, const String& Name, int32& Scalar, int32 Max, int32 Min, const float ColumnWidht)
	{
		DrawScalar(Id, ImGuiDataType_S32, Name, Scalar, Max, Min, ColumnWidht);
	}
	void DrawInt64(const String& Id, const String& Name, int64& Scalar, int64 Max, int64 Min, const float ColumnWidht)
	{
		DrawScalar(Id, ImGuiDataType_S64, Name, Scalar, Max, Min, ColumnWidht);
	}

	void DrawUInt8(const String& Id, const String& Name, uint8& Scalar, uint8 Max, uint8 Min, const float ColumnWidht)
	{
		Min = Math::Max<uint16>(0, Min);
		DrawScalar(Id, ImGuiDataType_U8, Name, Scalar, Max, Min, ColumnWidht);
	}

	void DrawUInt16(const String& Id, const String& Name, uint16& Scalar, uint16 Max, uint16 Min, const float ColumnWidht)
	{
		Min = Math::Max<uint16>(0, Min);
		DrawScalar(Id, ImGuiDataType_U16, Name, Scalar, Max, Min, ColumnWidht);
	}

	void DrawUInt32(const String& Id, const String& Name, uint32& Scalar, uint32 Max, uint32 Min, const float ColumnWidht)
	{
		Min = Math::Max<uint32>(0, Min);
		DrawScalar(Id, ImGuiDataType_U32, Name, Scalar, Max, Min, ColumnWidht);
	}

	void DrawUInt64(const String& Id, const String& Name, uint64& Scalar, uint64 Max, uint64 Min, const float ColumnWidht)
	{
		Min = Math::Max<uint64>(0, Min);
		DrawScalar(Id, ImGuiDataType_U64, Name, Scalar, Max, Min, ColumnWidht);
	}

	void DrawFloat(const String& Id, const String& Name, float& Scalar, float Max, float Min, const float ColumnWidht)
	{
		DrawScalar(Id, ImGuiDataType_Float, Name, Scalar, Max, Min, ColumnWidht);
	}

	void DrawDouble(const String& Id, const String& Name, double& Scalar, double Max, double Min, const float ColumnWidht)
	{
		DrawScalar(Id, ImGuiDataType_Double, Name, Scalar, Max, Min, ColumnWidht);
	}

	void DrawVector3(const String& Id, const String& NameOfVec, FVector& Vector, const ImVec4& Color, const float ColumnWidht)
	{
		ImGui::PushID(Id.c_str());
		ImGui::PushID(NameOfVec.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0,  ColumnWidht);
		ImGui::Text(NameOfVec.c_str());
		ImGui::NextColumn();
		ImGui::AlignTextToFramePadding();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing | ImGuiStyleVar_WindowBorderSize, ImVec2(10, 0));
		
		ImDrawList* DrawList = ImGui::GetWindowDrawList();
		ImVec2 Pos = ImGui::GetCursorScreenPos();
		ImVec2 PosMax = ImVec2(Pos.x + 15, Pos.y + 15);
		ImVec2 ScreePos = ImGui::GetCursorScreenPos();
		ScreePos.y += 5;
		/*DrawList->AddRect(ScreePos, PosMax, ImGui::GetColorU32(ImVec4(1.0, 0.0, 0.0, 1.f)), 3, ImDrawFlags_RoundCornersBottom);
		DrawList->AddText(ScreePos, ImGui::GetColorU32(ImVec4(1.0, 1.0, 1.0, 1.f)), "X");*/
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 0, 0, 1));
		//ImGui::SetCursorPosY(ScreePos.y);
		ImGui::Text("X");
		ImGui::PopStyleColor(2);
		

		ImGui::SameLine();
		ImGui::DragFloat(("##X" + NameOfVec).c_str(), &Vector.vector.x, 0.01f, 0, 0, "%.0003f");
		ImGui::PopItemWidth();
		


		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
		ImGui::Text("Y");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat(("##Y" + NameOfVec).c_str(), &Vector.vector.y, 0.01f, 0, 0, "%.0003f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 1, 1));
		ImGui::Text("Z");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::DragFloat(("##Z" + NameOfVec).c_str(), &Vector.vector.z, 0.01, 0, 0, "%.0003f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
		ImGui::PopID();
	}
	void DrawTransform(const String& Id, const String& NameOfTransform, FVector& Location, FVector& Rotation, FVector& Scale, const float ColumnWidth)
	{
		ImGui::PushID(Id.c_str());

		if (ImGui::TreeNodeEx(NameOfTransform.c_str()))
		{	

			DrawVector3(Id, "Location", Location, ImVec4(1, 0, 0, 1), ColumnWidth);
			DrawVector3(Id, "Rotation", Rotation, ImVec4(0, 1, 0, 1), ColumnWidth);
			DrawVector3(Id, "Scale", Scale, ImVec4(0, 0, 1, 1), ColumnWidth);

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}