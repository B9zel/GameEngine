#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Core/includes/Base.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>
#include <Runtime/includes/PrimitiveComponent.h>
#include <Render/includes/Model.h>


namespace CoreEngine
{
	class StaticMeshProxy;
	namespace Render
	{
		class Model;
	}
	namespace Runtime
	{
		class MeshComponent : public PrimitiveComponent
		{
		public:

			MeshComponent();

		public:

			bool LoadMesh(const StringView Path);
			void RemoveMesh();

			virtual PrimitiveProxy* GetSceneProxy() const override;
			virtual PrimitiveProxy* GetUpdateProxy() const override;

		private:

			void SetupNode(aiNode* Node, const aiScene* Scene);
			bool CheckCorrectNormals(aiNode* Node, const aiScene* Scene);
			bool CheckMeshNormal(aiMesh* Node, const aiScene* Scene);
			void ReloadMesh(Assimp::Importer& Importer, aiScene** Scene);

			struct ModelDeleter
			{
				void operator()(Render::Model* Ptr) const noexcept
				{
					delete Ptr;
				}
			};

		private:

			DArray<UniquePtr<CoreEngine::Render::Model, ModelDeleter>> m_Models;
			UniquePtr<StaticMeshProxy> m_Proxy;
			String m_PathToMesh;

			//Test
			UniquePtr<Render::Shader> m_Shader;
		};
	}
}