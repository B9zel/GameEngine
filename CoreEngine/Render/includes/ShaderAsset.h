#pragma once
#include <Render/includes/ShaderUtils.h>
#include <Core/includes/Asset.h>
#include <ShaderAsset.generated.h>

RCLASS()
class ShaderAsset : public Asset
{
	GENERATED_BODY()

public:

	uint64 GetHash() const;

	virtual void OnSerialize(CoreEngine::SerializeAchive& Achive) override;
	virtual void OnDeserialize(CoreEngine::SerializeAchive& Achive) override;

	virtual CoreEngine::EAssetType GetAssetType() const override;

public:

	SourceShader CustomShader;
};
