#pragma once
#include <Render/includes/ShaderUtils.h>
#include <Core/includes/Asset.h>
#include <ShaderAsset.generated.h>

RCLASS()
class ShaderAsset : public Asset
{
	GENERATED_BODY()

public:

	ShaderAsset(const CoreEngine::InitializeObject& Initilize);

public:

	uint64 GetHash() const;

	virtual void OnSerialize(CoreEngine::SerializeAchive& Achive) override;
	virtual void OnDeserialize(CoreEngine::SerializeAchive& Achive) override;

	virtual CoreEngine::EAssetType GetAssetType() const override;
	const SourceShader& GetCustomShader() const;
	void SetCustomShader(const SourceShader& NewShader);

public:

	DispatcherVoid ApplyNewShader;

protected:

	SourceShader CustomShader;
};
