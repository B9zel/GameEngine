#pragma once
#include <Runtime/includes/Actor.h>
#include <Runtime/includes/DirectionLightComponent.h>
#include <Runtime/includes/PrimitiveComponent.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Runtime/includes/SpotLightComponent.h>
#include <Runtime/includes/MeshComponent.h>

#include <Core/includes/PrimitiveProxy.h>
#include <TestLight.generated.h>

RCLASS()
class Light : public CoreEngine::Runtime::Actor
{
	GENERATED_BODY()

public:

	Light(const CoreEngine::InitializeObject& Object);

	virtual void Update(float deltaTime) override
	{
		CoreEngine::Runtime::Actor::Update(deltaTime);
		// AddActorRotation(FVector(0.1, 0, 0));
		LightCube->GetSceneProxy()->SetTransformMatrix(LightCube->MakeMatrixMesh());
		// EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Actor {0} {1} {2}", GetActorRotation().GetX(), GetActorRotation().GetY(), GetActorRotation().GetZ());
		// auto Class = CoreEngine::Engine::Get()->GetReflectionManger()->FindMetaClas("Object");
		/*	auto Prop = GetStaticClass()->GetPropertyFieldByName(this, "TestVar");
			if (Prop)
			{

				EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0} {1} {2}", Prop->Name, Prop->IsPointer, *Prop->GetSourcePropertyByName<int>(this));
				Prop->SetSourceProperty(this, *Prop->GetSourcePropertyByName<int>(this) + 1);
			}

			EG_LOG(CoreEngine::CORE, ELevelLog::INFO, TestVar);*/
		// EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Direction {0} {1} {2}", LightObj->GetComponentRotation().GetX(), LightObj->GetComponentRotation().GetY(),
		// LightObj->GetComponentRotation().GetZ()); AddActorRotation(FVector(100 * deltaTime, 0, 0));
	}

private:

	CoreEngine::Runtime::MeshComponent* mesh = nullptr;
	// CoreEngine::ObjectPtr<CoreEngine::Runtime::DirectionLightComponent> LightObj;
	CoreEngine::Runtime::PrimitiveComponent* LightCube;
	CoreEngine::Render::Shader* shade;
	UniquePtr<CoreEngine::Render::VertexArrayObject> arrObj;
	CoreEngine::Render::OpenGL::OpenGLVertexBufferObject vertObj;
	// CoreEngine::ObjectPtr<CoreEngine::Runtime::SpotLightComponent> LightObjh;

	CoreEngine::Render::RHI::HandleVAO VAO;
	CoreEngine::Render::RHI::BufferHandle VBO;

	float arr[288] = {
		-0.5f, -0.5f, -0.5f, 0.0f,	0.0f,  -1.0f, 0.0f, 0.0f, 0.5f,	 -0.5f, -0.5f, 0.0f,  0.0f,	 -1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  -0.5f, 0.0f,	0.0f,  -1.0f, 1.0f, 1.0f, 0.5f,	 0.5f,	-0.5f, 0.0f,  0.0f,	 -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f,  -0.5f, 0.0f,	0.0f,  -1.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,	 -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,	 0.0f,	0.0f,  1.0f,  0.0f, 0.0f, 0.5f,	 -0.5f, 0.5f,  0.0f,  0.0f,	 1.0f,	1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,	 0.0f,	0.0f,  1.0f,  1.0f, 1.0f, 0.5f,	 0.5f,	0.5f,  0.0f,  0.0f,	 1.0f,	1.0f, 1.0f,
		-0.5f, 0.5f,  0.5f,	 0.0f,	0.0f,  1.0f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,	 1.0f,	0.0f, 0.0f,

		-0.5f, 0.5f,  0.5f,	 -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, -0.5f, 0.5f,	-0.5f, -1.0f, 0.0f,	 0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,	 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,	 -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,	0.5f,  -1.0f, 0.0f,	 0.0f,	1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,	 1.0f,	0.0f,  0.0f,  1.0f, 0.0f, 0.5f,	 0.5f,	-0.5f, 1.0f,  0.0f,	 0.0f,	1.0f, 1.0f,
		0.5f,  -0.5f, -0.5f, 1.0f,	0.0f,  0.0f,  0.0f, 1.0f, 0.5f,	 -0.5f, -0.5f, 1.0f,  0.0f,	 0.0f,	0.0f, 1.0f,
		0.5f,  -0.5f, 0.5f,	 1.0f,	0.0f,  0.0f,  0.0f, 0.0f, 0.5f,	 0.5f,	0.5f,  1.0f,  0.0f,	 0.0f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f,	-1.0f, 0.0f,  0.0f, 1.0f, 0.5f,	 -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,	1.0f, 1.0f,
		0.5f,  -0.5f, 0.5f,	 0.0f,	-1.0f, 0.0f,  1.0f, 0.0f, 0.5f,	 -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,	 0.0f,	-1.0f, 0.0f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,	0.0f, 1.0f,

		-0.5f, 0.5f,  -0.5f, 0.0f,	1.0f,  0.0f,  0.0f, 1.0f, 0.5f,	 0.5f,	-0.5f, 0.0f,  1.0f,	 0.0f,	1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,	 0.0f,	1.0f,  0.0f,  1.0f, 0.0f, 0.5f,	 0.5f,	0.5f,  0.0f,  1.0f,	 0.0f,	1.0f, 0.0f,
		-0.5f, 0.5f,  0.5f,	 0.0f,	1.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,	-0.5f, 0.0f,  1.0f,	 0.0f,	0.0f, 1.0f,
	};
};
