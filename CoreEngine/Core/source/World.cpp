#include <Core/includes/World.h>
#include <Core/includes/UpdateManager.h>
#include <Core/includes/Level.h>
#include <Render/includes/Scene/SceneInterface.h>
#include <Render/includes/Scene/Scene.h>
#include <GLFW/glfw3.h>


namespace CoreEngine
{
	World::World()
	{
		m_UpdateManager = UpdateManager::CreateInstance();
		m_Scene = Allocator::Allocate<Render::Scene>();
	}

	void World::InitProperties()
	{
		m_Scene->SetWorld(this);
		m_MainLevel->SetWorld(this);
		m_MainLevel->ActorInitizize();
		Engine::Get()->GetMamoryManager()->GetGarbageCollector()->AddRootObject(m_MainLevel);
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

	UpdateManager* World::GetUpdateManager()
	{
		return m_UpdateManager.get();
	}

	float World::GetWorldDeltaTime() const
	{
		return m_DeltaTime;
	}

	const DArray<Level*>& World::GetLevels() const
	{
		return m_Levels;
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
			m_Levels[i]->ActorInitizize();
		}

	}

}