#include <Core/includes/PrimitiveProxy.h>

namespace CoreEngine
{
	const HashTableMap<Render::Shader*, Pair<Render::VertexArrayObject*, Render::ElementBufferObject*>>& PrimitiveProxy::GetShaders() const
	{
		return Shaders;
	}

	void PrimitiveProxy::AddShaderWithArrayObject(Render::Shader* shaderKey, Render::VertexArrayObject* arrayValue, Render::ElementBufferObject* elementValue)
	{
		if (shaderKey && arrayValue)
		{
			Shaders.insert(Pair<Render::Shader*, Pair<Render::VertexArrayObject*, Render::ElementBufferObject*>>(shaderKey, Pair<Render::VertexArrayObject*, Render::ElementBufferObject*>(arrayValue, elementValue)));
		}
	}

	void PrimitiveProxy::AddTexture(Render::Texture* NewTexture)
	{
		Textures.push_back(NewTexture);
	}

	const DArray<Render::Texture*>& PrimitiveProxy::GetTextures() const
	{
		return Textures;
	}

	const Transform& PrimitiveProxy::GetTransform() const
	{
		return transform;
	}

	void PrimitiveProxy::SetViewLocation(const FVector& Location)
	{
		ViewLocation = Location;
	}

	void PrimitiveProxy::AddLightLocation(const FVector& Location)
	{
		PositionLights.push_back(Location);
	}

	const DArray<FVector>& PrimitiveProxy::GetLocationLights() const
	{
		return PositionLights;
	}

	const FVector& PrimitiveProxy::GetViewLocation() const
	{
		return ViewLocation;
	}

	void PrimitiveProxy::SetTransform(const Transform& Transform)
	{
		transform = Transform;
	}

}
