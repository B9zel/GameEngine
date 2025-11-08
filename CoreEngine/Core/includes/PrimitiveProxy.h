#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/Shader.h>
#include <Render/includes/VertexArrayObject.h>
#include <Render/includes/ElementBufferObject.h>
#include <Render/includes/Texture.h>
#include <Math/includes/Transform.h>


namespace CoreEngine
{
	class UUID;

	class PrimitiveProxy
	{
	public:

		PrimitiveProxy() = default;

	public:

		virtual const HashTableMap<Render::Shader*, Pair<Render::VertexArrayObject*, Render::ElementBufferObject*>>& GetShaders() const;
		virtual void AddShaderWithArrayObject(Render::Shader* shaderKey, Render::VertexArrayObject* arrayValue, Render::ElementBufferObject* elementValue);
		virtual void AddTexture(Render::Texture* NewTexture);
		virtual void SetTransform(const Transform& transform);
		const DArray<Render::Texture*>& GetTextures() const;
		const Transform& GetTransform() const;
		void SetViewLocation(const FVector& Location);
		void AddLightLocation(const FVector& Location);
		const DArray<FVector>& GetLocationLights() const;
		const FVector& GetViewLocation() const;
		void SetUUID(const UUID* uuid);
		const UUID* GetUUID() const;


	protected:

		HashTableMap<Render::Shader*, Pair<Render::VertexArrayObject*, Render::ElementBufferObject*>> Shaders;
		DArray<Render::Texture*> Textures;
		DArray<FVector> PositionLights;
		FVector ViewLocation;
		Transform transform;
		UUID* m_UUID;


		//Test
	public:

		int32 CountVertex;
		int32 CountIndeces;
	};
}