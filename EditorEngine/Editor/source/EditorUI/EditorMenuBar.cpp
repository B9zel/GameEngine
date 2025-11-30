#include <Editor/includes/EditorUI/EditorMenuBar.h>
#include <Editor/includes/EditorApplication.h>
#include <Editor/includes/EditorEngine.h>
#include <Core/includes/World.h>
#include <Core/includes/Memory/SaveManager.h>
#include <imgui.h>

namespace Editor
{
	void EditorMenuBar::Draw()
	{
		
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					OwnerEditor->GetWorld()->GetSaveManager()->SaveScene("../Scene.json");
				}
				if (ImGui::MenuItem("Load"))
				{
					OwnerEditor->SetSelectedObject(nullptr);
					OwnerEditor->GetWorld()->GetSaveManager()->LoadSaveScene("../Scene.json");
				}
				if (ImGui::MenuItem("Exit"))
				{
					EditorApplication::Get()->Exit();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}