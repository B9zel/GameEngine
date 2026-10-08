#include <Render/includes/MaterialInstance.h>
#include <Render/includes/MaterialManager.h>
#include <Render/includes/Material.h>
#include <Core/includes/AssetManager.h>
#include <Render/includes/ShaderCache.h>
#include <Render/includes/Shader.h>
#include <Render/includes/ShaderGLSLUtils.h>

DECLARE_LOG_CATEGORY_EXTERN(RenderHandleLog);

MaterialInstance::MaterialInstance(const CoreEngine::InitializeObject& Object) : MaterialInterface(Object)
{
}

void MaterialInstance::SetParentMaterial(const CoreEngine::Render::MaterialHandle& ParentMaterial)
{
	if (!ParentMaterial.IsValid()) return;

	m_ParentMaterial = ParentMaterial;
	UpdateUniform(ParentMaterial);
}

void MaterialInstance::UpdateUniform(const CoreEngine::Render::MaterialHandle& Handle)
{
	if (!Handle.IsValid())
	{
		EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Invalid material handle");
		return;
	}

	const RMaterial* ParentShaderAsset = CoreEngine::Render::MaterialManager::Get().GetMaterial(Handle);
	auto* shader = CoreEngine::Render::ShaderCache::GetShaderFromMaterial(*ParentShaderAsset);
	if (!shader)
	{
		EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't find shader for material");
		return;
	}
	HashTableMap<String, CoreEngine::Render::GLSL::UniformFileInfo> UniformsCustomShader;

	CoreEngine::Render::GLSL::ParseShader(ParentShaderAsset->GetShaderAsset()->GetCustomShader().VertexShader +
											  ParentShaderAsset->GetShaderAsset()->GetCustomShader().FragmentShader,
										  UniformsCustomShader);
	for (auto& uniform : shader->GetAllUniforms())
	{
		if (UniformsCustomShader.find(uniform.first) == UniformsCustomShader.end()) continue;

		auto NewProperty = CoreEngine::Render::CreatePropertyFromType(uniform.second.Type);
		if (NewProperty)
		{
			NewProperty->Name = uniform.first;
			m_MaterialProperties.emplace_back(std::move(NewProperty.release()));
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

const DArray<SharedPtr<CoreEngine::Render::BaseMaterialProperty>>* MaterialInstance::GetMaterialProperties()
{
	return &m_MaterialProperties;
}
