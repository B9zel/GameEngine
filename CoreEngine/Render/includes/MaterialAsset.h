#pragma once
#include <Core/includes/Asset.h>
#include <Render/includes/Enums/ShaderRenderType.h>
#include <Render/includes/ShaderAsset.h>
#include <MaterialAsset.generated.h>

RCLASS()
class MaterialAsset : Asset
{
	GENERATED_BODY()

public:

	MaterialAsset(const CoreEngine::InitializeObject& Initilize);

public:

	virtual CoreEngine::EAssetType GetAssetType() const override;

	EShaderRenderType GetModeRender() const;
	ShaderAsset* GetShaderAsset() const;

private:

	RPROPERTY();
	ShaderAsset* m_ShaderAsset = nullptr;

	RPROPERTY();
	String PathToShaderAsset;

	EShaderRenderType m_ModeRender = EShaderRenderType::LIT;
};
