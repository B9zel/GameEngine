#include <Core/includes/World.h>
#include <Core/includes/UpdateManager.h>
#include <Core/includes/Level.h>
#include <Render/includes/Scene/SceneInterface.h>
#include <Render/includes/Scene/Scene.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Runtime/includes/PlayerController.h>
#include <Core/includes/Memory/SaveManager.h>
#include <Core/includes/Engine.h>
#include <GLFW/glfw3.h>

World::World(const CoreEngine::InitializeObject& Initilize) : Object(Initilize)
{
	m_UpdateManager = CoreEngine::UpdateManager::CreateInstance();
	m_Scene = MakeUniquePtr<CoreEngine::Render::Scene>();
	m_SaveManager = MakeUniquePtr<CoreEngine::SaveManager>();
	m_SaveManager->SetWorld(this);

	m_LastTime = static_cast<float>(glfwGetTime());
}

World::~World() = default;

void World::InitProperties()
{
	m_Scene->SetWorld(this);
}

void World::WorldUpdate()
{
	float now = static_cast<float>(glfwGetTime());
	m_DeltaTime = now - m_LastTime;
	m_LastTime = now;

	UpdateWorld();

	m_Scene->CollectProxy();
	m_Scene->StartRender();
}

void World::UpdateWorld()
{
	m_UpdateManager->ExecuteGroup(m_DeltaTime, CoreEngine::EStageUpdate::PRE_UPDATE);
	m_UpdateManager->ExecuteGroup(m_DeltaTime, CoreEngine::EStageUpdate::UPDATE);
	m_UpdateManager->ExecuteGroup(m_DeltaTime, CoreEngine::EStageUpdate::POST_UPDATE);
}

CoreEngine::UpdateManager* World::GetUpdateManager() const
{
	return m_UpdateManager.get();
}

CoreEngine::SaveManager* World::GetSaveManager() const
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
	if (!m_MainLevel) return FVector::ZeroVector;

	for (auto* actor : m_MainLevel->GetActors())
	{
		if (dynamic_cast<PlayerController*>(actor))
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

void World::OnSerialize(CoreEngine::SerializeAchive& Achive)
{
	Object::OnSerialize(Achive);

	m_MainLevel->Serialize(Achive);
}

void World::PreDeserialize()
{
	Object::PreDeserialize();

	m_MainLevel->PreDeserialize();
}

void World::OnDeserialize(CoreEngine::SerializeAchive& Data)
{
	Object::OnDeserialize(Data);

	m_MainLevel->Deserialize(Data);
}

void World::OpenLevel(Level* level)
{
	if (!level)
	{
		EG_LOG(CoreEngine::CORE, ELevelLog::ERROR, "Can't open null level");
		return;
	}

	if (m_MainLevel == level) return;

	if (m_MainLevel)
	{
		Level* previousLevel = m_MainLevel;
		Engine::Get()->GetMemoryManager()->GetGarbageCollector()->RemoveRootObject(previousLevel);

		const auto levelIt = std::find(m_Levels.begin(), m_Levels.end(), previousLevel);
		if (levelIt != m_Levels.end())
		{
			m_Levels.erase(levelIt);
		}

		previousLevel->MarkGarbage();
	}

	level->SetWorld(this);
	level->InitProperties();
	m_Levels.push_back(level);
	m_MainLevel = level;

	m_MainLevel->ActorInitialize();
	Engine::Get()->GetMemoryManager()->GetGarbageCollector()->AddRootObject(m_MainLevel);
}

void World::InitializePlayActors()
{
	for (size_t i = 0; i < m_Levels.size(); i++)
	{
		m_Levels[i]->ActorInitialize();
	}
}

void World::DestroyActor(Actor* ActorDestr)
{
	auto FindedElement = std::find(m_MainLevel->m_Actors.begin(), m_MainLevel->m_Actors.end(), ActorDestr);
	if (FindedElement != m_MainLevel->m_Actors.end())
	{
		m_MainLevel->m_Actors.erase(FindedElement);
	}
}
