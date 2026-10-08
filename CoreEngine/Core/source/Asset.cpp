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

	PreChangeProperty.Call(this, Property);
}

void Asset::PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property)
{
	Object::PostEditChangeProperty(Property);

	PostChangeProperty.Call(this, Property);
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
	Object::SetName(NewName);

#if WITH_EDITOR

	const String& Path = CoreEngine::FileManager::RenameFile(PathToAsset, NewName);
	if (!Path.empty())
	{
		PathToAsset = Path;
		Engine::Get()->GetWorld()->GetSaveManager()->SaveAsset(Path, this);
	}

#endif
}

void Asset::OnSerialize(CoreEngine::SerializeAchive& Archive)
{
	Object::OnSerialize(Archive);

	CoreEngine::Reflection::ClassField* Class = GetClass();
	DArray<CoreEngine::Reflection::PropertyField*> Fields = Class->GetWithParentPropertyFields();

	for (auto* Property : Fields)
	{
		if (Property->GetIsPointer())
		{
			auto object = Property->GetSourcePropertyByName<Object*>(this);
			if (object && *object)
			{
				if (auto* asset = dynamic_cast<Asset*>(*object))
				{
					// asset->GetPathToAsset();
				}
			}

			Archive.PushPrefix(Property->Name);
			Property->Serialize(Archive, this);
			Archive.PopPrefix();
		}
	}
}
