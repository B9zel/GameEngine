#include <Runtime/includes/MeshComponent.h>

#include <Core/includes/StaticMeshProxy.h>
#include <Runtime/includes/Actor.h>
#include <Core/includes/AssetManager.h>
#include <Render/includes/VertexArrayObject.h>
#include <Render/includes/ElementBufferObject.h>
#include <Render/includes/Material.h>
#include <Core/includes/Application.h>
#include <Render/includes/Material.h>
#include <glad/glad.h>

DECLARE_LOG_CATEGORY_EXTERN(MESH_COMPONENT_LOG);

MeshComponent::MeshComponent(const CoreEngine::InitializeObject& Object) : PrimitiveComponent(Object)
{
	auto* AssetManager = Engine::Get()->GetAssetManager();
	m_Shader.push_back(AssetManager->LoadShader(CoreEngine::Application::Get()->GetAppOptions().pathToProject + "/Shaders/StaticMeshBaseShader.glsl"));
	// m_Shader.push_back(Render::Shader::CreateShader());
	// m_Shader.push_back(Render::Shader::CreateShader());
	m_Proxy = MakeUniquePtr<CoreEngine::StaticMeshProxy>();

	SourceShader DefaultShader =
		AssetManager->LoadStringShaderFromFile(CoreEngine::Application::Get()->GetAppOptions().pathToProject + "/Shaders/DefaultShader.glsl");
	auto DefaultShaderAsset = MakeSharedPtr<CoreEngine::Render::ShaderAsset>();
	DefaultShaderAsset->CustomShader = DefaultShader;

	auto* Mat = CreateObject<RMaterial>(this);
	Mat->SetShaderAsset(DefaultShaderAsset);

	materials.push_back(Mat);
	// auto& Shaders = m_Shader[0]->LoadShader((Application::Get()->GetAppOptions().pathToProject + "/Shaders/StaticMeshBaseShader.glsl").c_str());
	// m_Shader[0]->CompileShader(Shaders.first, Shaders.second);
	// Shaders = m_Shader[1]->LoadShader((Application::Get()->GetAppOptions().pathToProject + "/Shaders/IdVisualShader.glsl").c_str());
	// m_Shader[1]->CompileShader(Shaders.first, Shaders.second);
}

bool MeshComponent::LoadMesh(const StringView Path)
{
	if (!m_Models.empty())
	{
		RemoveMesh();
	}
	Assimp::Importer importer;
	const aiScene* Scene = importer.ReadFile(Path.data(), aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_ValidateDataStructure |
															  aiProcess_ImproveCacheLocality);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		EG_LOG(MESH_COMPONENT_LOG, ELevelLog::ERROR, "Can't load 3d model {0}", Path.data());
		return false;
	}
	m_PathToMesh = Path;
	SetupNode(Scene->mRootNode, Scene);

	return true;
}

void MeshComponent::RemoveMesh()
{
	m_Models.clear();
	m_PathToMesh.clear();
}

CoreEngine::PrimitiveProxy* MeshComponent::GetSceneProxy() const
{
	return m_Proxy.get();
}

CoreEngine::PrimitiveProxy* MeshComponent::GetUpdateProxy() const
{
	m_Proxy->ClearData();
	m_Proxy->SetTransformMatrix(MakeMatrixMesh());

	m_Proxy->SetUUID(&GetOwner()->GetUUID());
	// m_Proxy->SetViewLocation(GetOwner()->GetWorld()->GetControllerLocation());
	// m_Proxy->AddLightLocation(FVector(3, 2, -7));
	if (!m_Models.empty())
	{
		for (uint64 i = 0; i < m_Shader.size(); i++)
		{
			CoreEngine::ParamOfShaderDesc Desc;
			Desc.shader = m_Shader[i]->GetHandle();

			// Desc.ElementObject = m_Models[i]->GetEBO();
			Desc.TextureNames = m_Shader[i]->GetNamesOfTexture();
			Desc.HasAllMatrix = m_Shader[i]->GetHasAllMatrix();

			m_Proxy->AddShader(Desc);
		}
		for (auto* Mat : materials)
		{
			m_Proxy->AddMaterial(Mat);
		}

		// for (uint64 i = 0; i < m_Shader.size(); i++)
		//{
		//	ParamOfShaderDesc Desc;
		//	Desc.shader = m_Shader[i]->GetHandle();
		//	Desc.ArrayObject = m_Models[0]->GetVertexArrayObject();
		//	Desc.ElementObject = m_Models[0]->GetEBO();
		//	Desc.TextureNames = m_Shader[i]->GetNamesOfTexture();
		//	Desc.HasAllMatrix = m_Shader[i]->GetHasAllMatrix();

		//	m_Proxy->AddShader(Desc);
		//	// m_Proxy->AddShader(m_Shader[i]->GetHandle(), m_Models[0]->GetVertexArrayObject()->GetHandle(), m_Models[0]->GetEBO()->GetHandle());
		//}
	}
	for (uint64 i = 0; i < m_Models.size(); i++)
	{
		m_Proxy->AddIndeces(m_Models[i]->GetIndeces());
		m_Proxy->AddArrayObject(m_Models[i]->GetVertexArrayObject());
	}
	return m_Proxy.get();
}

void MeshComponent::SetupNode(aiNode* Node, const aiScene* Scene)
{
	for (int64 i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		UniquePtr<CoreEngine::Render::Model, ModelDeleter> NewModel(CoreEngine::Render::Model::CreateModel());
		CoreEngine::SpecificationVertexData Data;
		Data.ObjectID = GetOwner()->GetUUID().GetID();
		NewModel->SetupModel(mesh, Scene, Data);
		m_Models.emplace_back(std::move(NewModel));
	}

	for (int64 i = 0; i < Node->mNumChildren; i++)
	{
		SetupNode(Node->mChildren[i], Scene);
	}
}

bool MeshComponent::CheckCorrectNormals(aiNode* Node, const aiScene* Scene)
{
	for (int64 i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		if (!CheckMeshNormal(mesh, Scene))
		{
			return false;
		}
	}
	return true;
}

bool MeshComponent::CheckMeshNormal(aiMesh* Mesh, const aiScene* Scene)
{
	for (int64 i = 0; i < Mesh->mNumVertices; i++)
	{
		if (Mesh->mVertices[i].x == 0 && Mesh->mVertices[i].y == 0 && Mesh->mVertices[i].z == 0)
		{
			return false;
		}
	}
	return true;
}

void MeshComponent::ReloadMesh(Assimp::Importer& Importer, aiScene** Scene)
{
	Importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_NORMALS);
	//	*Scene = Importer.ApplyPostProcessing(aiProcess_RemoveComponent | aiProcess_GenSmoothNormals);
}
