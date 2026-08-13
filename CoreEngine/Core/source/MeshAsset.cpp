#include <Core/includes/MeshAsset.h>

MeshAsset::MeshAsset(const CoreEngine::InitializeObject& Initilize) : Asset(Initilize)
{
}

CoreEngine::EAssetType MeshAsset::GetAssetType() const
{
	return CoreEngine::EAssetType::Model;
}

String MeshAsset::GetPathToModel() const
{
	return PathToModel;
}

void MeshAsset::SetPathToModel(const String& NewPath)
{
	PathToModel = NewPath;
}
