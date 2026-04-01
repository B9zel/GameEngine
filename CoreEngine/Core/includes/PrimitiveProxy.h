#pragma once
#include <Core/includes/Base.h>

#include <Render/includes/RenderHardwareInterface.h>

#include <Math/includes/Transform.h>

namespace CoreEngine
{
	struct ParamOfShaderDesc;
}

struct ShaderDescHasher
{
	size_t operator()(const CoreEngine::ParamOfShaderDesc& Desc) const;
};

namespace CoreEngine
{
	class UUID;

	struct ParamOfShaderDesc
	{
	public:

		bool operator==(const ParamOfShaderDesc& Other) const
		{
			return shader == Other.shader && ArrayObject == Other.ArrayObject && TextureNames == Other.TextureNames && HasAllMatrix == Other.HasAllMatrix;
		}

	public:

		Render::RHI::ShaderHandle shader;
		Render::RHI::HandleVAO ArrayObject;
		// Render::RHI::BufferHandle ElementObject;
		bool HasAllMatrix{false};

		DArray<String> TextureNames;
	};

	class PrimitiveProxy
	{
	public:

		PrimitiveProxy() = default;

	public:

		virtual const HashTableSet<ParamOfShaderDesc, ShaderDescHasher>& GetShaders() const;
		virtual void AddShader(const ParamOfShaderDesc& Desc);
		// virtual void AddTexture(Render::Texture* NewTexture);
		virtual void AddTexture(const Render::RHI::TextureHandle& NewTexture);
		virtual void SetTransformMatrix(const FMatrix4x4& transform);
		// const DArray<Render::Texture*>& GetTextures() const;
		const DArray<Render::RHI::TextureHandle>& GetTextures() const;
		const FMatrix4x4& GetTransformMatrix() const;
		void SetViewLocation(const FVector& Location);
		void AddLightLocation(const FVector& Location);
		const DArray<FVector>& GetLocationLights() const;
		const FVector& GetViewLocation() const;
		void SetUUID(const UUID* uuid);
		const UUID* GetUUID() const;

	protected:

		// HashTableMap<Render::Shader*, Pair<Render::VertexArrayObject*, Render::ElementBufferObject*>> Shaders;
		// HashTableMap<Render::RHI::ShaderHandle, Pair<Render::RHI::HandleVAO, Render::RHI::BufferHandle>> Shaders;
		HashTableSet<ParamOfShaderDesc, ShaderDescHasher> Shaders;
		DArray<Render::RHI::TextureHandle> Textures;
		DArray<FVector> PositionLights;
		FVector ViewLocation;
		FMatrix4x4 transform;
		UUID* m_UUID;

		// Test
	public:

		int32 CountVertex;
		int32 CountIndeces;
	};
} // namespace CoreEngine
