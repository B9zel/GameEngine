#include <Render/includes/Model.h>	
#include <Render/includes/VertexArrayObject.h>
#include <Render/includes/VertextBufferObject.h>
#include <Render/includes/ElementBufferObject.h>
#include <Render/includes/Texture.h>
#include <glad/glad.h>
#include <assimp/DefaultLogger.hpp>

namespace CoreEngine
{
	namespace Render
	{
		Model::~Model()
		{
			ClearModel();
		}

		Model::Model(Model&& Other)
		{
			m_EBO = std::move(Other.m_EBO);
			m_Indeces = std::move(Other.m_Indeces);
			m_Textures = std::move(Other.m_Textures);
			m_VAO = std::move(Other.m_VAO);
			m_VBO = std::move(Other.m_VBO);
			m_Vertices = std::move(Other.m_Vertices);
			m_HasLoadModel = Other.m_HasLoadModel;

			Other.m_HasLoadModel = false;
		}

		Model::Model()
		{
			m_HasLoadModel = false;
			m_VAO = VertexArrayObject::CreateVertexObject();
			m_VBO = VertexBufferObject::CreateVertexBufferObject();
			m_EBO = ElementBufferObject::CreateElementBuffer();
		}

		Model* Model::CreateModel()
		{
			return new Model;
		}

		void Model::ClearModel()
		{
			m_Vertices.clear();
			m_Textures.clear();

			m_VAO->DeleteVertexObject();
			m_VBO->DeleteBuffer();
		}

		void Model::SetupModel(aiMesh* Mesh, const aiScene* Scene, const SpecificationVertexData& Data)
		{

			for (int64 i = 0; i < Mesh->mNumVertices; i++)
			{
				Vertex vertex;
				aiVector3D& Location = Mesh->mVertices[i];
				vertex.Position.SetX(Location.x);
				vertex.Position.SetY(Location.y);
				vertex.Position.SetZ(Location.z);

				vertex.Normal = FVector(0.0f);

				/*aiVector3D& Normals = Mesh->mNormals ? Mesh->mNormals[i] : aiVector3D(0, 1, 0);

				vertex.Normal.SetX(Normals.x);
				vertex.Normal.SetX(Normals.y);
				vertex.Normal.SetX(Normals.z);*/


				//EG_LOG(CORE, ELevelLog::INFO, "Vertex {0}, {1}, {2} Normal {3}, {4}, {5}", vertex.Position.GetX(), vertex.Position.GetY(), vertex.Position.GetZ(), vertex.Normal.GetX(), vertex.Normal.GetY(), vertex.Normal.GetZ());


				if (Mesh->mTextureCoords[0])
				{
					vertex.TexCoord.x = Mesh->mTextureCoords[0][i].x;
					vertex.TexCoord.y = Mesh->mTextureCoords[0][i].y;
				}
				else
				{
					vertex.TexCoord = FVector2(0.0f, 0.0f);
				}
				vertex.ID = Data.ObjectID;

				m_Vertices.emplace_back(vertex);
			}

			for (int64 i = 0; i < Mesh->mNumFaces; i++)
			{
				aiFace& Face = Mesh->mFaces[i];
				for (int64 j = 0; j < Face.mNumIndices; j++)
				{
					m_Indeces.emplace_back(Face.mIndices[j]);
				}
			}

			// Generation normals
			for (int64 i = 0; i < m_Indeces.size(); i += 3)
			{
				int32 First = m_Indeces[i];
				int32 Second = m_Indeces[i + 1];
				int32 Third = m_Indeces[i + 2];

				FVector& FirstVertex = m_Vertices[First].Position;
				FVector& SecondVertex = m_Vertices[Second].Position;
				FVector& ThirdVertex = m_Vertices[Third].Position;

				FVector OneEdge = SecondVertex - FirstVertex;
				FVector TwoEdge = ThirdVertex - FirstVertex;

				FVector normal = OneEdge.Cross(TwoEdge).SafeNormalize();
				m_Vertices[First].Normal += normal;
				m_Vertices[Second].Normal += normal;
				m_Vertices[Third].Normal += normal;

			}

			for (int64 i = 0; i < m_Vertices.size(); i++)
			{
				if (m_Vertices[i].Normal.LengthSquared() > 0.0f)
				{
					m_Vertices[i].Normal.SafeNormalize();
				}
				else
				{
					m_Vertices[i].Normal = FVector(0, 1, 0);
				}
			}
			
			// Better way to everyone element has 4 bytes or equal count of bytes
			const int32 CountFloat = sizeof(m_Vertices[0]) /  sizeof(float); // 3 pos, 3 normal, 2 texcoord

			m_VAO->CreateVertexArray();
			m_VBO->CreaterBuffer(m_Vertices.data(), m_Vertices.size() * CountFloat, ETypeData::FLOAT, ETypeDraw::STATIC, *m_VAO.get());
			m_EBO->CreateBuffer(m_Indeces.data(), m_Indeces.size(), ETypeData::UNSIGNED_INT, ETypeDraw::STATIC, *m_VAO.get());
			std::cout << offsetof(Vertex, Vertex::TexCoord) << std::endl;
			m_VAO->SetupIntorprit(0, 3, 9, ETypeData::FLOAT, *m_VBO.get());
			m_VAO->SetupIntorprit(1, 3, 9, ETypeData::FLOAT, *m_VBO.get(), 3);
			m_VAO->SetupIntorprit(2, 2, 9, ETypeData::FLOAT, *m_VBO.get(), 6);
			m_VAO->SetupIntorprit(3, 1, 9, ETypeData::INT, *m_VBO.get(), 8);

		}

		const DArray<uint32>& Model::GetIndeces() const
		{
			return m_Indeces;
		}

		const UniquePtr<VertexArrayObject>& Model::GetVertexArrayObject() const
		{
			return m_VAO;
		}

	}
}