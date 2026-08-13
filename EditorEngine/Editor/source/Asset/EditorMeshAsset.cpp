#include <Editor/includes/Asset/EditorMeshAsset.h>
#include <Editor/includes/EditorUI/EditorPanelUtills.h>
#include <Editor/includes/Utills/EditorUtill.h>
#include <imgui/imgui.h>

DECLARE_LOG_CATEGORY_EXTERN(EDITOR_MESH_ASSET_LOG);

EditorMeshAsset::EditorMeshAsset(const CoreEngine::InitializeObject& Initilize) : MeshAsset(Initilize)
{
}

void EditorMeshAsset::DrawElements()
{
	ImGui::Text("Path to model: %s", GetPathToModel().c_str());
	ImGui::Separator();
	if (ImGui::Button("Load model", ImVec2(ImGui::GetWindowSize().x, 0)))
	{
		String Path = OpenFileDialoge("3D model files (*.fbx, *.obj, *.dae)\0*.fbx;*.obj;*.dae\0");

		if (auto* Field = GetClass()->GetPropertyFieldByName(this, "PathToModel"))
		{
			Editor::SetPropertyValue(this, Field, Path, GetPathToModel());
		}
		else
		{
			EG_LOG(EDITOR_MESH_ASSET_LOG, ELevelLog::ERROR, "Property field 'PathToModel' not found in class '{0}'", GetClass()->Name);
		}
	}
}
