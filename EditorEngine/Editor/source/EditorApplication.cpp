#include "../EditorEngine/Editor/includes/EditorApplication.h"
#include <Editor/includes/EditorEngine.h>
#include <Core/includes/Window.h>
#include <imgui/imgui.h>
#include <imgui_internal.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <Core/includes/World.h>
#include <Core/includes/Memory/SaveManager.h>


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
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)GetWindow().GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init();
	}

	void EditorApplication::Start()
	{
		CoreEngine::Application::Start();

		InstanceEngine->GetWorld()->GetSaveManager()->SaveSceneSerializedData();
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