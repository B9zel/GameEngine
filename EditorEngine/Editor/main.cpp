#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "includes/EditorApplication.h"
#include <Editor/includes/Test/TestEditorLevel.h>

//#include <Core/includes/Base.h>
#include <Core/includes/Engine.h>

#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/World.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

#include <Math/includes/Vector.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLElementBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLTexture.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>








unsigned int indexArr[] = { 0, 1, 2,
							0, 2, 3 };

class RenderLayer : public CoreEngine::Layer
{
public:
	RenderLayer() : texture(""), texture2("E:/SteamLibrary/steamapps/common/War Thunder/ui/splashWT.png")
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
		//shader.CompileShader(pa.first, pa.second);
		//bufferObj.CreaterBuffer(Arr2, sizeof(Arr2) / sizeof(*Arr2), CoreEngine::ETypeData::FLOAT, ETypeDraw::STATIC);
		//arrElement.CreateBuffer(indexArr, sizeof(indexArr) / sizeof(*arr), CoreEngine::ETypeData::UNSIGNED_INT, ETypeDraw::STATIC, bufferObj);
		arrObj.SetupIntorprit(0, 2, 0, CoreEngine::ETypeData::FLOAT, bufferObj);
		//arrObj.SetupIntorprit(1, 3, 7, CoreEngine::ETypeData::FLOAT, bufferObj, 2);
		//arrObj.SetupIntorprit(2, 2, 7, CoreEngine::ETypeData::FLOAT, bufferObj, 5);


	}
	virtual void OnDetach() {}
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
		else if (Rotate[2] != bufRotate[2])
		{
			matRotate = glm::rotate(matRotate, Rotate[2], FVector(0, 0, 1).vector);
			bufRotate[2] = Rotate[2];
		}

		ImGui::SliderFloat("Mix", &a, 0, 1);
		ImGui::SliderFloat2("slider Offset", offXY, -1, 1);
		ImGui::SliderFloat3("Rotate", Rotate, -2, 2);
		ImGui::ColorEdit4("Color", color);

		ImGui::End();

		shader.SetUniformVec4("inputColor", FVector4(color[0], color[1], color[2], color[3]));
		shader.SetUniform1i("ourTexture", 0);
		shader.SetUniform1i("ourTexture1", 1);
		shader.SetUniformFloat("a", a);

		texture.Bind();
		//texture2.Bind(1);

		shader.Bind();
		arrObj.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 8);


		shader.UnBind();
		arrObj.UnBind();

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
	float Rotate[3] = { 0, 0, 0 };
	float bufRotate[3] = { 0,0,0 };

	float color[4] = { 1,1,1,1 };
	FMatrix4x4 mat = { 1,0,0,0,0,1,0,0,
	0,0,1,0,
	0,0,0,1
	};
	FMatrix4x4 matOffset;

	FMatrix4x4 matRotate;
};

class Test : CoreEngine::Runtime::Object
{
public:

	Test(const CoreEngine::InitializeObject& Object) : Object(Object)
	{
		
	}

public:

	void Update()
	{
		
	}
};





//void dump(const YAML::Node& n, const std::string& name) {
//	std::cout << "---- " << name << " ----\n";
//	std::cout << "defined=" << (bool)n
//		<< " IsMap=" << n.IsMap()
//		<< " IsNull=" << n.IsNull()
//		<< " size=" << n.size() << "\n";
//	std::cout << name << " YAML:\n" << n << "\n\n";
//}


int main(int argc, char** argv)
{
	//YAML::Node a = YAML::Node(YAML::NodeType::Map);
	//dump(a, "a (initial)");

	//YAML::Node b = a;
	//dump(b, "b after b = a");

	//b.force_insert("aa", YAML::Node(YAML::NodeType::Map));
	//dump(a, "a after b[\"aa\"] = Map");
	//dump(b, "b after b[\"aa\"] = Map");

	//YAML::Node c = b["aa"];
	//b = c;
	//dump(a, "a after b = b[\"aa\"]");
	//dump(b, "b after b = b[\"aa\"]");

	//b["b"] = YAML::Node(YAML::NodeType::Map);
	//dump(a, "a after b[\"b\"] = Map");
	//dump(b, "b after b[\"b\"] = Map");

	////b.reset();
	//b = b["b"];
	//dump(a, "a after b = b[\"b\"]");
	//dump(b, "b after b[\"b\"]");

	//c["c"] = "5";
	//dump(a, "a after b[\"c\"] = \"5\"");
	//dump(b, "b after b[\"c\"] = \"5\"");


	CoreEngine::ApplicationOptions options("Test", argv[0], nullptr);
	auto app = MakeUniquePtr<Editor::EditorApplication>(options);
	app->CreateApp();
	CoreEngine::InitializeObject Init;
	//Init.Class = FirstLevel::GetStaticClass();
	
	auto* level = CoreEngine::Runtime::CreateObject< FirstLevel>(app->GetEngine()->GetWorld());// new FirstLevel(Init);
	app->Get()->GetEngine()->GetWorld()->OpenLevel(level);
	//app->PushLayer(new RenderLayer);


	app->Start();
}

