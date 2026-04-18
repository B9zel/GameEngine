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

	auto* shader = CoreEngine::Render::ShaderCache::GetShaderFromMaterial(*ParentShaderAsset);
	if (!shader)
	{
		EG_LOG(RenderHandleLog, ELevelLog::ERROR, "Can't find shader for material");
		return;
	}
	for (auto& uniform : shader->GetAllUniforms())
	{
		auto NewProperty = CreatePropertyFromType(uniform.second.Type);
		if (NewProperty)
		{
			NewProperty->Name = uniform.first;
			m_MaterialProperties.emplace_back(std::move(NewProperty));
		}
	}

	m_ParentMaterial = ParentMaterial;
}

const CoreEngine::Render::MaterialHandle& MaterialInstance::GetParentMaterial() const
{
	return m_ParentMaterial;
}

UniquePtr<CoreEngine::Render::BaseMaterialProperty> MaterialInstance::CreatePropertyFromType(const EUniformType& Type)
{
	switch (Type)
	{
	case EUniformType::INT:
		return MakeUniquePtr<CoreEngine::Render::MaterialPropertyInt>();
	case EUniformType::UINT:
		return MakeUniquePtr<CoreEngine::Render::MaterialPropertyUInt>();
	case EUniformType::FLOAT:
		return MakeUniquePtr<CoreEngine::Render::MaterialPropertyFloat>();
	case EUniformType::VEC3:
		return MakeUniquePtr<CoreEngine::Render::MaterialPropertyVec3>();
	case EUniformType::MAT4:
		return MakeUniquePtr<CoreEngine::Render::MaterialPropertyMat4>();
	default:
		ASSERT("Don't support this type of uniform");
		return nullptr;
	}
}
