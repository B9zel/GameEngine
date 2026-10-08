#include <Editor/includes/Asset/EditorMaterialAsset.h>

#include <Render/includes/MaterialInterface.h>
#include <Editor/includes/Utills/DrawUtills.h>
#include <imgui.h>

EditorMaterialAsset::EditorMaterialAsset(const CoreEngine::InitializeObject& Initilize) : MaterialAsset(Initilize)
{
}

void EditorMaterialAsset::DrawElements()
{
	// Draw the material properties in the editor
	for (auto& property : GetShaderUniforms())
	{
		if (property)
		{
			ImGui::Text("Property Name: %s", property->Name.c_str());
			switch (property->Type)
			{
			case EUniformType::FLOAT:
				Editor::DrawFloat(property->Name.c_str(), property->Name.c_str(), *property->GetProperty<float>());
				break;
			case EUniformType::INT:
				Editor::DrawInt32(property->Name.c_str(), property->Name.c_str(), *property->GetProperty<int32>(), std::numeric_limits<int32>::max(),
								  std::numeric_limits<int32>::lowest(), 100.0f);
				break;
			case EUniformType::UINT:
				Editor::DrawUInt32(property->Name.c_str(), property->Name.c_str(), *property->GetProperty<uint32>());
				break;
			case EUniformType::VEC3:
				Editor::DrawVector3(property->Name.c_str(), property->Name.c_str(), *property->GetProperty<FVector>());
				break;
			case EUniformType::MAT4:
				Editor::DrawMatrix4x4(property->Name.c_str(), property->Name.c_str(), *property->GetProperty<FMatrix4x4>());
				break;
			default:
				ImGui::Text("Unsupported property type");
				break;
			}
		}
	}
}
