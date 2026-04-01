#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/RenderHardwareInterface.h>
#include <Math/includes/Vector.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace CoreEngine
{

	struct Vertex
	{
		FVector Position;
		FVector Normal;
		FVector2 TexCoord;
		int32 ID;
	};

	struct SpecificationVertexData
	{
		int32 ObjectID;
	};

	namespace Render
	{
		class VertexBufferObject;
		class VertexArrayObject;
		class Texture2D;
		class ElementBufferObject;
		namespace Runtime
		{
			class MeshComponent;
		}

		class Model
		{
		public:

			~Model();
			Model(Model&& Other);
			Model(const Model&) = delete;

		private:

			Model();

		public:

			static Model* CreateModel();
			void ClearModel();

			void SetupModel(aiMesh* Mesh, const aiScene* Scene, const SpecificationVertexData& Data);

			const DArray<uint32>& GetIndeces() const;
			const RHI::HandleVAO& GetVertexArrayObject() const;

			// Test
			const RHI::BufferHandle& GetEBO()
			{
				return m_HandleEBO;
			}
			const RHI::BufferHandle& GetVBO()
			{
				return m_HandleVBO;
			}

		private:

			DArray<Vertex> m_Vertices;
			DArray<uint32> m_Indeces;
			DArray<Texture2D*> m_Textures;
			UniquePtr<VertexBufferObject> m_VBO;
			UniquePtr<VertexArrayObject> m_VAO;
			UniquePtr<ElementBufferObject> m_EBO;
			bool m_HasLoadModel;

			RHI::BufferHandle m_HandleVBO;
			RHI::BufferHandle m_HandleEBO;
			RHI::HandleVAO m_HandleVAO;

			friend Runtime::MeshComponent;
		};
	} // namespace Render

} // namespace CoreEngine
