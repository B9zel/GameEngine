#pragma once
#include <Runtime/includes/Actor.h>
#include <Runtime/includes/DirectionLightComponent.h>
#include <Runtime/includes/PrimitiveComponent.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Core/includes/PrimitiveProxy.h>
#include <TestLight.generated.h>



RCLASS()
class Light : public CoreEngine::Runtime::Actor
{
	GENERATED_BODY()

public:

	Light(const CoreEngine::InitializeObject& Object) : Actor(Object)
	{
		String Path = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		auto& shadPair = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/LightShader.glsl").c_str());
		shade.CompileShader(shadPair.first, shadPair.second);

		arrObj.CreateVertexArray();
		vertObj.CreaterBuffer(arr, 288, CoreEngine::ETypeData::FLOAT, ETypeDraw::STATIC, arrObj);
		arrObj.SetupIntorprit(0, 3, 8, CoreEngine::ETypeData::FLOAT, vertObj);

		SetActorLocation(FVector(3, 2, -7));
		SetActorScale(FVector(0.2));
		LightCube = CreateSubObject<CoreEngine::Runtime::PrimitiveComponent>("Primitive type");
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
		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Actor {0} {1} {2}", GetActorRotation().GetX(), GetActorRotation().GetY(), GetActorRotation().GetZ());
		//auto Class = CoreEngine::Engine::Get()->GetReflectionManger()->FindMetaClas("Object");
	/*	auto Prop = GetStaticClass()->GetPropertyFieldByName(this, "TestVar");
		if (Prop)
		{

			EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0} {1} {2}", Prop->Name, Prop->IsPointer, *Prop->GetSourcePropertyByName<int>(this));
			Prop->SetSourceProperty(this, *Prop->GetSourcePropertyByName<int>(this) + 1);
		}

		EG_LOG(CoreEngine::CORE, ELevelLog::INFO, TestVar);*/
		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Direction {0} {1} {2}", LightObj->GetComponentRotation().GetX(), LightObj->GetComponentRotation().GetY(), LightObj->GetComponentRotation().GetZ());
		//AddActorRotation(FVector(100 * deltaTime, 0, 0));
	}

private:

	CoreEngine::ObjectPtr<CoreEngine::Runtime::DirectionLightComponent> LightObj;
	CoreEngine::Runtime::PrimitiveComponent* LightCube;
	CoreEngine::Render::OpenGL::OpenGLShader shade;
	CoreEngine::Render::OpenGL::OpenGLVertexArrayObject arrObj;
	CoreEngine::Render::OpenGL::OpenGLVertexBufferObject vertObj;

	float arr[288] = {
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
};