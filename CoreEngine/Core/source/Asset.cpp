#include <Core/includes/Asset.h>
#include <Render/includes/MaterialAsset.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/Engine.h>
#include <Core/includes/World.h>
#include <Core/includes/FileManager.h>
#include <Core/includes/Memory/SaveManager.h>

#if !WITH_EDITOR

Asset* CoreEngine::AssetFactory::CreateAsset(const EAssetType& Type)
{
	switch (Type)
	{
	case EAssetType::Material:
		return CreateObject<Asset>(MaterialAsset::GetStaticClass());
	case EAssetType::Shader:
		return CreateObject<Asset>(ShaderAsset::GetStaticClass());
	default:
		ASSERT("Asset type is not implemant");
		return nullptr;
	}

	return nullptr;
}
#endif

Asset::Asset(const CoreEngine::InitializeObject& Initilize) : Object(Initilize)
{
}

void Asset::PreEditChangeProperty(CoreEngine::Reflection::PropertyField& Property)
{
	Object::PreEditChangeProperty(Property);

	PreChangeProperty.Call(Property);
}

void Asset::PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property)
{
	Object::PostEditChangeProperty(Property);

	PostChangeProperty.Call(Property);
}

CoreEngine::EAssetType Asset::GetAssetType() const
{
	return CoreEngine::EAssetType::None;
}

const String& Asset::GetPathToAsset() const
{
	return PathToAsset;
}

void Asset::SetPathToAsset(const String& NewPath)
{
	PathToAsset = NewPath;
}

void Asset::SetName(const String& NewName)
{
#if WITH_EDITOR

	const String& Path = CoreEngine::FileManager::RenameFile(PathToAsset, NewName);
	if (!Path.empty())
	{
		Engine::Get()->GetWorld()->GetSaveManager()->SaveAsset(Path, this);
		PathToAsset = Path;
	}

#endif

	Object::SetName(NewName);
}
