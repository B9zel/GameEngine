#pragma once
#include <Core/includes/Memory/SerializeArchive.h>

class World;
class Engine;
class Object;
class Asset;

namespace CoreEngine
{
	enum class EAssetType : uint16;

	class SaveManager
	{
	public:

		void SetWorld(World* NewWorld);
		World* GetWorld() const;

		virtual void SaveSceneSerializedData(const String& Path);

		virtual void SaveScene(const String& Path);
		virtual bool LoadSaveScene(const String& Path);

		virtual void SaveContentItems();

		EAssetType LoadFileAsset(const String& Path, SerializeAchive& OutData);

		bool SaveAsset(const String& Path, Asset* SavingAsset);

	protected:

		virtual void PreContentItemStartSerialized(const DArray<Object*>& SerializeAssets);
		virtual void StartConteneItemSerialized(const DArray<Object*>& SerializeAssets);

		virtual void PreStartSerialized();
		virtual void StartSerialized();

		virtual void PreStartDeserialized();
		virtual void StartDeserialized(SerializeAchive& LoadedData);

	public:

		static const String FileExtension;

	private:

		World* WorldPtr;
		SerializeAchive Achive;
	};
} // namespace CoreEngine
