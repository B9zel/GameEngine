#include <Editor/includes/Asset/EditorAsset.h>
#include <Core/includes/Asset.h>
#include <Render/includes/MaterialAsset.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Editor/includes/Asset/EditorMaterialAsset.h>
#include <Editor/includes/Asset/EditorShaderAsset.h>

Asset* CoreEngine::AssetFactory::CreateAsset(const EAssetType& Type)
{
	switch (Type)
	{
	case EAssetType::Material:
		return CreateObject<Asset>(EditorMaterialAsset::GetStaticClass());
	case EAssetType::Shader:
		return CreateObject<Asset>(EditorShaderAsset::GetStaticClass());
	default:
		ASSERT("Asset type is not implemant");
		return nullptr;
	}

	return nullptr;
}
