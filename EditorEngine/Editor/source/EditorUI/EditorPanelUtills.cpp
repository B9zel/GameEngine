#include <Editor/includes/EditorUI/EditorPanelUtills.h>
#include <Core/includes/Application.h>
#include <Core/includes/Window.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <windows.h>
#include <commapi.h>
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>
#include <commdlg.h>

String SaveFileDialoge(const char* Filter)
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

String OpenFileDialoge(const char* Filter)
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
