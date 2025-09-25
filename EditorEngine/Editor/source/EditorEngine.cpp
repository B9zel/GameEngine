
#include <Editor/includes/EditorEngine.h>
#include <imgui/imgui.h>
#include <Core/includes/World.h>
#include <Render/includes/Render.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <Core/includes/Application.h>
#include <Core/includes/Window.h>

namespace Editor
{
	EditorEngine::EditorEngine()
	{
		CoreEngine::Render::FramebufferSpecification Spec;
		Spec.Height = Application::Get()->GetWindow().GetHeight();
		Spec.Width = Application::Get()->GetWindow().GetWidth();
		FrameBuffer = CoreEngine::Render::Framebuffer::Create(Spec);
		
	}
	void EditorEngine::Update()
	{
		FrameBuffer->Bind();
		GetRender()->ClearBuffersScreen();
		Engine::Update();
		FrameBuffer->UnBind();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
		//	ImGui::ShowDemoWindow();
		RenderEditor();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorEngine::RenderEditor()
	{
		//ImGui::Text("Hello, world %d", 123);
		//if (ImGui::Button("Save"))
		//{

		//}
		////MySaveFunction();
		//ImGui::InputText("string", buf.data(), IM_ARRAYSIZE(buf.data()));
		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

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
		static ImVec2 lastSize = ImVec2(0, 0);
		ImGui::Begin("Viewport");
		uint32 Texture = FrameBuffer->GetColorAttachmentID();
		ImGui::Image((ImTextureID)(intptr_t)Texture, ImVec2(FrameBuffer->GetSpecifiction().Width, FrameBuffer->GetSpecifiction().Height - 36), ImVec2(0, 1), ImVec2(1, 0));
		ImVec2 Space = ImGui::GetWindowSize();;
		if (Space.x != lastSize.x || Space.y != lastSize.y)
		{
			FrameBuffer->Resize(abs(Space.x), abs(Space.y));
			lastSize = Space;
			EG_LOG(CORE, ELevelLog::INFO, "{0} {1}", abs(Space.x), (Space.y));
		}
		ImGui::End();
	}	
	


}

