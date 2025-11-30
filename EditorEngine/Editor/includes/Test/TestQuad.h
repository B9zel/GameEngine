#pragma once
#include <Runtime/includes/Pawn.h>
#include <Runtime/includes/MeshComponent.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLElementBufferObject.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Platform/Renderer/OpenGL/include/OpenGLTexture.h>
#include <Runtime/includes/InputComponent.h>
#include <ReflectionSystem/Include/PropertyField.h>
#include <TestQuad.generated.h>




RCLASS()
class Quad : public CoreEngine::Runtime::Pawn
{
	GENERATED_BODY()

public:

	Quad(const CoreEngine::InitializeObject& Object) : Pawn(Object)
	{
		mesh = CreateSubObject<CoreEngine::Runtime::MeshComponent>("Mesh");
		mesh->LoadMesh("C:/Projects/3D_Models/Table.obj");
		mesh->SetComponentScale(FVector(0.8));
		mesh->AddComponentLocation(FVector(0, -1, 0));
		//mesh->SetComponentScale(FVector(2));
		//mesh->AddComponentLocation(FVector(0, 0, -5));
		//mesh->SetComponentScale(FVector(10));
		String Path = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		auto& shadPair = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/StaticMeshBaseShader.glsl").c_str());
		auto& shad = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/IdVisualShader.glsl").c_str());
		Shader.CompileShader(shadPair.first, shadPair.second);
		//ShaderID.CompileShader(shad.first, shad.second);
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
	CoreEngine::Render::OpenGL::OpenGLShader ShaderID;

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

	/*RPROPERTY(EditorVisible);
	int8 Test8 = 0;
	RPROPERTY(EditorVisible);
	int16 Test16 = 0;
	RPROPERTY(EditorVisible);
	int32 Test32 = 0;
	RPROPERTY(EditorVisible);
	int64 TestTestTestTest64 = 0;
	RPROPERTY(EditorVisible);
	uint8 TestU8 = 0;
	RPROPERTY(EditorVisible);
	uint16 TestU16 = 0;
	RPROPERTY(EditorVisible);
	uint32 TestU32 = 0;
	RPROPERTY(EditorVisible);
	uint64 TestTestTestTestU64 = 0;
	RPROPERTY(EditorVisible);
	String TestTestTestTestString ;
	RPROPERTY(EditorVisible);
	FVector TestTestTestTestVector ;*/

private:

	float arri[288] = {
	-0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // 0
	 0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // 1
	 0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // 2
	-0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // 3

	// Back axis
	 -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // 4
	 0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // 5
	 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // 6
	-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // 7

	// left edge
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0,0.0f, 0.0, // 8
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0,1.0f, 0.0, // 9
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0,1.0f, 1.0, // 10
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0 ,0.0f, 1.0, // 11

	// right edge
	0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // 12
	0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // 13
	0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // 14
	0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // 15

	// Top edge
	-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // 16
	0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // 17
	0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // 18
	-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // 19

	// bottom edge
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // 20
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // 21
	0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // 22
	-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f
	};

	int Index[37] = {
		0, 1, 2, 2, 3, 0,   // Передняя грань
		4, 5, 6, 6, 7, 4,   // Задняя грань
		8, 9, 10, 10, 11, 8, // Левая грань
		12, 13, 14, 14, 15, 12, // Правая грань
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};
};

//RCLASS()
//class QuadTest : public CoreEngine::Runtime::Pawn
//{
//	GENERATED_BODY()
//};
