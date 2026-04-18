#pragma once
#include <Core/includes/Base.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShader.h>
#include <Runtime/includes/PrimitiveComponent.h>
#include <Render/includes/Model.h>
#include <MeshComponent.generated.h>

namespace CoreEngine
{
	class StaticMeshProxy;
	namespace Render
	{
		class Model;
	}
} // namespace CoreEngine
RCLASS();
class MeshComponent : public PrimitiveComponent
{
	GENERATED_BODY()

public:

	MeshComponent(const CoreEngine::InitializeObject& Object);

public:

	bool LoadMesh(const StringView Path);
	void RemoveMesh();

	virtual CoreEngine::PrimitiveProxy* GetSceneProxy() const override;
	virtual CoreEngine::PrimitiveProxy* GetUpdateProxy() const override;

private:

	void SetupNode(aiNode* Node, const aiScene* Scene);
	bool CheckCorrectNormals(aiNode* Node, const aiScene* Scene);
	bool CheckMeshNormal(aiMesh* Node, const aiScene* Scene);
	void ReloadMesh(Assimp::Importer& Importer, aiScene** Scene);

	struct ModelDeleter
	{
		void operator()(CoreEngine::Render::Model* Ptr) const noexcept
		{
			delete Ptr;
		}
	};

private:

	DArray<UniquePtr<CoreEngine::Render::Model, ModelDeleter>> m_Models;
	UniquePtr<CoreEngine::StaticMeshProxy> m_Proxy;
	String m_PathToMesh;

public:

	// Test
	DArray<CoreEngine::Render::Shader*> m_Shader;
};
