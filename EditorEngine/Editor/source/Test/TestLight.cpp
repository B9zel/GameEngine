#include <Editor/includes/Test/TestLight.h>
#include <Core/includes/AssetManager.h>
#include <Render/includes/RenderDevice.h>
#include <Render/includes/VertexArrayObject.h>
#include <Core/includes/Engine.h>

Light::Light(const CoreEngine::InitializeObject& Object) : CoreEngine::Runtime::Actor(Object)
{
	mesh = CreateSubObject<CoreEngine::Runtime::MeshComponent>("Mesh");
	mesh->LoadMesh("C:/Projects/3D_Models/Cube.obj");

	auto* AssetManager = CoreEngine::Engine::Get()->GetAssetManager();
	auto& Device = CoreEngine::Engine::Get()->GetRenderDevice();

	// Device->CreateVAO()
	String Path = CoreEngine::Application::Get()->GetAppOptions().pathToProject;
	shade = AssetManager->LoadShader(Path + "/Shaders/LightShader.glsl");

	/*auto& shadPair = CoreEngine::Render::Shader::LoadShader((Path + "/Shaders/LightShader.glsl").c_str());
shade.CompileShader(shadPair.first, shadPair.second);*/
	// LightObjh = CreateSubObject<CoreEngine::Runtime::SpotLightComponent>("Spot light2");

	// arrObj = CoreEngine::Render::VertexArrayObject::CreateVertexObject();
	// arrObj
	//	->SetupIntorprit(Device, 0, 3, 8, CoreEngine::ETypeData::FLOAT, vertObj, )

	//	/*arrObj.CreateVertexArray();*/

	//	vertObj.CreateBuffer(arr, 288, CoreEngine::ETypeData::FLOAT, ETypeStorageDraw::STATIC, arrObj);
	// arrObj->SetupIntorprit(0, 3, 8, CoreEngine::ETypeData::FLOAT, vertObj);
	// arrObj.SetupIntorprit(1, 2, 8, CoreEngine::ETypeData::FLOAT, vertObj, 7);

	// SetActorLocation(FVector(3, 2, -7));
	// SetActorScale(FVector(0.2));
	// LightCube = CreateSubObject<CoreEngine::Runtime::PrimitiveComponent>("Primitive type");
	// LightCube->GetSceneProxy()->CountVertex = 108;
	// LightCube->GetSceneProxy()->AddShaderWithArrayObject(&shade, &arrObj, nullptr);

	/*LightObj = CreateSubObject<CoreEngine::Runtime::DirectionLightComponent>();
	LightObj->SetIntencity(10);
	LightObj->SetComponentRotation(FVector(0, 0, 0));
	LightObj->SetComponentLocation(FVector(3, 2, -7));*/
}
