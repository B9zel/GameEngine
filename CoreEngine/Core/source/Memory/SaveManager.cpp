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
	void SaveManager::SaveSceneSerializedData()
	{
		std::ofstream fout("Scene.json");
		fout << Achive.Data().dump(4);
		
		fout.close();
	}

	void SaveManager::PrintSerializeData()
	{
		
		EG_LOG(CORE, ELevelLog::INFO, Achive.Data().dump(4));
	}

}