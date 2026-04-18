#pragma once
#include <Core/includes/Memory/SerializeArchive.h>

class World;
class Engine;

namespace CoreEngine
{

	class SaveManager
	{
	public:

		void SetWorld(World* NewWorld);
		World* GetWorld() const;

		virtual void PreStartSerialized();
		virtual void StartSerialized();
		virtual void SaveSceneSerializedData(const String& Path);

		virtual void PreStartDeserialized();
		virtual void StartDeserialized(SerializeAchive& LoadedData);

		virtual void SaveScene(const String& Path);
		virtual bool LoadSaveScene(const String& Path);

	private:

		World* WorldPtr;
		SerializeAchive Achive;
	};
} // namespace CoreEngine
