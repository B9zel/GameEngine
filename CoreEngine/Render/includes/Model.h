#pragma once
#include <Core/includes/Base.h>
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

			void SetupModel(aiMesh* Mesh, const aiScene* Scene);

			const DArray<uint32>& GetIndeces() const;
			const UniquePtr<VertexArrayObject>& GetVertexArrayObject() const;

			//Test
			const UniquePtr<ElementBufferObject>& GetEBO() { return m_EBO; }
			const UniquePtr<VertexBufferObject>& GetVBO() { return m_VBO; }

		private:

			DArray<Vertex> m_Vertices;
			DArray<uint32> m_Indeces;
			DArray<Texture2D*> m_Textures;
			UniquePtr<VertexBufferObject> m_VBO;
			UniquePtr<VertexArrayObject> m_VAO;
			UniquePtr<ElementBufferObject> m_EBO;
			bool m_HasLoadModel;

			friend Runtime::MeshComponent;
		};
	}


}