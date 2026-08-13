#include <Core/includes/Memory/SaveManager.h>
#include <Core/includes/World.h>
#include <Core/includes/Engine.h>
#include <Core/includes/Asset.h>
#include <fstream>

namespace CoreEngine
{
	const String SaveManager::FileExtension = ".reflect";

	void SaveManager::SetWorld(World* NewWorld)
	{
		WorldPtr = NewWorld;
	}

	World* SaveManager::GetWorld() const
	{
		return WorldPtr;
	}
	void SaveManager::PreStartSerialized()
	{
		Achive.ClearData();
		WorldPtr->PreSerialize();
	}
	void SaveManager::StartSerialized()
	{
		WorldPtr->Serialize(Achive);
	}
	void SaveManager::SaveSceneSerializedData(const String& Path)
	{
		std::ofstream fout(Path);
		fout << Achive.Data().dump(4);

		fout.close();
	}

	void SaveManager::PreStartDeserialized()
	{
		WorldPtr->PreDeserialize();
	}

	void SaveManager::StartDeserialized(SerializeAchive& LoadedData)
	{
		WorldPtr->Deserialize(LoadedData);
	}

	void SaveManager::SaveScene(const String& Path)
	{
		PreStartSerialized();
		StartSerialized();
		SaveSceneSerializedData(Path);
	}

	bool SaveManager::LoadSaveScene(const String& Path)
	{
		std::ifstream File(Path);
		if (!File.is_open()) return false;

		SerializeAchive LoadedAchive;
		try
		{
			File >> LoadedAchive;
		}
		catch (const nlohmann::json::exception& Error)
		{
			EG_LOG(CORE, ELevelLog::ERROR, "Can't parse scene file '{0}': {1}", Path, Error.what());
			return false;
		}

		auto& Root = LoadedAchive.Data();
		if (!Root.is_object() || Root.count("Type") != 0)
		{
			EG_LOG(CORE, ELevelLog::ERROR, "File '{0}' is an asset, not a scene", Path);
			return false;
		}

		const bool HasWorld = std::any_of(Root.begin(), Root.end(), [](const nlohmann::json& Node)
		{
			return Node.is_object() && Node.count("m_MainLevel") != 0;
		});
		if (!HasWorld)
		{
			EG_LOG(CORE, ELevelLog::ERROR, "File '{0}' doesn't contain a serialized world", Path);
			return false;
		}

		PreStartDeserialized();
		StartDeserialized(LoadedAchive);

		return true;
	}

	void SaveManager::SaveContentItems()
	{
		static DArray<Object*> SerializeAssets;
		SerializeAssets.clear();

		auto& Objects = MemoryManager::GetInstance()->GetGarbageCollector()->GetAllObjects();
		for (auto& Obj : Objects)
		{
			if (Obj->GetClass()->IsChildClassOf(Asset::GetStaticClass()))
			{
				SerializeAssets.emplace_back(Obj);
			}
		}

		PreContentItemStartSerialized(SerializeAssets);
		StartConteneItemSerialized(SerializeAssets);
	}

	EAssetType SaveManager::LoadFileAsset(const String& Path, SerializeAchive& OutData)
	{
		if (Path.rfind(FileExtension) == Path.npos) return EAssetType::None;

		std::ifstream File(Path);
		if (!File.is_open()) return EAssetType::None;

		File >> OutData;

		bool IsSuccess = false;
		auto TypeAsset = OutData.DeserializeData<CoreEngine::EAssetType>("Type", IsSuccess);

		return TypeAsset;
	}

	bool SaveManager::SaveAsset(const String& Path, Asset* SavingAsset)
	{
		CoreEngine::SerializeAchive LocalAchive;

		LocalAchive.SerializeData("Type", SavingAsset->GetAssetType());

		SavingAsset->Serialize(LocalAchive);
		std::ofstream fout(Path);
		fout << LocalAchive.Data().dump(4);
		fout.close();

		return true;
	}

	void SaveManager::PreContentItemStartSerialized(const DArray<Object*>& SerializeAssets)
	{
		Achive.ClearData();
		for (auto* Asset : SerializeAssets)
		{
			Asset->PreSerialize();
		}
	}

	void SaveManager::StartConteneItemSerialized(const DArray<Object*>& SerializeAssets)
	{
		for (auto* asset : SerializeAssets)
		{
			Achive.ClearData();

			Asset* ToAsset = static_cast<Asset*>(asset);
			SaveAsset(ToAsset->GetPathToAsset(), ToAsset);
		}
	}

} // namespace CoreEngine
