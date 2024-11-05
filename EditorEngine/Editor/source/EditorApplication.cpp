#include "../EditorEngine/Editor/includes/EditorApplication.h"
#include <Core/includes/Window.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace Editor
{
	EditorApplication::EditorApplication(const CoreEngine::ApplicationOptions& option) : CoreEngine::Application(option)
	{

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)GetWindow().GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init();


		PushLayer(new EditorLayer());
	}

}