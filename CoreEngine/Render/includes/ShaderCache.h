#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/RenderHardwareInterface.h>
#include <Render/includes/ShaderVariantKey.h>

class RMaterial;

namespace CoreEngine::Render
{
	// struct ShaderVariantKey;
	class Shader;
	



	class ShaderCache
	{
	public:

		static bool TryGetShaderHandle(const ShaderVariantKey& Key, RHI::ShaderHandle& OutHandle);
		static Shader* GetShaderFromMaterial(const RMaterial& material);
		static void AddShader(const ShaderVariantKey& Key, Shader* shader);

	private:

		static HashTableMap<ShaderVariantKey, Shader*, ShaderVariantKeyHasher> m_Cache;
	};
} // namespace CoreEngine::Render
