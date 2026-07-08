#pragma once
#include <Runtime/includes/Actor.h>
#include <Runtime/includes/SpotLightComponent.h>
#include <Runtime/includes/PrimitiveComponent.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Runtime/includes/PointLightComponent.h>
#include <Core/includes/Application.h>
#include <TestLightActor.generated.h>

RCLASS()
class LightActor : public Actor
{

	GENERATED_BODY()

public:

	LightActor(const CoreEngine::InitializeObject& Object) : Actor(Object)
	{
		LightObj = CreateSubObject<SpotLightComponent>("Spot light");
		LightObj->SetColor(FVector(0, 1, 0));
		LightObj->SetConstant(1.0f);
		LightObj->SetLinear(0.2f);
		LightObj->SetQuadratic(0.3f);
		LightObj->SetIntencity(10);
		LightObj->SetCutOff(50);
		LightObj->SetOuterCutOff(60);
		// CoreEngine::Runtime::PoinLightComponent::GetStaticClass();
		String Path = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
		/*auto& shadPair = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/LightShader.glsl").c_str());
		shade.CompileShader(shadPair.first, shadPair.second);

		arrObj.CreateVertexArray();
		vertObj.CreateBuffer(arr, 288, CoreEngine::ETypeData::FLOAT, ETypeStorageDraw::STATIC, arrObj);
		arrObj.SetupIntorprit(0, 3, 8, CoreEngine::ETypeData::FLOAT, vertObj);
		arrObj.SetupIntorprit(1, 2, 8, CoreEngine::ETypeData::FLOAT, vertObj, 6);
		LightCube = CreateSubObject<CoreEngine::Runtime::PrimitiveComponent>("Light cube");
		LightCube->GetSceneProxy()->CountVertex = 108;
		LightCube->GetSceneProxy()->AddShaderWithArrayObject(&shade, &arrObj, nullptr);
		LightCube->GetSceneProxy()->SetTransformMatrix(LightCube->MakeMatrixMesh());*/
	}

	virtual void Update(float delta) override
	{
		Actor::Update(delta);

		// LightCube->GetSceneProxy()->
	}

private:

	RPROPERTY(EditorVisible);
	SpotLightComponent* LightObj;

	PrimitiveComponent* LightCube;
	// CoreEngine::Render::OpenGL::OpenGLShader shade;
	CoreEngine::Render::OpenGL::OpenGLVertexArrayObject arrObj;
	CoreEngine::Render::OpenGL::OpenGLVertexBufferObject vertObj;

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
