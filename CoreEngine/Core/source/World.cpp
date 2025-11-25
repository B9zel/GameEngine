#include <Core/includes/World.h>
#include <Core/includes/UpdateManager.h>
#include <Core/includes/Level.h>
#include <Render/includes/Scene/SceneInterface.h>
#include <Render/includes/Scene/Scene.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Runtime/includes/PlayerController.h>
#include <Core/includes/Memory/SaveManager.h>
#include <GLFW/glfw3.h>


namespace CoreEngine
{
	World::World(const InitializeObject& Initilize) : Object(Initilize)
	{
		m_UpdateManager = UpdateManager::CreateInstance();
		m_Scene = Allocator::Allocate<Render::Scene>();
		m_SaveManager = MakeUniquePtr<SaveManager>();
		m_SaveManager->SetWorld(this);

		m_LastTime = static_cast<float>(glfwGetTime());
	}

	void World::InitProperties()
	{
		m_Scene->SetWorld(this);
		m_MainLevel->SetWorld(this);
		m_MainLevel->ActorInitialize();
		Engine::Get()->GetMemoryManager()->GetGarbageCollector()->AddRootObject(m_MainLevel);
	}

	void World::WorldUpdate()
	{
		float now = static_cast<float>(glfwGetTime());
		m_DeltaTime = now - m_LastTime;
		m_LastTime = now;

		m_UpdateManager->ExecuteGroup(m_DeltaTime, EStageUpdate::PRE_UPDATE);
		m_UpdateManager->ExecuteGroup(m_DeltaTime, EStageUpdate::UPDATE);
		m_UpdateManager->ExecuteGroup(m_DeltaTime, EStageUpdate::POST_UPDATE);

		m_Scene->CollectProxy();
		m_Scene->StartRender();
	}

	UpdateManager* World::GetUpdateManager() const
	{
		return m_UpdateManager.get();
	}

	SaveManager* World::GetSaveManager() const
	{
		return m_SaveManager.get();
	}

	float World::GetWorldDeltaTime() const
	{
		return m_DeltaTime;
	}

	const DArray<Level*>& World::GetLevels() const
	{
		return m_Levels;
	}

	FVector World::GetControllerLocation() const
	{
		for (auto* actor : m_MainLevel->GetActors())
		{
			if (dynamic_cast<Runtime::PlayerController*>(actor))
			{
				return actor->GetActorLocation();
			}
		}
		return FVector(0);
	}

	void World::PreSerialize()
	{
		Object::PreSerialize();

		m_MainLevel->PreSerialize();
	}

	void World::Serialize(SerializeAchive& Achive)
	{
		Object::Serialize(Achive);

		m_MainLevel->Serialize(Achive);
	}

	void World::OpenLevel(Level* level)
	{
		if (!m_MainLevel || !m_Levels.empty())
		{
			m_MainLevel->StartDestroy();
			m_MainLevel->FinishDestroy();
			m_Levels.erase(std::find(m_Levels.begin(), m_Levels.end(), m_MainLevel));
			delete m_MainLevel;
		}
		level->InitProperties();
		m_Levels.push_back(level);
		m_MainLevel = level;
	}

	void World::InitializePlayActors()
	{
		for (size_t i = 0; i < m_Levels.size(); i++)
		{
			m_Levels[i]->ActorInitialize();
		}
	}
}