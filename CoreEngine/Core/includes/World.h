#pragma once
#include <Core/includes/Base.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Runtime/includes/Actor.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/Level.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class Object;
	}
	namespace Render
	{
		class SceneInterface;
	}
}

namespace CoreEngine
{
	class TimerManager;
	class UpdateManager;
	class Level;

	struct SpawnParamConfiguration
	{
	public:
		Level* SpawnLevel = nullptr;
	};

	class World : public CoreEngine::Runtime::Object
	{
	public:
		World();
	public:
		virtual void InitProperties() override;
		virtual void WorldUpdate();
		UpdateManager* GetUpdateManager();
		float GetWorldDeltaTime() const;
		const DArray<Level*>& GetLevels() const;

		template<class T>
		T* SpawnActor(Runtime::Actor* Owner, const SpawnParamConfiguration& Param = SpawnParamConfiguration());

		void OpenLevel(Level* level);
		void InitializePlayActors();
	private:

		UniquePtr<UpdateManager> m_UpdateManager;

		DArray<Level*> m_Levels;
		Level* m_MainLevel;

		Render::SceneInterface* m_Scene;

		float m_DeltaTime;
		float m_LastTime;
	};

	template<class T>
	inline T* World::SpawnActor(Runtime::Actor* Owner, const SpawnParamConfiguration& Param )
	{
		if (!Runtime::IsParentClass<Runtime::Actor, T>())
		{
			EG_LOG(CORE, ELevelLog::WARNING, "Set class doesn't child of Actor");
			return nullptr;
		}

		Level* spawnToLevel = nullptr;
		if (Param.SpawnLevel)
		{
			spawnToLevel = Param.SpawnLevel;
		}
		else
		{
			spawnToLevel = m_Levels.front();
			if (!spawnToLevel)
			{
				EG_LOG(CORE, ELevelLog::ERROR, "There is no single level");
				return nullptr;
			}
		}
		T* NewActor = Runtime::CreateObject<T>(Owner);
		NewActor->SetOwner(Owner);
		NewActor->PostSpawnActor();
		spawnToLevel->AddActor(NewActor);
		
		return NewActor;
	}
}