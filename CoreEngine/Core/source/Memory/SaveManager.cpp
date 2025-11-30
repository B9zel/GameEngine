#include <Core/includes/Memory/SaveManager.h>
#include <Core/includes/World.h>
#include <fstream>

namespace CoreEngine
{
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
		File >> LoadedAchive;
		EG_LOG(CORE, ELevelLog::INFO, LoadedAchive.Data().dump(4));
		for (auto& i : LoadedAchive.Data()["World"]["FirstLevel"])
		{

			EG_LOG(CORE, ELevelLog::INFO, i.dump(4));
		}
		
		PreStartDeserialized();
		StartDeserialized(LoadedAchive);
	}


}