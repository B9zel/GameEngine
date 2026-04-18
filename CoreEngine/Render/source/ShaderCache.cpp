#include <Render/includes/ShaderCache.h>
#include <Render/includes/Shader.h>
#include <Render/includes/Material.h>

namespace CoreEngine::Render
{
	DECLARE_LOG_CATEGORY_EXTERN(RenderHandleLog);

	HashTableMap<ShaderVariantKey, Shader*, ShaderVariantKeyHasher> ShaderCache::m_Cache;

	bool ShaderCache::TryGetShaderHandle(const ShaderVariantKey& Key, RHI::ShaderHandle& OutHandle)
	{
		auto& Shader = m_Cache.find(Key);
		if (Shader == m_Cache.end())
		{
			return false;
		}

		OutHandle = Shader->second->GetHandle();
		return true;
	}

	Shader* ShaderCache::GetShaderFromMaterial(const RMaterial& material)
	{
		ShaderVariantKey Key;
		Key.m_Hash = material.GetShaderAsset()->GetHash();
		Key.ModeRender = material.GetModeRender();

		auto& Shader = m_Cache.find(Key);
		if (Shader != m_Cache.end())
		{
			return Shader->second;
		}

		return nullptr;
	}

	void ShaderCache::AddShader(const ShaderVariantKey& Key, Shader* shader)
	{
		if (m_Cache.count(Key))
		{
			EG_LOG(RenderHandleLog, ELevelLog::WARNING, "Shader with this key already exist in cache");
			return;
		}
		if (!shader)
		{
			EG_LOG(RenderHandleLog, ELevelLog::WARNING, "Invalid shader pointer");
			return;
		}

		m_Cache.emplace(Key, shader);
	}
} // namespace CoreEngine::Render
