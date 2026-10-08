#pragma once
#include <Core/includes/Asset.h>
#include <Render/includes/Enums/ShaderRenderType.h>
#include <Render/includes/ShaderAsset.h>
#include <MaterialAsset.generated.h>

namespace CoreEngine::Render
{
	struct BaseMaterialProperty;
}

RCLASS()
class MaterialAsset : public Asset
{
	GENERATED_BODY()

public:

	MaterialAsset(const CoreEngine::InitializeObject& Initilize);

public:

	virtual CoreEngine::EAssetType GetAssetType() const override;

	EShaderRenderType GetModeRender() const;
	ShaderAsset* GetShaderAsset() const;
	void SetShaderAsset(ShaderAsset* Shader);

	const String GetPathToShaderAsset() const;
	DArray<SharedPtr<CoreEngine::Render::BaseMaterialProperty>>& GetShaderUniforms();

private:

	void PostChangePropertyPathToAsset(Asset* Asset, CoreEngine::Reflection::PropertyField& Property);
	void PreChangePropertyPathToAsset(Asset* Asset, CoreEngine::Reflection::PropertyField& Property);
	void SetNewShader(ShaderAsset* Shader);

	void UpdateShaderUniforms();

public:

	DispatcherVoid OnUpdateShaderUniform;

private:

	RPROPERTY(EditorVisible Transient);
	ShaderAsset* m_ShaderAsset = nullptr;

	RPROPERTY();
	String PathToShaderAsset;

	DArray<SharedPtr<CoreEngine::Render::BaseMaterialProperty>> m_ShaderUniforms;

	EShaderRenderType m_ModeRender = EShaderRenderType::LIT;
};
