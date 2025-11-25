#include "Editor/includes/Util/DrawUtils.h"
#include "Math/includes/Matrix.h"
#include "imgui_internal.h"
//#include <imgui.h>


namespace Editor
{
	uint64 CalculateSizeOfPropertyName(const String& Str, const float MaxWidth)
	{
		float Avail = MaxWidth - ImGui::CalcTextSize("...").x - ImGui::GetStyle().ItemSpacing.x;

		ImVec2 TextSize = ImGui::CalcTextSize(Str.c_str());

		if (TextSize.x <= Avail)
		{
			return Str.size();
		}
		else
		{
			uint64 Size = Str.size();
			int64 Low = 0;
			int64 High = Size;
			while (Low < High)
			{
				uint64 mid = (Low + High + 1) / 2;
				ImVec2 FillSize = ImGui::CalcTextSize((Str.substr(0, mid) + "...").c_str());
				if (FillSize.x <= Avail)
				{
					Low = mid;
				}
				else
				{
					High = mid - 1;
				}
			}
			return Low;
		}
	}




	template<class T>
	static void DrawScalar(const String& Id, ImGuiDataType Type, const String& Name, T& Scalar, T Max, T Min, const float ColumnWidth)
	{
		ImGui::PushID(Id.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, ColumnWidth);

		float ColWidth = ImGui::GetColumnWidth(0);
		
		ImVec2 TextSize = ImGui::CalcTextSize(Name.c_str());
		uint64 Size = CalculateSizeOfPropertyName(Name, ColWidth);
		if (Size == Name.size())
		{
			ImGui::TextUnformatted(Name.c_str());
		}
		else
		{
			ImGui::TextUnformatted((Name.substr(0, Size) + "...").c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("%s", Name.c_str());
			}
		}

		/*if (TextSize.x <= Avail)
		{
			ImGui::TextUnformatted(Name.c_str());
		}
		else
		{
			uint64 Size = Name.size();
			int64 Low = 0;
			int64 High = Size;
			while (Low < High)
			{
				uint64 mid = (Low + High + 1) / 2;
				ImVec2 FillSize = ImGui::CalcTextSize((Name.substr(0, mid) + "...").c_str());
				if (FillSize.x <= Avail)
				{
					Low = mid;
				}
				else
				{
					High = mid - 1;
				}
			}
			ImGui::TextUnformatted((Name.substr(0, Low) + "...").c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("%s", Name.c_str());
			}
		}*/


		ImGui::NextColumn();
		ImGui::AlignTextToFramePadding();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		//ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushItemWidth(-FLT_MIN);


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

	void DrawVector3(const String& Id, const String& NameOfVec, FVector& Vector, const float ColumnWidht)
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
		
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 0, 0, 1));
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

		if (ImGui::TreeNodeEx(NameOfTransform.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{	
			DrawVector3(Id, "Location", Location, ColumnWidth);
			DrawVector3(Id, "Rotation", Rotation,  ColumnWidth);
			DrawVector3(Id, "Scale", Scale, ColumnWidth);

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
	void DrawString(const String& Id, const String& NameString, String& SourceStr,const uint32 MaxBufferSize, const float ColumnWidth)
	{
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
		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		//ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		SourceStr.reserve(MaxBufferSize);
		
		ImGui::InputText(("##" + NameString).c_str(), SourceStr.data(), MaxBufferSize);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	void DrawBool(const String& Id, const String& NameString, bool& Value, const float ColumnWidth)
	{
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
		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
		//ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		

		ImGui::Checkbox(("##" + NameString).c_str(), &Value);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		ImGui::PopID();
	}
	
}