#include "Editor/includes/EditorDetails.h"

#include <imgui/imgui.h>
#include <Runtime/CoreObject/Include/Object.h> 
#include <ReflectionSystem/Include/PropertyField.h>
#include <Math/includes/Transform.h>
#include <Runtime/includes/SceneComponent.h>
#include <Editor/includes/Util/DrawUtils.h>
//#include <Core/includes/StringUtil.h>

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
		ImGui::Begin("Details");

		if (!SelectedObject)
		{
			ImGui::Text("No object selected.");
			ImGui::End();
			return;
		}
		DrawDetailsRecursive(SelectedObject, nullptr, false);

		ImGui::End();
	}

	void EditorDetails::DrawDetailsRecursive(CoreEngine::Runtime::Object* SelectedObject, CoreEngine::Runtime::Object* SourceClass, bool IsDrawTree)
	{
		auto* ClassInfo = SelectedObject->GetClass();
		auto* MainClass = ClassInfo;
		if (!HasAnyProperty(ClassInfo)) return;
		bool IsOpen = true;
		if (IsDrawTree && SourceClass)
		{
			IsOpen = ImGui::TreeNodeEx(SourceClass->GetName().c_str());
		}

		if (!IsOpen) return;
		
		while (ClassInfo != nullptr)
		{
			for (const auto& Property : ClassInfo->PropertyFileds)
			{
				DrawProperty(Property, SelectedObject, MainClass, SourceClass);
			}
			ClassInfo = ClassInfo->ParentClass;
		}
		if (IsDrawTree)
		{
			ImGui::TreePop();
		}
		

	}

	void EditorDetails::DrawProperty(CoreEngine::Reflection::PropertyField* Property, CoreEngine::Runtime::Object* SelectedObject, CoreEngine::Reflection::ClassField* MainClass, CoreEngine::Runtime::Object* SourceClass)
	{
		if (HasFlag(static_cast<uint32>(Property->Params), static_cast<uint32>(CoreEngine::Reflection::EPropertyFieldParams::EditorVisible)))
		{
			if (Property->TypeProperty->GetTypeOfPropertyType() == CoreEngine::Reflection::ETypeOfPropertyType::SIMPLE)
			{
				//ImGui::BeginChild(("#sidebar" + ClassInfo->Name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeX);
				if (auto* SimpleType = dynamic_cast<CoreEngine::Reflection::SimplePropertyTypeField*>(Property->TypeProperty.get()))
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

						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawInt8(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}
						Property->SetSourceProperty<int8>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::INT16:
					{
						int16 Value = *Property->GetSourcePropertyByName<int16>(SelectedObject);

						static const int16 min = std::numeric_limits<int16>::min();
						static const int16 max = std::numeric_limits<int16>::max();
						auto* Object = SourceClass ? SourceClass : SelectedObject;

						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawInt16(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}
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

						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawInt32(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}



						/*ImGui::Text(Property->Name.c_str());
						ImGui::SameLine();
						ImGui::InputScalar(("##" + Property->Name).c_str(), ImGuiDataType_S32, &Value);
						ImGui::SameLine();
						ImGui::SliderScalar(("###" + Property->Name).c_str(), ImGuiDataType_S32, &Value, &min, &max);*/
						//ImGui::SameLine();


						Property->SetSourceProperty<int32>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::INT64:
					{
						int64 Value = *Property->GetSourcePropertyByName<int64>(SelectedObject);

						static const int64 min = std::numeric_limits<int64>::min();
						static const int64 max = std::numeric_limits<int64>::max();
						auto* Object = SourceClass ? SourceClass : SelectedObject;
						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawInt64(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}
						Property->SetSourceProperty<int64>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::UINT8:
					{
						uint8 Value = *Property->GetSourcePropertyByName<uint8>(SelectedObject);

						static const uint8 min = std::numeric_limits<uint8>::min();
						static const uint8 max = std::numeric_limits<uint8>::max();
						auto* Object = SourceClass ? SourceClass : SelectedObject;

						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawUInt8(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}
						Property->SetSourceProperty<uint8>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::UINT16:
					{
						uint16 Value = *Property->GetSourcePropertyByName<uint16>(SelectedObject);

						static const uint16 min = std::numeric_limits<uint16>::min();
						static const uint16 max = std::numeric_limits<uint16>::max();
						auto* Object = SourceClass ? SourceClass : SelectedObject;

						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawUInt16(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}
						Property->SetSourceProperty<uint16>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::UINT32:
					{
						uint32 Value = *Property->GetSourcePropertyByName<uint32>(SelectedObject);

						static const uint32 min = std::numeric_limits<uint32>::min();
						static const uint32 max = std::numeric_limits<uint32>::max();
						auto* Object = SourceClass ? SourceClass : SelectedObject;

						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawUInt32(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}
						Property->SetSourceProperty<uint32>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::UINT64:
					{
						uint64 Value = *Property->GetSourcePropertyByName<uint64>(SelectedObject);

						static const uint64 min = std::numeric_limits<uint64>::min();
						static const uint64 max = std::numeric_limits<uint64>::max();
						auto* Object = SourceClass ? SourceClass : SelectedObject;

						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawUInt64(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}
						Property->SetSourceProperty<uint64>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_SINGLE:
					{
						float Value = *Property->GetSourcePropertyByName<float>(SelectedObject);

						static const float min = std::numeric_limits<float>::min();
						static const float max = std::numeric_limits<float>::max();
						auto* Object = SourceClass ? SourceClass : SelectedObject;

						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawFloat(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}
						Property->SetSourceProperty<float>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_DOUBLE:
					{
						double Value = *Property->GetSourcePropertyByName<double>(SelectedObject);

						static const double min = std::numeric_limits<double>::min();
						static const double max = std::numeric_limits<double>::max();
						auto* Object = SourceClass ? SourceClass : SelectedObject;

						if (ImGui::TreeNode(Object->GetName().c_str()))
						{
							DrawDouble(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, 50);
							ImGui::TreePop();
						}
						Property->SetSourceProperty<double>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::STRING:
					{
						String& Value = *Property->GetSourcePropertyByName<String>(SelectedObject);

						ImGui::SameLine();
						char Buffer[256];
						std::strncpy(Buffer, Value.c_str(), sizeof(Buffer));
						if (ImGui::InputText(Property->Name.c_str(), Buffer, sizeof(Buffer)))
						{
							Property->SetSourceProperty<String>(SelectedObject, String(Buffer));
						}
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::BOOL:
					{
						bool Value = *Property->GetSourcePropertyByName<bool>(SelectedObject);

						ImGui::SameLine();
						ImGui::Checkbox(Property->Name.c_str(), &Value);
						Property->SetSourceProperty<bool>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::CHAR:
					{
						char Value = *Property->GetSourcePropertyByName<char>(SelectedObject);

						ImGui::SameLine();
						ImGui::InputScalar(Property->Name.c_str(), ImGuiDataType_S8, &Value);
						Property->SetSourceProperty<char>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::VECTOR3F:
					{
						FVector Value = *Property->GetSourcePropertyByName<FVector>(SelectedObject);

						ImGui::Text(Property->Name.c_str());
						ImGui::SameLine();
						ImGui::InputFloat3(("##" + Property->Name + MainClass->Name).c_str(), const_cast<float*>(GetValuePtr(Value.vector)));
						Property->SetSourceProperty<FVector>(SelectedObject, Value);
						break;
					}
					case CoreEngine::Reflection::EPrimitiveTypes::TRANSFORM:
					{

						FTransform Value = *Property->GetSourcePropertyByName<FTransform>(SelectedObject);

						DrawTransform(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value.GetLocationRef(), Value.GetRotationRef(), Value.GetScaleRef(), 100);

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
				ImGui::PopID();
				//ImGui::EndChild();	
			}
		}
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
	}

	bool EditorDetails::HasAnyProperty(CoreEngine::Reflection::ClassField* Class)
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

	void EditorDetails::SetSelectableObject(CoreEngine::Runtime::Object* Object)
	{
		SelectedObject = Object;
	}
}