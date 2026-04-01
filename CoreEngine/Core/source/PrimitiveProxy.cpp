#include <Core/includes/PrimitiveProxy.h>
#include <Render/includes/ElementBufferObject.h>
#include <Render/includes/Texture.h>
#include <Core/includes/UUID.h>
#include <Render/includes/Shader.h>
#include <Render/includes/VertexArrayObject.h>

DECLARE_LOG_CATEGORY_EXTERN(PrimitiveProxyLog)

namespace CoreEngine
{
	/*const HashTableMap<Render::Shader*, Pair<Render::VertexArrayObject*, Render::ElementBufferObject*>>& PrimitiveProxy::GetShaders() const
	{
		return Shaders;
	}*/

	const HashTableSet<ParamOfShaderDesc, ShaderDescHasher>& PrimitiveProxy::GetShaders() const
	{
		return Shaders;
	}

	void PrimitiveProxy::AddShader(const ParamOfShaderDesc& Desc)
	{
		if (!Desc.shader.IsValid())
		{
			EG_LOG(PrimitiveProxyLog, ELevelLog::WARNING, "elements of shader don't valid");
			return;
		}
		Shaders.insert(Desc);
	}

	// void PrimitiveProxy::AddTexture(Render::Texture* NewTexture)
	//{
	//	Textures.push_back(NewTexture);
	// }

	// const DArray<Render::Texture*>& PrimitiveProxy::GetTextures() const
	//{
	//	return Textures;
	// }

	const DArray<Render::RHI::TextureHandle>& PrimitiveProxy::GetTextures() const
	{
		return Textures;
	}

	const FMatrix4x4& PrimitiveProxy::GetTransformMatrix() const
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

	void PrimitiveProxy::AddTexture(const Render::RHI::TextureHandle& NewTexture)
	{
		Textures.push_back(NewTexture);
	}

	void PrimitiveProxy::SetTransformMatrix(const FMatrix4x4& Transform)
	{
		transform = Transform;
	}
	void PrimitiveProxy::SetUUID(const UUID* uuid)
	{
		m_UUID = const_cast<UUID*>(uuid);
	}

	const UUID* PrimitiveProxy::GetUUID() const
	{
		return m_UUID;
	}
} // namespace CoreEngine

size_t ShaderDescHasher::operator()(const CoreEngine::ParamOfShaderDesc& Desc) const
{
	size_t hash = 0;
	std::hash<uint64> uint64Hasher;
	std::hash<String> stringHasher;

	hash ^= uint64Hasher(Desc.shader.GetId()) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= uint64Hasher(Desc.ArrayObject.GetId()) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	// hash ^= uint64Hasher(Desc.ElementObject.GetId()) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	for (const auto& TextureName : Desc.TextureNames)
	{
		hash ^= stringHasher(TextureName) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	}
	return hash;
}
