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

	void World::WorldUpdate()
	{
		float now = static_cast<float>(glfwGetTime());
		m_DeltaTime = now - m_LastTime;
		m_LastTime = now;

		m_UpdateManager->ExecuteGroup(m_DeltaTime, EStageUpdate::PRE_UPDATE);
		m_UpdateManager->ExecuteGroup(m_DeltaTime, EStageUpdate::UPDATE);
		m_UpdateManager->ExecuteGroup(m_DeltaTime, EStageUpdate::POST_UPDATE);
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

}