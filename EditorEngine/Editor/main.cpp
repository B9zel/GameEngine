#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "includes/EditorApplication.h"
#include <Core/includes/Base.h>
#include <Core/includes/Engine.h>
#include <Core/includes/ObjectPtr.h>
#include <Core/includes/Memory/GarbageCollector.h>
#include <Core/includes/TimerManager.h>
#include <Runtime/includes/PrimitiveComponent.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/World.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <iostream>
#include <Math/includes/Math.h>
#include <Math/includes/Transform.h>
#include <Math/includes/Vector.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLElementBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLTexture.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Runtime/includes/SceneComponent.h>
#include <Templates/Function.h>
#include <Core/includes/UpdateFunction.h>
#include <Runtime/includes/PlayerController.h>
#include <Runtime/includes/Pawn.h>
#include <Runtime/includes/InputComponent.h>
#include <Runtime/includes/CameraManager.h>
#include <Runtime/includes/CameraComponent.h>
#include <Core/includes/InputDevice.h>


//float arr[] = { 0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
//				0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
//				-0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
//				-0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top Left 
//};

float arr[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
};


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
		//texture2.Bind(1);

		shader.Bind();
		arrObj.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 8);


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


class MyController : public CoreEngine::Runtime::PlayerController
{
public:

	MyController()
	{
		//SetActorLocation(FVector(0, 0, -3));
		SetActorRotation(FVector(0, 0, 0));
		LastMousePos = FVector2(0.0f, 0.0f);
	}

protected:


	virtual void SetupInputComponent() override
	{
		PlayerController::SetupInputComponent();

		//inputComponent->BindAction(GLFW_KEY_W, CoreEngine::Runtime::EActionType::PRESSED, &MyController::Test, this)6;
		inputComponent->BindAction(GLFW_KEY_W, CoreEngine::EActionType::PRESSED, &MyController::Test2, this);

		inputComponent->BindAxis(GLFW_KEY_W, 1, &MyController::MoveForward, this);
		inputComponent->BindAxis(GLFW_KEY_S, -1, &MyController::MoveForward, this);
		inputComponent->BindAxis(GLFW_KEY_A, 1, &MyController::MoveLeft, this);
		inputComponent->BindAxis(GLFW_KEY_D, -1, &MyController::MoveLeft, this);
		inputComponent->BindAxis(GLFW_KEY_E, 1, &MyController::MoveUp, this);
		inputComponent->BindAxis(GLFW_KEY_Q, -1, &MyController::MoveUp, this);
		inputComponent->BindMouseMotionAxis(&MyController::MoveRight, this);
		//inputComponent->BindAction(GLFW_MOUSE_BUTTON_1, CoreEngine::Runtime::EActionType::PRESSED, &MyController::Test, this);
		//SetActorRotation(FVector(0, 1, 0));
		//SetActorLocation(FVector(0, 0, -3));
	}

	void MoveRight(double axisX, double axisY)
	{
		if (LastMousePos.x == 0.0f && LastMousePos.y == 0.0f)
		{
			LastMousePos = CoreEngine::Engine::Get()->GetInputDevice()->GetMousePos();
			return;
		}
		DVector2 NowPos = CoreEngine::Engine::Get()->GetInputDevice()->GetMousePos();
		double DeltaX = Math::Clamp(LastMousePos.x - NowPos.x, -360, 360);
		double DeltaY = Math::Clamp(LastMousePos.y - NowPos.y, -360, 360);

		if (DeltaX != 0.0f)
		{

			SetActorRotation(GetActorRotation() + FVector(0.0, (-DeltaX * 0.1), 0.0));
		}
		if (DeltaY != 0.0f)
		{

			SetActorRotation(GetActorRotation() + FVector((DeltaY * 0.1), 0.0f, 0.0f));
		}
		CoreEngine::Engine::Get()->GetInputDevice()->SetMousePos(DVector2(100, 100));
		LastMousePos = CoreEngine::Engine::Get()->GetInputDevice()->GetMousePos();
		//	SetActorRotation(FVector(0, 10, -10));
	}

	void MoveLeft(float axis)
	{
		if (axis == 1)
		{
			AddActorLocation(GetActorRightVector() * 0.01);
		}
		if (axis == -1)
		{
			AddActorLocation(-GetActorRightVector() * 0.01);

		}
	}
	void MoveUp(float axis)
	{
		AddActorLocation(FVector(0, 0.01 * axis, 0.0f));
	}

	void MoveForward(float axis)
	{
		if (axis == 1)
		{
			AddActorLocation(GetActorForwardVector() * 0.01);
			//	AddActorLocation(FVector(0, 0, -0.1));
		}
		if (axis == -1)
		{
			AddActorLocation(-GetActorForwardVector() * 0.01);

		}
		//	SetActorRotation(FVector(0, Math::Clamp(GetActorRotation().GetY() + 0.01, 0, 90), 0.0));
	}

	void AxisTest(float a)
	{
		using namespace CoreEngine::Runtime;

		//	this->AddActorLocation(FVector(0, 0, 0.01));

		FMatrix4x4 mat = cameraManager->GetViewMatrix();
		/*for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				std::cout << mat[i][j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;*/
		//	std::cout << GetActorRotation().GetX() << " " << GetActorRotation().GetY() << " " << GetActorRotation().GetZ() << std::endl;
			//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Controller {0}", a);
	}


	void Test()
	{
		EG_LOG(CoreEngine::CORE, ELevelLog::ERROR, "PResss controller");
	}

	void Test2()
	{
		EG_LOG(CoreEngine::CORE, ELevelLog::ERROR, "Release");

	}

private:

	DVector2 LastMousePos;
};

class Light : public CoreEngine::Runtime::Actor
{
public:

	Light()
	{
		String Path = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		auto& shadPair = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/LightShader.glsl").c_str());
		shade.CompileShader(shadPair.first, shadPair.second);

		vertObj.CreaterBuffer(arr, 288, CoreEngine::ETypeData::FLOAT, ETypeDraw::STATIC);
		arrObj.SetupIntorprit(0, 3, 8, CoreEngine::ETypeData::FLOAT, vertObj);

		SetActorLocation(FVector(3, 2, -7));
		SetActorScale(FVector(0.5));
		LightCube = CreateSubObject<CoreEngine::Runtime::PrimitiveComponent>();
		LightCube->GetSceneProxy()->CountVertex = 108;
		LightCube->GetSceneProxy()->AddShaderWithArrayObject(&shade, &arrObj);
		LightCube->GetSceneProxy()->SetTransform(GetActorTransform());

	}

private:

	CoreEngine::Runtime::PrimitiveComponent* LightCube;
	CoreEngine::Render::OpenGL::OpenGLShader shade;
	CoreEngine::Render::OpenGL::OpenGLVertexArrayObject arrObj;
	CoreEngine::Render::OpenGL::OpenGLVertexBufferObject vertObj;

};

class Quad : public CoreEngine::Runtime::Pawn
{
public:

	Quad()
	{
		String Path = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		auto& shadPair = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/ShaderBase.glsl").c_str());
		Shader.CompileShader(shadPair.first, shadPair.second);
		Shader.SetUniformVec3("PosLight", FVector(3, 2, -7));
		VertexBuffer.CreaterBuffer(arr, 288, CoreEngine::ETypeData::FLOAT, ETypeDraw::STATIC);
		VertexArray.SetupIntorprit(0, 3, 8, CoreEngine::ETypeData::FLOAT, VertexBuffer);
		VertexArray.SetupIntorprit(1, 3, 8, CoreEngine::ETypeData::FLOAT, VertexBuffer, 3);
		VertexArray.SetupIntorprit(2, 2, 8, CoreEngine::ETypeData::FLOAT, VertexBuffer, 6);
		texture.ChangeTexture("../../Shaders/container.jpg");
		texture2.ChangeTexture("../../Shaders/basi3p04.png");
		texture3.ChangeTexture("../../Shaders/bowt.png");
		texture3.ChangeTexture("C:/UnrealEngine/UE_5.3/Templates/TP_VehicleAdv/Media/TP_VehicleAdv_Preview.png");

		SetActorLocation(FVector(0, 0, -5));
		//SetActorScale(FVector(4, 4, 4));
		//SetActorLocation(FVector(2.0f, 5.0f, -15.0f));

		angle = 1;
		quad = CreateSubObject<CoreEngine::Runtime::PrimitiveComponent>();
		quad->GetSceneProxy()->CountVertex = 108;
		quad->GetSceneProxy()->AddShaderWithArrayObject(&Shader, &VertexArray);
		//quad->GetSceneProxy()->AddTexture(&texture);
		//quad->GetSceneProxy()->AddTexture(&texture2);
		//quad->GetSceneProxy()->AddTexture(&texture3);
		//quad->GetSceneProxy()->AddTexture(&texture4);
		quad->GetSceneProxy()->SetTransform(GetActorTransform());
	}

	virtual void Update(float deltaTime) override
	{
		Pawn::Update(deltaTime);
		Shader.SetUniformVec3("PosLight", FVector(3, 2, -7));
		Shader.SetUniformVec3("ViewPos", GetOwner()->GetActorLocation());
		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0} {1} {2}", GetOwner()->GetActorLocation().GetX(), GetOwner()->GetActorLocation().GetY(), GetOwner()->GetActorLocation().GetZ());
		scale += deltaTime;
		angle = abs(cos(scale)) + 0.2;
		SetActorRotation(GetActorRotation() + FVector(0.0, 1, 0.0));
		//SetActorRotation(GetActorRotation() + FVector(0.0f, 0.0f, -0.005));
		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, scale);
		//SetActorScale(FVector(angle, angle, angle));
		//SetActorLocation(FVector(scale/ 5,0, -5));
		//SetActorRotation(FVector(0, scale, 0));
	//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0} {1} {2}", GetActorLocation().GetX(), GetActorLocation().GetY(), GetActorLocation().GetZ());
		quad->GetSceneProxy()->SetTransform(GetActorTransform());
	}
protected:

	virtual void SetupInputPlayerController(CoreEngine::Runtime::InputComponent* Input) override
	{
		Pawn::SetupInputPlayerController(Input);

		Input->BindAction(GLFW_MOUSE_BUTTON_1, CoreEngine::EActionType::PRESSED, &Quad::Test, this);
		inputComponent->BindAxis(GLFW_KEY_RIGHT, 1, &Quad::MoveRight, this);
		inputComponent->BindAxis(GLFW_KEY_LEFT, -1, &Quad::MoveRight, this);
		inputComponent->BindAxis(GLFW_KEY_UP, 1, &Quad::MoveForward, this);
		inputComponent->BindAxis(GLFW_KEY_DOWN, -1, &Quad::MoveForward, this);

		EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Input setup");
	}
	void MoveForward(float a)
	{

		AddActorLocation(FVector(0, 0, a * 0.001));


		//if (a == 10.0f)
		{

			//EG_LOG(CoreEngine::CORE, ELevelLog::ERROR, "Pawn {0}", a);
		}
		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Pawn {0}", a);
	}
	void MoveRight(float a)
	{
		AddActorLocation(FVector(a * 0.001, 0, 0));
		//if (a == 10.0f)
		{

			//EG_LOG(CoreEngine::CORE, ELevelLog::ERROR, "Pawn {0}", a);
		}
		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Pawn {0}", a);
	}

	void Test()
	{
		EG_LOG(CoreEngine::CORE, ELevelLog::ERROR, "PResss");
	}

private:

	CoreEngine::Runtime::PrimitiveComponent* quad = nullptr;

	CoreEngine::Render::OpenGL::OpenGLVertexBufferObject VertexBuffer;
	CoreEngine::Render::OpenGL::OpenGLVertexArrayObject VertexArray;
	CoreEngine::Render::OpenGL::OpenGLShader Shader;

	float Arr2[24] = { 0.5,  -0.5,		1.0, 0.0,
						-0.5, -0.5,		0.0, 0.0,
						0.5,  0.5,		1, 1,
						-0.5, -0.5,		0.0, 0.0,
						0.5,  0.5,		1, 1,
						-0.5, 0.5,		1,0

	};

	CoreEngine::Render::OpenGL::OpenGLTexture2D texture;
	CoreEngine::Render::OpenGL::OpenGLTexture2D texture2;
	CoreEngine::Render::OpenGL::OpenGLTexture2D texture3;
	CoreEngine::Render::OpenGL::OpenGLTexture2D texture4;

	float angle = 0;
	float scale = 0;
};



class FirstLevel : public CoreEngine::Level
{
	virtual void ActorInitialize() override
	{
		auto* controller = GetWorld()->SpawnActor<MyController>(nullptr);
		auto* pawn = GetWorld()->SpawnActor<Quad>(controller);
		auto* ac = GetWorld()->SpawnActor<Light>(controller);
		controller->Possess(pawn);

		Level::ActorInitialize();
	}
};





int main(int argc, char** argv)
{

	CoreEngine::ApplicationOptions options("Test", argv[0]);
	auto app = MakeUniquePtr<Editor::EditorApplication>(options);
	auto* level = new FirstLevel();
	app->Get()->m_Engine->GetWorld()->OpenLevel(level);
	app->PushLayer(new RenderLayer);



	app->Start();

}

