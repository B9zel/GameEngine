#include <Render/includes/MaterialInstance.h>
#include <Render/includes/MaterialManager.h>
#include <Render/includes/Material.h>
#include <Core/includes/AssetManager.h>
#include <Render/includes/ShaderCache.h>
#include <Render/includes/Shader.h>

DECLARE_LOG_CATEGORY_EXTERN(RenderHandleLog);

MaterialInstance::MaterialInstance(const CoreEngine::InitializeObject& Object) : MaterialInterface(Object)
{
}

void MaterialInstance::SetParentMaterial(const CoreEngine::Render::MaterialHandle& ParentMaterial)
{
	if (!ParentMaterial.IsValid()) return;

	const RMaterial* ParentShaderAsset = CoreEngine::Render::MaterialManager::Get().GetMaterial(ParentMaterial);
	m_ParentMaterial = ParentMaterial;

	auto* shader = CoreEngine::Render::ShaderCache::GetShaderFromMaterial(*ParentShaderAsset);
	if (!shader)
	{
		EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't find shader for material");
		return;
	}
	for (auto& uniform : shader->GetAllUniforms())
	{
		auto NewProperty = CoreEngine::Render::CreatePropertyFromType(uniform.second.Type);
		if (NewProperty)
		{
			NewProperty->Name = uniform.first;
			m_MaterialProperties.emplace_back(std::move(NewProperty));
		}
	}
}

const CoreEngine::Render::MaterialHandle& MaterialInstance::GetParentMaterial() const
{
	return m_ParentMaterial;
}

const CoreEngine::Render::MaterialHandle& MaterialInstance::GetPotentialParentMaterial() const
{
	return m_PotentialParentMaterial;
}
