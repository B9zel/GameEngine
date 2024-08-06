#pragma once
#include <Core/includes/Layer.h>
#include <Core/includes/Log.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace Editor
{
	DECLARE_LOG_CATEGORY_EXTERN(EDITOR)

	class EditorLayer : public CoreEngine::Layer
	{
		std::string buf;
		float f;
		void MySaveFunction()
		{
			bool is = ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left);
			EG_LOG(EDITOR, ELevelLog::INFO, "Click {0}", is);
		}


		virtual void OnAttach() override {}
		virtual void OnDetach() override {}
		virtual void NativeUpdate(float deltaTime) override
		{
			//ImGui_ImplOpenGL3_NewFrame();
			//ImGui_ImplGlfw_NewFrame();
			//ImGui::NewFrame();
			//ImGui::ShowDemoWindow();

			//ImGui::Text("Hello, world %d", 123);
			//if (ImGui::Button("Save") && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
			//	MySaveFunction();
			/*
			ImGui::InputText("Input", buf.data(), IM_ARRAYSIZE(buf.data()));
			ImGui::InputText("Input", buf.data(), IM_ARRAYSIZE(buf.data()));
			ImGui::SliderFloat("Slider", &f, 0.0f, 1.0f);*/

			//ImGui::Render();
			//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		};
	};
}