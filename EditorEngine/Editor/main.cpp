#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "includes/EditorApplication.h"
//#include <Core/includes/Base.h>
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
#include <Runtime/includes/MeshComponent.h>
#include <Runtime/includes/PointLightComponent.h>
#include <Runtime/includes/SpotLightComponent.h>
#include <Runtime/includes/DirectionLightComponent.h>
#include <Editor/includes/EditorEngine.h>
#include <Events/include/Event.h>


float arr[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
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

float arri[] = {
	-0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 0
	 0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 1
	 0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 2
	-0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 3

	// Задняя грань
	-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // 4
	 0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 5
	 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // 6
	-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 7

	// Левая грань
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0,0.0f, 0.0, // 8
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0,1.0f, 0.0, // 9
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0,1.0f, 1.0, // 10
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0 ,0.0f, 1.0, // 11

	// Правая грань
	0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 12
	0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 13
	0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 14
	0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 15

	// Верхняя грань
	-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // 16
	0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 17
	0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 18
	-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // 19

	// Нижняя грань
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // 20
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 21
	0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // 22
	-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f
};

int Index[] = {
	0, 1, 2, 2, 3, 0,   // Передняя грань
	4, 5, 6, 6, 7, 4,   // Задняя грань
	8, 9, 10, 10, 11, 8, // Левая грань
	12, 13, 14, 14, 15, 12, // Правая грань
	16, 17, 18, 18, 19, 16,
	20, 21, 22, 22, 23, 20
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


class MyController : public CoreEngine::Runtime::PlayerController
{
public:

	MyController()
	{
		SetActorRotation(FVector(0, 0, 0));
		LastMousePos = FVector2(0.0f, 0.0f);
	}

protected:


	virtual void SetupInputComponent() override
	{
		PlayerController::SetupInputComponent();

		inputComponent->BindAxis(GLFW_KEY_W, 1, &MyController::MoveForward, this);
		inputComponent->BindAxis(GLFW_KEY_S, -1, &MyController::MoveForward, this);
		inputComponent->BindAxis(GLFW_KEY_A, 1, &MyController::MoveLeft, this);
		inputComponent->BindAxis(GLFW_KEY_D, -1, &MyController::MoveLeft, this);
		inputComponent->BindAxis(GLFW_KEY_E, 1, &MyController::MoveUp, this);
		inputComponent->BindAxis(GLFW_KEY_Q, -1, &MyController::MoveUp, this);
		inputComponent->BindAction(GLFW_MOUSE_BUTTON_RIGHT, CoreEngine::EActionType::PRESSED, &MyController::RightPress, this);
		inputComponent->BindAction(GLFW_MOUSE_BUTTON_RIGHT, CoreEngine::EActionType::RELEASED, &MyController::RightRelease, this);
		inputComponent->BindMouseMotionAxis(&MyController::MoveRight, this);
	}

	void RightPress()
	{
		CurrentMouseLoc = CoreEngine::Engine::Get()->GetInputDevice()->GetMousePos();
		IsLooking = true;
	}

	void RightRelease()
	{

		IsLooking = false;
	}

	void MoveRight(double axisX, double axisY)
	{
		if (!IsLooking) return;
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
		LastMousePos = CoreEngine::Engine::Get()->GetInputDevice()->GetMousePos();
		CoreEngine::Engine::Get()->GetInputDevice()->SetMousePos(LastMousePos);

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
		}
		if (axis == -1)
		{
			AddActorLocation(-GetActorForwardVector() * 0.01);
		}
	}

	virtual void Update(float DeltaTime) override
	{
		CoreEngine::Runtime::PlayerController::Update(DeltaTime);

		if (IsLooking)
		{

		}

	}


private:

	bool IsLooking{ false };
	DVector2 LastMousePos;
	DVector2 CurrentMouseLoc;
};

class Light : public CoreEngine::Runtime::Actor
{
public:

	Light()
	{
		String Path = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		auto& shadPair = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/LightShader.glsl").c_str());
		shade.CompileShader(shadPair.first, shadPair.second);

		arrObj.CreateVertexArray();
		vertObj.CreaterBuffer(arr, 288, CoreEngine::ETypeData::FLOAT, ETypeDraw::STATIC, arrObj);
		arrObj.SetupIntorprit(0, 3, 8, CoreEngine::ETypeData::FLOAT, vertObj);

		SetActorLocation(FVector(3, 2, -7));
		SetActorScale(FVector(0.2));
		LightCube = CreateSubObject<CoreEngine::Runtime::PrimitiveComponent>();
		LightCube->GetSceneProxy()->CountVertex = 108;
		LightCube->GetSceneProxy()->AddShaderWithArrayObject(&shade, &arrObj, nullptr);
		LightCube->GetSceneProxy()->SetTransform(GetActorTransform());

		/*LightObj = CreateSubObject<CoreEngine::Runtime::DirectionLightComponent>();
		LightObj->SetIntencity(10);
		LightObj->SetComponentRotation(FVector(0, 0, 0));
		LightObj->SetComponentLocation(FVector(3, 2, -7));*/

	}

	virtual void Update(float deltaTime) override
	{
		CoreEngine::Runtime::Actor::Update(deltaTime);
		//AddActorRotation(FVector(0.1, 0, 0));
		EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Actor {0} {1} {2}", GetActorRotation().GetX(), GetActorRotation().GetY(), GetActorRotation().GetZ());
		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Direction {0} {1} {2}", LightObj->GetComponentRotation().GetX(), LightObj->GetComponentRotation().GetY(), LightObj->GetComponentRotation().GetZ());
		//AddActorRotation(FVector(100 * deltaTime, 0, 0));
	}

private:

	CoreEngine::ObjectPtr<CoreEngine::Runtime::DirectionLightComponent> LightObj;
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
		mesh = CreateSubObject<CoreEngine::Runtime::MeshComponent>();
		mesh->LoadMesh("C:/Projects/3D_Models/Table.obj");
		mesh->SetComponentScale(FVector(0.8));
		mesh->AddComponentLocation(FVector(0, -1, 0));
		//mesh->SetComponentScale(FVector(2));
		//mesh->AddComponentLocation(FVector(0, 0, -5));
		//mesh->SetComponentScale(FVector(10));
		String Path = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		auto& shadPair = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/StaticMeshBaseShader.glsl").c_str());
		Shader.CompileShader(shadPair.first, shadPair.second);
		//Shader.SetUniformVec3("PosLight", FVector(3, 2, -7));
		VertexArray.CreateVertexArray();
		VertexBuffer.CreaterBuffer(arri, 192, CoreEngine::ETypeData::FLOAT, ETypeDraw::STATIC, VertexArray);
		ElementBuffer.CreateBuffer(Index, 36, CoreEngine::ETypeData::UNSIGNED_INT, ETypeDraw::STATIC, VertexArray);
		VertexArray.SetupIntorprit(0, 3, 8, CoreEngine::ETypeData::FLOAT, VertexBuffer);
		VertexArray.SetupIntorprit(1, 3, 8, CoreEngine::ETypeData::FLOAT, VertexBuffer, 3);
		VertexArray.SetupIntorprit(2, 2, 8, CoreEngine::ETypeData::FLOAT, VertexBuffer, 6);
		/*	texture.ChangeTexture("../../Shaders/container.jpg");
			texture2.ChangeTexture("../../Shaders/basi3p04.png");
			texture3.ChangeTexture("../../Shaders/bowt.png");
			texture3.ChangeTexture("C:/UnrealEngine/UE_5.3/Templates/TP_VehicleAdv/Media/TP_VehicleAdv_Preview.png");*/

		SetActorLocation(FVector(0, 0, -5));
		SetActorScale(FVector(1));

		/*quad = CreateSubObject<CoreEngine::Runtime::PrimitiveComponent>();
		quad->GetSceneProxy()->CountVertex = 0;
		quad->GetSceneProxy()->CountIndeces = 36;
		quad->GetSceneProxy()->AddShaderWithArrayObject(&Shader, &VertexArray, &ElementBuffer);
		quad->GetSceneProxy()->SetTransform(GetActorTransform());*/
	}


	virtual void Update(float deltaTime) override
	{
		Pawn::Update(deltaTime);
		//Shader.SetUniformVec3("PosLight", FVector(3, 2, -7));
		// Shader.SetUniformVec3("ViewPos", GetOwner()->GetActorLocation());
	}

protected:

	virtual void SetupInputPlayerController(CoreEngine::Runtime::InputComponent* Input) override
	{
		Pawn::SetupInputPlayerController(Input);

		inputComponent->BindAxis(GLFW_KEY_RIGHT, 1, &Quad::MoveRight, this);
		inputComponent->BindAxis(GLFW_KEY_LEFT, -1, &Quad::MoveRight, this);
		inputComponent->BindAxis(GLFW_KEY_UP, 1, &Quad::MoveForward, this);
		inputComponent->BindAxis(GLFW_KEY_DOWN, -1, &Quad::MoveForward, this);
		inputComponent->BindAxis(GLFW_KEY_N, 1, &Quad::MoveRotate, this);
		inputComponent->BindAxis(GLFW_KEY_M, -1, &Quad::MoveRotate, this);
		inputComponent->BindAxis(GLFW_KEY_L, -1, &Quad::MoveUp, this);
		inputComponent->BindAxis(GLFW_KEY_P, 1, &Quad::MoveUp, this);
	}
	void MoveForward(float a)
	{
		//AddActorLocation(FVector(0, 0, a * 0.01));

		GetWorld()->GetLevels()[0]->GetActors()[3]->AddActorLocation(FVector(0.1 * a, 0, 0));
	}
	void MoveRotate(float a)
	{
		GetWorld()->GetLevels()[0]->GetActors()[3]->AddActorRotation(FVector(0, 25 * a, 0));
	}
	void MoveRight(float a)
	{
		//AddActorLocation(FVector(a * 0.01, 0, 0));
		GetWorld()->GetLevels()[0]->GetActors()[3]->AddActorLocation(FVector(0, 0, 0.1 * a));
	}
	void MoveUp(float a)
	{
		AddActorLocation(FVector(0, a * 0.01, 0));
	}

private:

	CoreEngine::Runtime::PrimitiveComponent* quad = nullptr;
	CoreEngine::Runtime::MeshComponent* mesh = nullptr;

	CoreEngine::Render::OpenGL::OpenGLVertexBufferObject VertexBuffer;
	CoreEngine::Render::OpenGL::OpenGLVertexArrayObject VertexArray;
	CoreEngine::Render::OpenGL::OpenGLElementBufferObject ElementBuffer;
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
};

class LightActor : public CoreEngine::Runtime::Actor
{
public:

	LightActor()
	{
		LightObj = CreateSubObject<CoreEngine::Runtime::SpotLightComponent>();
		LightObj->SetColor(FVector(0, 1, 0));
		LightObj->SetConstant(1.0f);
		LightObj->SetLinear(0.2f);
		LightObj->SetQuadratic(0.3f);
		LightObj->SetIntencity(10);
		LightObj->SetCutOff(50);
		LightObj->SetOuterCutOff(60);


		String Path = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		auto& shadPair = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/LightShader.glsl").c_str());
		shade.CompileShader(shadPair.first, shadPair.second);

		arrObj.CreateVertexArray();
		vertObj.CreaterBuffer(arr, 288, CoreEngine::ETypeData::FLOAT, ETypeDraw::STATIC, arrObj);
		arrObj.SetupIntorprit(0, 3, 8, CoreEngine::ETypeData::FLOAT, vertObj);
		LightCube = CreateSubObject<CoreEngine::Runtime::PrimitiveComponent>();
		LightCube->GetSceneProxy()->CountVertex = 108;
		LightCube->GetSceneProxy()->AddShaderWithArrayObject(&shade, &arrObj, nullptr);
		LightCube->GetSceneProxy()->SetTransform(GetActorTransform());

	}

	virtual void Update(float delta) override
	{
		Actor::Update(delta);

		//LightCube->GetSceneProxy()->
	}

private:

	CoreEngine::ObjectPtr<CoreEngine::Runtime::SpotLightComponent> LightObj;
	CoreEngine::Runtime::PrimitiveComponent* LightCube;
	CoreEngine::Render::OpenGL::OpenGLShader shade;
	CoreEngine::Render::OpenGL::OpenGLVertexArrayObject arrObj;
	CoreEngine::Render::OpenGL::OpenGLVertexBufferObject vertObj;
};

class FirstLevel : public CoreEngine::Level
{
	virtual void ActorInitialize() override
	{
		auto* controller = GetWorld()->SpawnActor<MyController>(nullptr);
		auto* pawn = GetWorld()->SpawnActor<Quad>(controller);
		auto* acc = GetWorld()->SpawnActor<Light>(controller);
		auto* ac = GetWorld()->SpawnActor<LightActor>(controller);
		controller->Possess(pawn);

		Level::ActorInitialize();
	}
};


int main(int argc, char** argv)
{

	CoreEngine::ApplicationOptions options("Test", argv[0], nullptr);
	auto app = MakeUniquePtr<Editor::EditorApplication>(options);
	app->CreateApp();
	auto* level = new FirstLevel();
	app->Get()->InstanceEngine->GetWorld()->OpenLevel(level);
	//app->PushLayer(new RenderLayer);

	app->Start();
}

