#pragma once
#include <Core/includes/Asset.h>
#include <MeshAsset.generated.h>

RCLASS()
class MeshAsset : public Asset
{
	GENERATED_BODY()

public:

	MeshAsset(const CoreEngine::InitializeObject& Initilize);

public:

	virtual CoreEngine::EAssetType GetAssetType() const override;

	String GetPathToModel() const;
	void SetPathToModel(const String& NewPath);

private:

	RPROPERTY();
	String PathToModel;
};
