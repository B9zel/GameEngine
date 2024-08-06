#pragma once

#include <iostream>
#include <Core/includes/Log.h>
#include <Core/includes/TimerManager.h>
#include "includes/EditorApplication.h"
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <Core/includes/GBNotify.h>
#include <Templates/Function.h>
#include <Core/includes/Base.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <iostream>

#include <Math/includes/Math.h>




float arr[] = { -0.5f, 0.5,
				0.5, 0.5,
				-0.5,-0.5,
				0.5, -0.5,
				0.5,0.5,
				-0.5,-0.5 };




const char* vv = "\n"
"#version 330 core\n"
"layout (location = 0) in vec2 position; \n"
"uniform mat4 scale;\n"
"uniform mat4 offset;"
"uniform mat4 rotate;"
"void main() \n"
"{ \n"
"	gl_Position = offset * rotate * scale * vec4(position.x,position.y, 1,1)  ; \n"
"}";

const char* ff = 
R"(//#type vertex
)";

class RenderLayer : public CoreEngine::Layer
{
	
	virtual void OnAttach() 
	{
		mat = FMatrix4x4(1);
		matOffset = FMatrix4x4(1);
		matRotate = FMatrix4x4(1);



		String a = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		Pair<String, String>& pa = CoreEngine::Render::Shader::LoadShader((a + "/Shaders/ShaderBase.glsl").c_str());
		shader.CompileShader(pa.first, pa.second);
		bufferObj.CreaterBuffer(arr, sizeof(arr) / sizeof(*arr), CoreEngine::ETypeData::FLOAT, ETypeDraw::STATIC);
		arrObj.SetupIntorprit(0, 2, 0, CoreEngine::ETypeData::FLOAT, bufferObj);
	}
	virtual void OnDetach()  {}
	virtual void NativeUpdate(float deltaTime)
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow();
		ImGui::Begin("wINDOW", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Hello, world %d", 123);
		ImGui::SliderFloat2("slider", xy, -1, 1, "%.3f", ImGuiSliderFlags_::ImGuiSliderFlags_None);
	
		mat = glm::scale_slow(FMatrix4x4(1), FVector(xy[0], xy[1], 0));
		matOffset = glm::translate(FMatrix4x4(1), FVector(offXY[0], offXY[1], 0));

		if (Rotate[0] != bufRotate[0])
		{
			matRotate = glm::rotate(matRotate, Rotate[0], FVector(1, 0, 0));
			bufRotate[0] = Rotate[0];
		}
		else if (Rotate[1] != bufRotate[1])
		{
			matRotate = glm::rotate(matRotate, Rotate[1], FVector(0, 1, 0));
			bufRotate[1] = Rotate[1];
		}
		else if(Rotate[2] != bufRotate[2])
		{
			matRotate = glm::rotate(matRotate, Rotate[2], FVector(0, 0, 1));
			bufRotate[2] = Rotate[2];
		}

		ImGui::SliderFloat2("slider Offset", offXY, -1, 1);
		ImGui::SliderFloat3("Rotate", Rotate, -2, 2);
		ImGui::ColorEdit4("Color", color);

		ImGui::End();

		shader.SetUniformMatrix4x4("scale", mat);
		shader.SetUniformMatrix4x4("offset", matOffset);
		shader.SetUniformMatrix4x4("rotate", matRotate);
		

		shader.SetUniformVec2("iResolution", FVector2(800, 450));//FVector2(CoreEngine::Application::Get()->GetWindow().GetWidth(), CoreEngine::Application::Get()->GetWindow().GetHeight()));
		shader.SetUniformFloat("iTime", glfwGetTime());
        shader.SetUniformVec2("iMouse", CoreEngine::Application::Get()->GetInputDevice()->GetMousePos());
	
		shader.Bind();
		bufferObj.Bind();		
		

		glDrawArrays(GL_TRIANGLES, 0, 6);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

private:

	
	CoreEngine::Render::OpenGLShader shader;
	CoreEngine::Render::OpenGLVertexBufferObject bufferObj;
	CoreEngine::Render::OpenGLVertexArrayObject arrObj;
	float xy[2] = { 1, 1 };
	float offXY[2] = { 0,0 };
	char* buf;
	float Rotate[3] = {0, 0, 0};
	float bufRotate[3] = { 0,0,0 };
	
	float color[4] = { 1,1,1,1 };
	FMatrix4x4 mat = {1,0,0,0,0,1,0,0,
	0,0,1,0,
	0,0,0,1
	};
	FMatrix4x4 matOffset;

	FMatrix4x4 matRotate;
};




int main(int argc, char** argv)
{	
	
//	std::cout << argv[0] << std::endl;
	CoreEngine::ApplicationOptions options("Test", argv[0]);
	auto app = MakeUniquePtr<Editor::EditorApplication>(options);
	app->PushLayer(new RenderLayer);
	
	//TO q;
	//CoreEngine::GB::GBNotify<int*> a(p, Function<void(CoreEngine::Runtime::Object*, CoreEngine::Runtime::Object*)>(&Test::Notfy,&t));
	//a = &c;
	app->Start();

}

