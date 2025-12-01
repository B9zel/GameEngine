
#include <Editor/includes/EditorEngine.h>
#include <imgui/imgui.h>
#include <Core/includes/World.h>
#include <Render/includes/Render.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <Core/includes/Application.h>
#include <Core/includes/Window.h>
#include <Render/includes/Framebuffer.h>
#include <Core/includes/Memory/SaveManager.h>
#include <Editor/includes/EditorViewportClient.h>
#include <Editor/includes/EditorUI/EditorMenuBar.h>
#include <Editor/includes/EditorWorld.h>
#include <Editor/includes/EditorUI/EditorToolbar.h>
#include <ImGuizmo/ImGuizmo.h>

namespace Editor
{
	using namespace CoreEngine;

	EditorEngine::EditorEngine()
	{
		using namespace CoreEngine::Render;

		CoreEngine::Render::FramebufferSpecification Spec;
		Spec.Height =  Application::Get()->GetWindow().GetHeight();
		Spec.Width = Application::Get()->GetWindow().GetWidth();
		Spec.AttachTextures.Textures.push_back(CoreEngine::Render::FramebufferTextureAttachment(EFramebufferTextureFormat::RGBA8));
		Spec.AttachTextures.Textures.push_back(CoreEngine::Render::FramebufferTextureAttachment(EFramebufferTextureFormat::RED_INTEGER));
		Spec.AttachTextures.Textures.push_back(CoreEngine::Render::FramebufferTextureAttachment(EFramebufferTextureFormat::DEPTH24_STENCIL8));

		FrameBuffer = CoreEngine::Render::Framebuffer::Create(Spec);

		// Create UI
		Viewport = MakeSharedPtr<EditorViewport>();
		Viewport->SetOwnerEditor(this);
		SceneHier = MakeSharedPtr<SceneHierarhy>();
		SceneHier->SetOwnerEditor(this);

		DetailsPanel = MakeSharedPtr<EditorDetails>();
		DetailsPanel->SetOwnerEditor(this);

		MenuBar = MakeSharedPtr<EditorMenuBar>();
		MenuBar->SetOwnerEditor(this);

		Toolbar = MakeSharedPtr<EditorToolbar>();
		Toolbar->SetOwnerEditor(this);

		EditorWidgets.push_back(Viewport);
		EditorWidgets.push_back(SceneHier);
		EditorWidgets.push_back(DetailsPanel);
		EditorWidgets.push_back(MenuBar);
		EditorWidgets.push_back(Toolbar);
		///
		m_ViewportCamera = MakeUniquePtr<EditorViewportClient>();
		m_ViewportCamera->EventActiveMove.AddBind(&EditorViewport::OnActiveMoveCamera, Viewport.get());
	}
	void EditorEngine::Update()
	{
		m_ViewportCamera->Update(GetWorld()->GetWorldDeltaTime(), Viewport->GetIsFocused());

		// Start render scene
		FrameBuffer->Bind(); // Enable write render in buffer 
		
		GetRender()->ClearBuffersScreen(); // Clear buffer
		Engine::Update();

		FrameBuffer->UnBind(); // Disable write render in buffer 
		// Finish render scene


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		ImGuizmo::BeginFrame(); 

		ImGui::ShowDemoWindow();

		RenderEditor();
		
		ImGui::Render();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		Application::Get()->GetWindow().GetContext()->UpdateContext();
		/*FrameBuffer->Bind();
		FrameBuffer->ClearTexture(1);
		FrameBuffer->UnBind();*/
		//GetWorld()->GetSaveManager()->PrintSerializeData();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//GetWorld()->GetSaveManager()->SaveSceneSerializedData();
	}

	void EditorEngine::SetSelectedObject(Runtime::Object* NewSelected)
	{
		SelectedObject = NewSelected;
	}

	Runtime::Object* EditorEngine::GetSelectedObject() const
	{
		return SelectedObject;
	}

	EditorViewportClient* EditorEngine::GetViewpoertClient() const
	{
		return m_ViewportCamera.get();
	}

	EStateWorld EditorEngine::GetCurrentStateWorld() const
	{
		return CurretnStateWorld;
	}

	void EditorEngine::SetCurrentStateWorld(EStateWorld NewState)
	{
		CurretnStateWorld = NewState;
	}

	void EditorEngine::RenderEditor()
	{
		Viewport->SetFrameBuffer(FrameBuffer);
		DetailsPanel->SetSelectableObject(SelectedObject);

		for (auto& Widget : EditorWidgets)
		{
			Widget->Draw();
		}

		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
		
		// Edit a color stored as 4 floats
		ImGui::ColorEdit4("Color", my_color);

		// Generate samples and plot them
		float samples[100];
		for (int n = 0; n < 100; n++)
			samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
		ImGui::PlotLines("Samples", samples, 100);

		// Display contents in a scrolling region
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
		ImGui::BeginChild("Scrolling");
		/*for (int n = 0; n < 50; n++)
			ImGui::Text("%04d: Some text", n);*/


		ImGui::EndChild();
	
		ImGui::End();
	}
	CoreEngine::World* EditorEngine::CreateWorld() const
	{
		return Runtime::CreateObject<EditorWorld>();
	}
}

