#include "../EditorEngine/Editor/includes/EditorApplication.h"
#include <Core/includes/Window.h>
#include <Core/includes/World.h>
#include <Render/includes/Render.h>
#include <Core/includes/InputDevice.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <imgui/imgui.h>
#include <Editor/includes/EditorEngine.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui_internal.h>


namespace Editor
{
	EditorApplication::EditorApplication(CoreEngine::ApplicationOptions& option) : CoreEngine::Application(option)
	{
		//ConstructEngine();




		//PushLayer(new EditorLayer());
	}


	EditorApplication::~EditorApplication()
	{
		ImGui_ImplOpenGL3_Shutdown();

	}


	void EditorApplication::CreateApp()
	{
		Application::CreateApp();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)GetWindow().GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init();
	}

	void EditorApplication::ConstructEngine()
	{
		/*if (InstanceEngine)
		{
			return move(InstanceEngine);
		}*/
		InstanceEngine = MakeUniquePtr<EditorEngine>();
	}

	//UniquePtr<Engine> EditorApplication::ConstructEngine()
	//{
	//	return new EditorEngine();
	//}


}