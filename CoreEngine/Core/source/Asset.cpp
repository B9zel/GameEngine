#include <Core/includes/Asset.h>

Asset::Asset(const CoreEngine::InitializeObject& Initilize) : Object(Initilize)
{
}

CoreEngine::EAssetType Asset::GetAssetType() const
{
	return CoreEngine::EAssetType::None;
}

const String& Asset::GetPathToAsset() const
{
	return PathToAsset;
}
