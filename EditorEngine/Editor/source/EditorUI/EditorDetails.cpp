#include "Editor/includes/EditorUI/EditorDetails.h"

#include <imgui/imgui.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <ReflectionSystem/Include/PropertyField.h>
#include <Math/includes/Transform.h>
#include <Runtime/includes/SceneComponent.h>
#include <Editor/includes/Util/DrawUtils.h>
#include <Runtime/includes/Actor.h>
#include <Runtime/includes/ActorComponent.h>
#include <Editor/includes/EditorEngine.h>
#include <Editor/includes/EditorDrawingInterface.h>
#include <Editor/includes/Util/EditorUtil.h>
#include <Runtime/CoreObject/Include/Object.h>

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

		const uint32 MaxSizeName = 128;
		StaticArray<char, MaxSizeName> NewName;
		std::fill(NewName.data(), NewName.data() + NewName.size(), '\0');
		const String& Name = SelectedObject->GetName();
		// std::copy(Name.data(), Name.data() + Name.size(), NewName.data());
		float Pos = ImGui::GetCursorPosX();
		ImGui::Text(Name.c_str());
		// ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowSize().x - ImGui::GetCursorPos().x - Pos, 0.0));

		/*{
			if (NewName.front() != '\0')
			{
				SelectedObject->SetName(NewName.data());
			}
		}*/

		if (SelectedObject && SelectedObject->GetClass()->IsChildClassOf(Actor::GetStaticClass()))
		{
			const float WidthButton = 150;

			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - WidthButton);

			if (ImGui::Button("Add component", ImVec2(WidthButton, 0)))
			{
				ImGui::OpenPopup("Add component");
			}
			if (ImGui::BeginPopup("Add component"))
			{
				auto& Data = CoreEngine::Reflection::MapRegistryClass::Instance().GetData();
				for (auto& El : Data)
				{
					auto* MetaClass = El.second.CreateMetaClass.Invoke();
					if (MetaClass && !HasFlag(MetaClass->ParamFlags, EClassFieldParams::EditorComponent) || MetaClass->Name.empty()) continue;

					if (ImGui::MenuItem(MetaClass->Name.c_str()))
					{
						auto* ClassData = dynamic_cast<CoreEngine::Reflection::ClassField*>(MetaClass);
						auto* SelectedActor = dynamic_cast<Actor*>(SelectedObject);
						DArray<ActorComponent*>& Components = SelectedActor->FindComponentsByClass(ClassData);
						SelectedActor->CreateSubObject(ClassData, MetaClass->Name + std::to_string(Components.size()));
					}
				}
				ImGui::EndPopup();
			}
		}

		// if (ImGui::TreeNodeEx(SelectedObject->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
		// ImGuiTreeNodeFlags_DrawLinesFull))
		{
			ImGui::Separator();

			DrawDetailsRecursive(SelectedObject, nullptr, true);

			if (SelectedObject->GetClass()->IsChildClassOf(Asset::GetStaticClass()))
			{
				auto* asset = dynamic_cast<IEditorDrawingInterface*>(SelectedObject);
				asset->DrawElements();
			}

			// ImGui::TreePop();
		}

		ImGui::End();
	}

	void EditorDetails::OnConstruct()
	{
	}

	void EditorDetails::DrawDetailsRecursive(Object* SelectedObject, Object* SourceClass, bool IsDrawTree)
	{

		auto* ClassInfo = SelectedObject->GetClass();
		auto* MainClass = ClassInfo;
		if (!HasAnyPropertyDeep(ClassInfo)) return;
		bool IsOpen = true;
		if (IsDrawTree && SourceClass)
		{
			IsOpen = ImGui::TreeNodeEx(SourceClass->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed);
		}

		DrawComponentContextDraw(OwnerEditor, SelectedObject);

		if (!IsOpen) return;

		while (ClassInfo != nullptr)
		{

			if (HasAnyProperty(ClassInfo))
			{
				for (const auto& Property : ClassInfo->PropertyFileds)
				{
					if ((HasFlag(static_cast<uint32>(Property->Params), static_cast<uint32>(CoreEngine::Reflection::EPropertyFieldParams::EditorVisible)) ||
						 SelectedObject->GetClass()->IsChildClassOf(Actor::GetStaticClass())))
					{
						DrawProperty(Property, SelectedObject, MainClass, SourceClass);
					}
				}
			}

			ClassInfo = ClassInfo->ParentClass;
		}

		if (IsDrawTree && SourceClass)
		{
			ImGui::TreePop();
		}
	}

	void EditorDetails::DrawProperty(CoreEngine::Reflection::PropertyField* Property, Object* SelectedObject, CoreEngine::Reflection::ClassField* MainClass,
									 Object* SourceClass)
	{
		static float WidthColumn = 150;

		if (!HasFlag(static_cast<uint32>(Property->Params), static_cast<uint32>(CoreEngine::Reflection::EPropertyFieldParams::EditorVisible)))
		{
			if (Property->GetPrimitiveType() == CoreEngine::Reflection::EConteinType::ARRAY)
			{
				if (auto* ArrayProperty = dynamic_cast<CoreEngine::Reflection::ArrayPropertyField*>(Property))
				{
					for (int64 i = 0; i < ArrayProperty->GetSizeArray<Object*>(SelectedObject); i++)
					{
						auto* Property = *ArrayProperty->GetElement<Object*>(SelectedObject, i);
						auto* Source = SourceClass ? SourceClass : Property;
						DrawDetailsRecursive(Property, Source);
					}
				}
			}
			else if (Property->GetPrimitiveType() == CoreEngine::Reflection::EConteinType::PRIMITIVE)
			{
				if (auto* ComplexProperty = dynamic_cast<CoreEngine::Reflection::ComplexPropertyTypeField*>(Property))
				{
					auto* ComplexInstanceProperty = *Property->GetSourcePropertyByName<Object*>(SelectedObject);
					auto* Source = SourceClass ? SourceClass : ComplexInstanceProperty;
					DrawDetailsRecursive(ComplexInstanceProperty, Source);
				}
			}
			return;
		}

		if (Property->GetTypeProperty()->GetTypeOfPropertyType() == CoreEngine::Reflection::ETypeOfPropertyType::SIMPLE)
		{
			// ImGui::BeginChild(("#sidebar" + ClassInfo->Name).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0), ImGuiChildFlags_AutoResizeX);
			if (auto* SimpleType = dynamic_cast<CoreEngine::Reflection::SimplePropertyTypeField*>(Property->GetTypeProperty()))
			{
				ImGui::PushID((Property->Name + Utils::ConvertToString(SelectedObject->GetUUID().GetID())).c_str());
				switch (SimpleType->Primitive)
				{
				case CoreEngine::Reflection::EPrimitiveTypes::INT8:
				{
					int8 Value = *Property->GetSourcePropertyByName<int8>(SelectedObject);
					int8 OldValue = Value;

					static const int8 min = std::numeric_limits<int8>::min();
					static const int8 max = std::numeric_limits<int8>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawInt8(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<int8>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::INT16:
				{
					int16 Value = *Property->GetSourcePropertyByName<int16>(SelectedObject);
					int16 OldValue = Value;

					static const int16 min = std::numeric_limits<int16>::min();
					static const int16 max = std::numeric_limits<int16>::max();

					DrawInt16(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<int16>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::INT32:
				{
					int32 Value = *Property->GetSourcePropertyByName<int32>(SelectedObject);
					int32 OldValue = Value;

					// ImGui::InputScalar(Property->Name.c_str(), ImGuiDataType_S32, &Valur);
					static const int32 min = std::numeric_limits<int32>::min();
					static const int32 max = std::numeric_limits<int32>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawInt32(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<int32>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::INT64:
				{
					int64 Value = *Property->GetSourcePropertyByName<int64>(SelectedObject);
					int64 OldValue = Value;

					static const int64 min = std::numeric_limits<int64>::min();
					static const int64 max = std::numeric_limits<int64>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;
					DrawInt64(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<int64>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT8:
				{
					uint8 Value = *Property->GetSourcePropertyByName<uint8>(SelectedObject);
					uint8 OldValue = Value;

					static const uint8 min = std::numeric_limits<uint8>::min();
					static const uint8 max = std::numeric_limits<uint8>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawUInt8(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<uint8>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT16:
				{
					uint16 Value = *Property->GetSourcePropertyByName<uint16>(SelectedObject);
					uint16 OldValue = Value;

					static const uint16 min = std::numeric_limits<uint16>::min();
					static const uint16 max = std::numeric_limits<uint16>::max();

					DrawUInt16(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<uint16>(SelectedObject, Property, Value, OldValue);

					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT32:
				{
					uint32 Value = *Property->GetSourcePropertyByName<uint32>(SelectedObject);
					uint32 OldValue = Value;

					static const uint32 min = std::numeric_limits<uint32>::min();
					static const uint32 max = std::numeric_limits<uint32>::max();

					DrawUInt32(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<uint32>(SelectedObject, Property, Value, OldValue);

					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT64:
				{
					uint64 Value = *Property->GetSourcePropertyByName<uint64>(SelectedObject);
					uint64 OldValue = Value;

					static const uint64 min = std::numeric_limits<uint64>::min();
					static const uint64 max = std::numeric_limits<uint64>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawUInt64(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<uint64>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_SINGLE:
				{
					float Value = *Property->GetSourcePropertyByName<float>(SelectedObject);
					float OldValue = Value;

					static const float min = std::numeric_limits<float>::min();
					static const float max = std::numeric_limits<float>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawFloat(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<float>(SelectedObject, Property, Value, OldValue);

					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_DOUBLE:
				{
					double Value = *Property->GetSourcePropertyByName<double>(SelectedObject);
					double OldValue = Value;

					static const double min = std::numeric_limits<double>::min();
					static const double max = std::numeric_limits<double>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawDouble(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<double>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::STRING:
				{
					String& Value = *Property->GetSourcePropertyByName<String>(SelectedObject);
					static String OldValue;
					OldValue = Value;

					DrawString(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, 256, WidthColumn);

					SetPropertyValue<String>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::BOOL:
				{
					bool Value = *Property->GetSourcePropertyByName<bool>(SelectedObject);
					bool OldValue = Value;

					DrawBool(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, WidthColumn);

					SetPropertyValue<bool>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::CHAR:
				{
					signed char Value = *Property->GetSourcePropertyByName<char>(SelectedObject);
					signed char OldValue = Value;

					static const uint8 min = std::numeric_limits<char>::min();
					static const uint8 max = std::numeric_limits<char>::max();
					auto* Object = SourceClass ? SourceClass : SelectedObject;

					DrawInt8(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, max, min, WidthColumn);

					SetPropertyValue<char>(SelectedObject, Property, Value, OldValue);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::VECTOR3F:
				{
					FVector Value = *Property->GetSourcePropertyByName<FVector>(SelectedObject);
					FVector OldValue = Value;

					DrawVector3(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, WidthColumn);

					SetPropertyValue<FVector>(SelectedObject, Property, Value, OldValue);

					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::TRANSFORM:
				{
					FTransform Value = *Property->GetSourcePropertyByName<FTransform>(SelectedObject);
					FTransform OldValue = Value;

					DrawTransform(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value.GetLocationRef(), Value.GetRotationRef(),
								  Value.GetScaleRef(), WidthColumn);

					if (auto* sceneComponent = dynamic_cast<SceneComponent*>(SelectedObject))
					{
						SetPropertyValue<FTransform>(SelectedObject, Property, Value, OldValue);
					}

					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::COLOR:
				{
					LinearColor Value = *Property->GetSourcePropertyByName<LinearColor>(SelectedObject);
					LinearColor OldValue = Value;

					DrawColor(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, Value, WidthColumn);

					SetPropertyValue<LinearColor>(SelectedObject, Property, Value, OldValue);
				}
				default:
					break;
				}
			}
			ImGui::Dummy(ImVec2(0, 0.5));
			ImGui::PopID();
		}
		else
		{
			DrawPointerProperty(Property, SelectedObject, MainClass, SourceClass);
		}
	}

	void EditorDetails::DrawPointerProperty(CoreEngine::Reflection::PropertyField* Property, Object* SelectedObject,
											CoreEngine::Reflection::ClassField* MainClass, Object* SourceClass)
	{
		if (!Property->GetIsPointer() ||
			!HasFlag(static_cast<uint32>(Property->Params), static_cast<uint32>(CoreEngine::Reflection::EPropertyFieldParams::EditorVisible)))
			return;

		Object** StoreObj = Property->GetSourcePropertyByName<Object*>(SelectedObject);
		// if (!StoreObj || !(*StoreObj) || !(*StoreObj)->GetClass()->IsChildClassOf(Asset::GetStaticClass()))
		{
			// return;
		}

		static DArray<Asset*> AvailableAssets;
		AvailableAssets.clear();

		if (Property->GetTypeProperty()->GetTypeOfPropertyType() == CoreEngine::Reflection::ETypeOfPropertyType::COMPLEX)
		{
			// if (!(*Property->GetSourcePropertyByName<Object*>(SelectedObject))->GetClass()->IsChildClassOf(Asset::GetStaticClass())) return;

			Asset** Value = (Property->GetSourcePropertyByName<Asset*>(SelectedObject));
			int32 SelectElementIndex = -1;
			if (Value)
			{
				auto* ComplexType = dynamic_cast<CoreEngine::Reflection::ComplexPropertyTypeField*>(Property->GetTypeProperty());
				const auto& AllLoadedAssets = AssetManager::Get().GetLoadedAssets();

				for (uint64 i = 0; i < AllLoadedAssets.size(); i++)
				{
					if (AllLoadedAssets[i]->GetClass()->IsChildClassOf(static_cast<CoreEngine::Reflection::ClassField*>(ComplexType->GetTypeFiled())))
					{
						if ((*Value) == AllLoadedAssets[i])
						{
							SelectElementIndex = i;
						}

						AvailableAssets.push_back(AllLoadedAssets[i]);
					}
				}
			}

			Asset* OldValue = *Value;
			int32 NewItem = DrawComboBox(Utils::ConvertToString(SelectedObject->GetUUID().GetID()), Property->Name, *Value ? (*Value)->GetName() : "null",
										 AvailableAssets, SelectElementIndex, 180);
			if (NewItem >= 0)
			{
				SetPropertyValue(SelectedObject, Property, AvailableAssets[NewItem], OldValue);

				(*Value) = AvailableAssets[NewItem];
			}
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

	void EditorDetails::SetSelectableObject(Object* Object)
	{
		SelectedObject = Object;
	}
} // namespace Editor
