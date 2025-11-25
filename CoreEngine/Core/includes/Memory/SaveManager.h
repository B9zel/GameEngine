#pragma once
#include <Core/includes/Memory/SerializeArchive.h>

namespace CoreEngine
{
	class World;

	class SaveManager
	{
	public:

		void SetWorld(World* NewWorld);
		World* GetWorld() const;

		virtual void PreStartSerialized();
		virtual void StartSerialized();
		virtual void SaveSceneSerializedData();

		void PrintSerializeData();

	private:

		World* WorldPtr;
		SerializeAchive Achive;
	};
}