#include <Editor/includes/EditorUI/EditorMenuBar.h>
#include <Editor/includes/EditorApplication.h>
#include <Editor/includes/EditorEngine.h>
#include <Core/includes/World.h>
#include <Core/includes/Memory/SaveManager.h>
#include <Core/includes/Application.h>
#include <Core/includes/Window.h>
#include <imgui.h>
#include <Editor/includes/EditorUI/EditorPanelUtills.h>

namespace Editor
{
	void EditorMenuBar::Draw()
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					const String Path = OpenFileDialogeMenu("Reflect engine files (*.reflect)\0*.reflect\0");
					if (!Path.empty())
					{
						OwnerEditor->SetSelectedObject(nullptr);
						OwnerEditor->GetWorld()->GetSaveManager()->LoadSaveScene(Path);
					}
				}
				if (ImGui::MenuItem("Save As"))
				{
					const String Path = SaveFileDialogeMenu("Reflect engine files (*.reflect)\0*.reflect\0");
					if (!Path.empty())
					{
						OwnerEditor->GetWorld()->GetSaveManager()->SaveScene(Path);
					}
				}
				if (ImGui::MenuItem("Save all"))
				{
					OwnerEditor->GetWorld()->GetSaveManager()->SaveContentItems();
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
	void EditorMenuBar::OnConstruct()
	{
	}
	String EditorMenuBar::SaveFileDialogeMenu(const char* Filter)
	{
		return SaveFileDialoge(Filter);
	}
	String EditorMenuBar::OpenFileDialogeMenu(const char* Filter)
	{
		return OpenFileDialoge(Filter);
	}
} // namespace Editor
