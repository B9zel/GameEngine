#include <Runtime/includes/MeshComponent.h>

#include <Core/includes/StaticMeshProxy.h>
#include <Runtime/includes/Actor.h>
//#include <Core/includes/World.h>
#include <glad/glad.h>


namespace CoreEngine
{
	namespace Runtime
	{
		DECLARE_LOG_CATEGORY_EXTERN(MESH_COMPONENT_LOG);

		MeshComponent::MeshComponent(const InitializeObject& Object) : PrimitiveComponent(Object)
		{
			m_Shader.push_back(Render::Shader::CreateShader());
			//m_Shader.push_back(Render::Shader::CreateShader());
			m_Proxy = MakeUniquePtr<StaticMeshProxy>();

			auto& Shaders = m_Shader[0]->LoadShader((Application::Get()->GetAppOptions().pathToProject + "/Shaders/StaticMeshBaseShader.glsl").c_str());
			m_Shader[0]->CompileShader(Shaders.first, Shaders.second);
			//Shaders = m_Shader[1]->LoadShader((Application::Get()->GetAppOptions().pathToProject + "/Shaders/IdVisualShader.glsl").c_str());
			//m_Shader[1]->CompileShader(Shaders.first, Shaders.second);
		}


		bool MeshComponent::LoadMesh(const StringView Path)
		{
			if (!m_Models.empty())
			{
				RemoveMesh();
			}
			Assimp::Importer importer;
			const aiScene* Scene = importer.ReadFile(Path.data(), aiProcess_FlipUVs | aiProcess_Triangulate |
				aiProcess_GenNormals |
				aiProcess_ValidateDataStructure |
				aiProcess_ImproveCacheLocality);

			if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
			{
				EG_LOG(MESH_COMPONENT_LOG, ELevelLog::ERROR, "Can't load 3d model {0}", Path.data());
				return false;
			}
			m_PathToMesh = Path;
			/*if (CheckCorrectNormals(Scene->mRootNode, Scene));
			{

			}*/
			SetupNode(Scene->mRootNode, Scene);

			return true;
		}

		void MeshComponent::RemoveMesh()
		{
			m_Models.clear();
			m_PathToMesh.clear();
		}

		PrimitiveProxy* MeshComponent::GetSceneProxy() const
		{
			return m_Proxy.get();
		}

		PrimitiveProxy* MeshComponent::GetUpdateProxy() const
		{
			m_Proxy->ClearData();
			m_Proxy->SetTransform(Transform);
			
			m_Proxy->SetUUID(&GetOwner()->GetUUID());
			//m_Proxy->SetViewLocation(GetOwner()->GetWorld()->GetControllerLocation());
			//m_Proxy->AddLightLocation(FVector(3, 2, -7));
			if (!m_Models.empty())
			{
				for (uint64 i = 0; i < m_Shader.size(); i++)
				{
					m_Proxy->AddShaderWithArrayObject(m_Shader[i].get(), m_Models[0]->GetVertexArrayObject().get(), m_Models[0]->GetEBO().get());
				}
			}
			for (uint64 i = 0; i < m_Models.size(); i++)
			{
				m_Proxy->AddIndeces(m_Models[i]->GetIndeces());
				m_Proxy->AddArrayObject(m_Models[i]->GetVertexArrayObject().get());
			}
			return m_Proxy.get();
		}

		void MeshComponent::SetupNode(aiNode* Node, const aiScene* Scene)
		{
			for (int64 i = 0; i < Node->mNumMeshes; i++)
			{
				aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
				UniquePtr<Render::Model, ModelDeleter> NewModel(Render::Model::CreateModel());
				SpecificationVertexData Data;
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

	}
}