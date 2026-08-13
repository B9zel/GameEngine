#pragma once
#include <Core/includes/Base.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Runtime/includes/Actor.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/Level.h>
#include <Math/includes/Transform.h>
#include <World.generated.h>

namespace CoreEngine
{
	namespace Render
	{
		class SceneInterface;
	}
} // namespace CoreEngine

class Object;
class Level;

namespace CoreEngine
{
	class TimerManager;
	class UpdateManager;
	class SaveManager;

	struct SpawnParamConfiguration
	{
	public:

		Level* SpawnLevel = nullptr;
	};

	/*template<typename T>
	inline T* World::SpawnActor(Runtime::Actor* Owner, const SpawnParamConfiguration& Param)*/
} // namespace CoreEngine

RCLASS()
class World : public Object
{
	GENERATED_BODY()

public:

	World(const CoreEngine::InitializeObject& Initilize);
	virtual ~World();

public:

	virtual void InitProperties() override;
	virtual void WorldUpdate();

	CoreEngine::UpdateManager* GetUpdateManager() const;
	CoreEngine::SaveManager* GetSaveManager() const;
	float GetWorldDeltaTime() const;
	const DArray<Level*>& GetLevels() const;
	virtual FVector GetControllerLocation() const;

	template <class T>
	T* SpawnActor(Actor* Owner, const FTransform& transform, const CoreEngine::SpawnParamConfiguration& Param = CoreEngine::SpawnParamConfiguration());
	template <class T>
	T* SpawnActor(CoreEngine::Reflection::ClassField* ClassSource, Actor* Owner, const FTransform& transform,
				  const CoreEngine::SpawnParamConfiguration& Param = CoreEngine::SpawnParamConfiguration());

	virtual void PreSerialize() override;
	virtual void OnSerialize(CoreEngine::SerializeAchive& Achive) override;

	virtual void PreDeserialize() override;
	virtual void OnDeserialize(CoreEngine::SerializeAchive& Data) override;

	void OpenLevel(Level* level);
	void InitializePlayActors();

	template <class Predict> DArray<Actor*> GetAllActorsPredicate(Predict Predication);
	template <class Predict> Actor* GetActorPredicate(Predict Predication);

	virtual void DestroyActor(Actor* ActorDestr);

protected:

	virtual void UpdateWorld();

private:

	UniquePtr<CoreEngine::UpdateManager> m_UpdateManager;
	UniquePtr<CoreEngine::SaveManager> m_SaveManager;

	DArray<Level*> m_Levels;
	RPROPERTY();
	Level* m_MainLevel{ nullptr };

	UniquePtr<CoreEngine::Render::SceneInterface> m_Scene;

	float m_DeltaTime{0.0f};
	float m_LastTime{0.0f};
};

template <class T> T* World::SpawnActor(Actor* Owner, const FTransform& transform, const CoreEngine::SpawnParamConfiguration& Param)
{
	if (!IsParentClass<Actor, T>())
	{
		EG_LOG(CoreEngine::CORE, ELevelLog::WARNING, "Set class doesn't child of Actor");
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
			EG_LOG(CoreEngine::CORE, ELevelLog::ERROR, "There is no single level");
			return nullptr;
		}
	}

	T* NewActor = CreateObject<T>(Owner);
	NewActor->SetOwner(Owner);
	NewActor->PostSpawnActor();
	spawnToLevel->AddActor(NewActor);
	NewActor->SetActorTransform(transform);

	return NewActor;
}

template <class T>
inline T* World::SpawnActor(CoreEngine::Reflection::ClassField* ClassSource, Actor* Owner, const FTransform& transform,
							const CoreEngine::SpawnParamConfiguration& Param)
{
	CoreEngine::Reflection::ClassField* ClassOfTargetType = T::GetStaticClass();

	if (!ClassSource->IsChildClassOf(ClassOfTargetType))
	{
		EG_LOG(CoreEngine::CORE, ELevelLog::WARNING, "Set class doesn't child of Actor");
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
			EG_LOG(CoreEngine::CORE, ELevelLog::ERROR, "There is no single level");
			return nullptr;
		}
	}
	T* NewActor = CreateObject<T>(ClassSource, Owner);
	NewActor->SetOwner(Owner);
	NewActor->PostSpawnActor();
	spawnToLevel->AddActor(NewActor);
	NewActor->SetActorTransform(transform);

	return NewActor;
}

template <class Predict> DArray<Actor*> World::GetAllActorsPredicate(Predict Predication)
{
	DArray<Actor*> CollectActors;
	for (Actor* actor : m_MainLevel->GetActors())
	{
		if (Predication(actor))
		{
			CollectActors.push_back(actor);
		}
	}
	return CollectActors;
}

template <class Predict> Actor* World::GetActorPredicate(Predict Predication)
{
	for (Actor* actor : m_MainLevel->GetActors())
	{
		if (Predication(actor))
		{
			return actor;
		}
	}
	return nullptr;
}
