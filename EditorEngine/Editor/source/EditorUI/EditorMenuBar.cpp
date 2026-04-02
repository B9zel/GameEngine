#include <Editor/includes/EditorUI/EditorMenuBar.h>
#include <Editor/includes/EditorApplication.h>
#include <Editor/includes/EditorEngine.h>
#include <Core/includes/World.h>
#include <Core/includes/Memory/SaveManager.h>
#include <Core/includes/Application.h>
#include <Core/includes/Window.h>
#include <imgui.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <windows.h>
#include <commapi.h>
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>
#include <commdlg.h>

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
					const String Path = OpenFileDialoge("Reflect engine files (*.reflect)\0*.reflect\0");
					if (!Path.empty())
					{
						OwnerEditor->SetSelectedObject(nullptr);
						OwnerEditor->GetWorld()->GetSaveManager()->LoadSaveScene(Path); //"../Scene.json");
					}
				}
				if (ImGui::MenuItem("Save As"))
				{
					const String Path = SaveFileDialogeMenu("Reflect engine files (*.reflect)\0*.reflect\0");
					if (!Path.empty())
					{
						OwnerEditor->GetWorld()->GetSaveManager()->SaveScene(Path); //"../Scene.json");
					}
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
		OPENFILENAMEA Ofn;
		CHAR sizeFile[260] = {0};
		ZeroMemory(&Ofn, sizeof(OPENFILENAMEA));
		Ofn.lStructSize = sizeof(OPENFILENAMEA);
		Ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)CoreEngine::Application::Get()->GetWindow().GetNativeWindow());
		Ofn.lpstrFile = sizeFile;
		Ofn.nMaxFile = sizeof(sizeFile);
		Ofn.lpstrFilter = Filter;
		Ofn.lpstrDefExt = "reflect";
		Ofn.nFilterIndex = 1;
		Ofn.Flags = OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileName(&Ofn) == TRUE)
		{
			return Ofn.lpstrFile;
		}
		return String();
	}
	String EditorMenuBar::OpenFileDialoge(const char* Filter)
	{
		OPENFILENAMEA Ofn;
		CHAR sizeFile[260] = {0};
		ZeroMemory(&Ofn, sizeof(OPENFILENAMEA));
		Ofn.lStructSize = sizeof(OPENFILENAMEA);
		Ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)CoreEngine::Application::Get()->GetWindow().GetNativeWindow());
		Ofn.lpstrFile = sizeFile;
		Ofn.nMaxFile = sizeof(sizeFile);
		Ofn.lpstrFilter = Filter;

		Ofn.nFilterIndex = 1;
		Ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileName(&Ofn) == TRUE)
		{
			return Ofn.lpstrFile;
		}
		return String();
	}
} // namespace Editor
