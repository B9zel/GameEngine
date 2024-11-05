#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "includes/EditorApplication.h"
#include <Core/includes/Memory/GarbageCollector.h>
#include <Core/includes/Base.h>
#include <Core/includes/Engine.h>
#include <Core/includes/TimerManager.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLTexture.h>
#include <Core/includes/GBNotify.h>
#include <Templates/Function.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <Platform/Renderer/OpenGL/include/OpenGLElementBufferObject.h>
#include <Math/includes/Math.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/TimerManager.h>
#include <Core/includes/World.h>
#include <Math/includes/Transform.h>
#include <Math/includes/Vector.h>
#include <Runtime/includes/SceneComponent.h>


float arr[] = { 0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
				0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
				-0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
				-0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top Left 
};

unsigned int indexArr[] = { 0, 1, 2,
							0, 2, 3 };

class RenderLayer : public CoreEngine::Layer
{
public:
	RenderLayer() : texture("C:/c++/GameEngine/Shaders/container.jpg"), texture2("E:/SteamLibrary/steamapps/common/War Thunder/ui/splashWT.png")
	{
		
	}
	
	virtual void OnAttach() 
	{
		mat = FMatrix4x4(1);
		matOffset = FMatrix4x4(1);
		matRotate = FMatrix4x4(1);
		


		String a = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		Pair<String, String>& pa = CoreEngine::Render::Shader::LoadShader((a + "/Shaders/ShaderBase.glsl").c_str());
		//texture.ChangeTexture("");
		shader.CompileShader(pa.first, pa.second);
		bufferObj.CreaterBuffer(arr, sizeof(arr) / sizeof(*arr), CoreEngine::ETypeData::FLOAT, ETypeDraw::STATIC);
		arrElement.CreateBuffer(indexArr, sizeof(indexArr) / sizeof(*arr), CoreEngine::ETypeData::UNSIGNED_INT, ETypeDraw::STATIC, bufferObj);
		arrObj.SetupIntorprit(0, 2, 7, CoreEngine::ETypeData::FLOAT, bufferObj, arrElement);
		arrObj.SetupIntorprit(1, 3, 7, CoreEngine::ETypeData::FLOAT, bufferObj, arrElement, 2);
		arrObj.SetupIntorprit(2, 2, 7, CoreEngine::ETypeData::FLOAT, bufferObj, arrElement, 5);

		
	}
	virtual void OnDetach()  {}
	virtual void NativeUpdate(float deltaTime)
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		
		ImGui::Begin("wINDOW", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Hello, world %d", 123);
		ImGui::SliderFloat2("slider", xy, -3, 3, "%.3f", ImGuiSliderFlags_::ImGuiSliderFlags_None);
	
		mat = glm::scale_slow(FMatrix4x4(1), FVector(xy[0], xy[1], 0).vector);
		matOffset = glm::translate(FMatrix4x4(1), FVector(offXY[0], offXY[1], 0).vector);

		if (Rotate[0] != bufRotate[0])
		{
			matRotate = glm::rotate(matRotate, Rotate[0], FVector(1, 0, 0).vector);
			bufRotate[0] = Rotate[0];
		}
		else if (Rotate[1] != bufRotate[1])
		{
			matRotate = glm::rotate(matRotate, Rotate[1], FVector(0, 1, 0).vector);
			bufRotate[1] = Rotate[1];
		}
		else if(Rotate[2] != bufRotate[2])
		{
			matRotate = glm::rotate(matRotate, Rotate[2], FVector(0, 0, 1).vector);
			bufRotate[2] = Rotate[2];
		}

		ImGui::SliderFloat("Mix", &a, 0, 1);
		ImGui::SliderFloat2("slider Offset", offXY, -1, 1);
		ImGui::SliderFloat3("Rotate", Rotate, -2, 2);
		ImGui::ColorEdit4("Color", color);

		ImGui::End();


		//shader.SetUniformMatrix4x4("scale", mat);
		//shader.SetUniformMatrix4x4("offset", matOffset);
		//shader.SetUniformMatrix4x4("rotate", matRotate);
		shader.SetUniformVec4("inputColor", FVector4(color[0], color[1], color[2], color[3]));
		shader.SetUniform1i("ourTexture", 0);
		shader.SetUniform1i("ourTexture1", 1);
		shader.SetUniformFloat("a", a);


		//shader.SetUniformVec2("iResolution", FVector2(800, 450));//FVector2(CoreEngine::Application::Get()->GetWindow().GetWidth(), CoreEngine::Application::Get()->GetWindow().GetHeight()));
	
		//shader.SetUniform1i("outTexture", 0);

		texture.Bind();
		texture2.Bind(1);
		
		shader.Bind();
		arrObj.Bind();
	
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		
		shader.UnBind();
		arrObj.UnBind();
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

private:

	float a;
	//GLuint texture;
	CoreEngine::Render::OpenGL::OpenGLTexture2D texture;
	CoreEngine::Render::OpenGL::OpenGLTexture2D texture2;
	CoreEngine::Render::OpenGL::OpenGLShader shader;
	CoreEngine::Render::OpenGL::OpenGLVertexBufferObject bufferObj;
	CoreEngine::Render::OpenGL::OpenGLVertexArrayObject arrObj;
	CoreEngine::Render::OpenGL::OpenGLElementBufferObject arrElement;
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

class Test : public CoreEngine::Runtime::Object
{
public:
	Test()
	{
		obj = CoreEngine::Runtime::CreateObject<Object>();
		obj2 = CoreEngine::Runtime::CreateObject<Object>();
		CoreEngine::TimerHandle hand;
		EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Obj live")
		CoreEngine::Engine::Get()->GetTimerManager()->SetTimer(hand, this, &Test::Del, 9, false);
		//CoreEngine::Application::Get()->GetTimerManager()
	}

	void Del()
	{
		obj = obj2;
		CoreEngine::TimerHandle hand;
		CoreEngine::Engine::Get()->GetTimerManager()->SetTimer(hand, this, &Test::Print, 2, false);
	}

	void Print()
	{
		if (obj)
		{
			EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Obj live")
		}
		if (obj2)
			{
				EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Obj2 live")
			}
	}

	PROPERTY(Object*, obj);
	PROPERTY(Object*, obj2);
};


class A
{

};

class B : public A
{
public:

	void Print()
	{
		std::cout << typeid(*this).raw_name() << std::endl;
	}

};

class C : public B
{

};

int main(int argc, char** argv)
{	

	CoreEngine::ApplicationOptions options("Test", argv[0]);
	auto app = MakeUniquePtr<Editor::EditorApplication>(options);
	//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, CoreEngine::Runtime::IsParentClass(a,c) == true? "true": "false");
	app->PushLayer(new RenderLayer);
	//Test a;
	//TO q;
	//CoreEngine::GB::GBNotify<int*> a(p, Function<void(CoreEngine::Runtime::Object*, CoreEngine::Runtime::Object*)>(&Test::Notfy,&t));
	//a = &c;
	app->Start();
	
}

