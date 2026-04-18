#include <Render/includes/MaterialAsset.h>

MaterialAsset::MaterialAsset(const CoreEngine::InitializeObject& Initilize) : Asset(Initilize)
{
}

CoreEngine::EAssetType MaterialAsset::GetAssetType() const
{
	return CoreEngine::EAssetType::Material;
}

EShaderRenderType MaterialAsset::GetModeRender() const
{
	return m_ModeRender;
}

ShaderAsset* MaterialAsset::GetShaderAsset() const
{
	return m_ShaderAsset;
}