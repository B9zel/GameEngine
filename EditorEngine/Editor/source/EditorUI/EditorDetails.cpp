#include "Editor/includes/EditorUI/EditorDetails.h"

#include <imgui/imgui.h>
#include <Runtime/CoreObject/Include/Object.h> 
#include <ReflectionSystem/Include/PropertyField.h>
#include <Math/includes/Transform.h>
#include <Runtime/includes/SceneComponent.h>
#include <Editor/includes/Util/DrawUtils.h>


namespace CoreEngine::Reflection
{
	struct SimplePropertyTypeField;
}

namespace Utils
{
	String ConvertToString(int64 Number);
}

namespace Editor
{
	void EditorDetails::Draw()
	{
		HasEditorRender.clear();
		ImGui::Begin("Details");

		if (!SelectedObject)
		{
			ImGui::Text("No object selected.");
			ImGui::End();
			return;
		}
		if (ImGui::TreeNodeEx(SelectedObject->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DrawLinesFull))
		{
			DrawDetailsRecursive(SelectedObject, nullptr, true);

			ImGui::TreePop();
		}


		ImGui::End();
	}

	void EditorDetails::DrawDetailsRecursive(CoreEngine::Runtime::Object* SelectedObject, CoreEngine::Runtime::Object* SourceClass, bool IsDrawTree)
	{


		auto* ClassInfo = SelectedObject->GetClass();
		auto* MainClass = ClassInfo;
		if (!HasAnyPropertyDeep(ClassInfo)) return;
		bool IsOpen = true;
		if (IsDrawTree && SourceClass)
		{
			IsOpen = ImGui::TreeNodeEx(SourceClass->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed);
		}

		if (!IsOpen) return;
		
		
		while (ClassInfo != nullptr)
		{
			
			if (HasAnyProperty(ClassInfo))
			{
				for (const auto& Property : ClassInfo->PropertyFileds)
				{
					if (SourceClass && Property->GetIsPointer()) continue;

					DrawProperty(Property, SelectedObject, MainClass, SourceClass);
				}
			}
				
			ClassInfo = ClassInfo->ParentClass;
		}
		
		if (IsDrawTree && SourceClass)
		{
			ImGui::TreePop();
		}
		

	}

	void EditorDetails::DrawProperty(CoreEngine::Reflection::PropertyField* Property, CoreEngine::Runtime::Object* SelectedObject, CoreEngine::Reflection::ClassField* MainClass, CoreEngine::Runtime::Object* SourceClass)
	{
		static float WidthColumn = 150;

		if (!HasFlag(static_cast<uint32>(Property->Params), static_cast<uint32>(CoreEngine::Reflection::EPropertyFieldParams::EditorVisible)))
		{
			if (Property->GetPrimitiveType() == CoreEngine::Reflection::EConteinType::ARRAY)
			{
				if (auto* ArrayProperty = dynamic_cast<CoreEngine::Reflection::ArrayPropertyField*>(Property))
				{
					for (int64 i = 0; i < ArrayProperty->GetSizeArray<CoreEngine::Runtime::Object*>(SelectedObject); i++)
					{
						auto* Property = *ArrayProperty->GetElement<CoreEngine::Runtime::Object*>(SelectedObject, i);
						auto* Source = SourceClass ? SourceClass : Property;
						DrawDetailsRecursive(Property, Source);
					}
				}
			}
			else if (Property->GetPrimitiveType() == CoreEngine::Reflection::EConteinType::PRIMITIVE)
			{
				if (auto* ComplexProperty = dynamic_cast<CoreEngine::Reflection::ComplexPropertyTypeField*>(Property))
				{
					auto* ComplexInstanceProperty = *Property->GetSourcePropertyByName<CoreEngine::Runtime::Object*>(SelectedObject);
					auto* Source = SourceClass ? SourceClass : ComplexInstanceProperty;
					DrawDetailsRecursive(ComplexInstanceProperty, Source);
				}
			}
			return;
		}
		
		if (Property->GetTypeProperty()->GetTypeOfPropertyType() == CoreEngine::Reflection::ETypeOfPropertyType::SIMPLE)
		{
			//ImGui::BeginChild(("#sidebar" + ClassInfo->Name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeX);
			if (auto* SimpleType = dynamic_cast<CoreEngine::Reflection::SimplePropertyTypeField*>(Property->GetTypeProperty()))
			{
				ImGui::PushID((Property->Name + Utils::ConvertToString(SelectedObject->GetUUID().GetID())).c_str());
				switch (SimpleType->Primitive)
				{
				case CoreEngine::Reflection::EPrimitiveTypes::INT8:
				{
					int8 Value = *Property->GetSourcePropertyByName<int8>(SelectedObject);

					static const int8 min = std::numeric_limits<int8>::min();
					static const int8 max = std::numeric_limits<int8>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawInt8(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					
					Property->SetSourceProperty<int8>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::INT16:
				{
					int16 Value = *Property->GetSourcePropertyByName<int16>(SelectedObject);

					static const int16 min = std::numeric_limits<int16>::min();
					static const int16 max = std::numeric_limits<int16>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawInt16(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					
					Property->SetSourceProperty<int16>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::INT32:
				{
					int32 Value = *Property->GetSourcePropertyByName<int32>(SelectedObject);


					//ImGui::InputScalar(Property->Name.c_str(), ImGuiDataType_S32, &Valur);
					static const int32 min = std::numeric_limits<int32>::min();
					static const int32 max = std::numeric_limits<int32>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawInt32(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					

					Property->SetSourceProperty<int32>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::INT64:
				{
					int64 Value = *Property->GetSourcePropertyByName<int64>(SelectedObject);

					static const int64 min = std::numeric_limits<int64>::min();
					static const int64 max = std::numeric_limits<int64>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;
					DrawInt64(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					
					Property->SetSourceProperty<int64>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT8:
				{
					uint8 Value = *Property->GetSourcePropertyByName<uint8>(SelectedObject);

					static const uint8 min = std::numeric_limits<uint8>::min();
					static const uint8 max = std::numeric_limits<uint8>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawUInt8(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					
					Property->SetSourceProperty<uint8>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT16:
				{
					uint16 Value = *Property->GetSourcePropertyByName<uint16>(SelectedObject);

					static const uint16 min = std::numeric_limits<uint16>::min();
					static const uint16 max = std::numeric_limits<uint16>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawUInt16(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					
					Property->SetSourceProperty<uint16>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT32:
				{
					uint32 Value = *Property->GetSourcePropertyByName<uint32>(SelectedObject);

					static const uint32 min = std::numeric_limits<uint32>::min();
					static const uint32 max = std::numeric_limits<uint32>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawUInt32(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					
					Property->SetSourceProperty<uint32>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT64:
				{
					uint64 Value = *Property->GetSourcePropertyByName<uint64>(SelectedObject);

					static const uint64 min = std::numeric_limits<uint64>::min();
					static const uint64 max = std::numeric_limits<uint64>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawUInt64(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					
					Property->SetSourceProperty<uint64>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_SINGLE:
				{
					float Value = *Property->GetSourcePropertyByName<float>(SelectedObject);

					static const float min = std::numeric_limits<float>::min();
					static const float max = std::numeric_limits<float>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawFloat(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					
					Property->SetSourceProperty<float>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_DOUBLE:
				{
					double Value = *Property->GetSourcePropertyByName<double>(SelectedObject);

					static const double min = std::numeric_limits<double>::min();
					static const double max = std::numeric_limits<double>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawDouble(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);
					
					Property->SetSourceProperty<double>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::STRING:
				{
					String& Value = *Property->GetSourcePropertyByName<String>(SelectedObject);
					auto* Object = SourceClass ? SourceClass : SelectedObject;
		
					DrawString(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, 256, WidthColumn);
				
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::BOOL:
				{
					bool* Value = Property->GetSourcePropertyByName<bool>(SelectedObject);

					DrawBool(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, *Value, WidthColumn);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::CHAR:
				{
					signed char Value = *Property->GetSourcePropertyByName<char>(SelectedObject);

					static const uint8 min = std::numeric_limits<char>::min();
					static const uint8 max = std::numeric_limits<char>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawInt8(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					Property->SetSourceProperty<char>(SelectedObject, Value);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::VECTOR3F:
				{
					FVector& Value = *Property->GetSourcePropertyByName<FVector>(SelectedObject);

					DrawVector3(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, WidthColumn);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::TRANSFORM:
				{
					FTransform Value = *Property->GetSourcePropertyByName<FTransform>(SelectedObject);

					DrawTransform(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value.GetLocationRef(), Value.GetRotationRef(), Value.GetScaleRef(), WidthColumn);

					if (auto* SceneComponent = dynamic_cast<CoreEngine::Runtime::SceneComponent*>(SelectedObject))
					{
						SceneComponent->SetTransform(Value);
					}

					break;
				}
				default:
					break;
				}
			}
			ImGui::Dummy(ImVec2(0, 0.5));
			ImGui::PopID();
		}
	}

	bool EditorDetails::HasAnyPropertyDeep(CoreEngine::Reflection::ClassField* Class)
	{
		while (Class != nullptr)
		{
			if (!Class->PropertyFileds.empty())
			{
				return true;
			}
			Class = Class->ParentClass;
		}
		return false;
	}

	bool EditorDetails::HasAnyProperty(CoreEngine::Reflection::ClassField* Class)
	{
		return !Class->PropertyFileds.empty();
	}

	void EditorDetails::SetSelectableObject(CoreEngine::Runtime::Object* Object)
	{
		SelectedObject = Object;
	}
}