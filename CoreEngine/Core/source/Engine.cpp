#include <Core/includes/Engine.h>

#include <Core/includes/Memory/Allocator.h>
#include <Core/includes/Base.h>
#include <Core/includes/Log.h>
#include <Core/includes/World.h>

#include <Core/includes/MemoryManager.h>
#include <Core/includes/UpdateManager.h>
#include <Core/includes/TimerManager.h>
#include <Core/includes/InputDevice.h>

#include <Render/includes/RenderDevice.h>
#include <Render/includes/Render.h>
#include <Core/includes/Window.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <Events/include/Event.h>
#include <Core/includes/Memory/SaveManager.h>
#include <Core/includes/AssetManager.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/Application.h>

DECLARE_LOG_CATEGORY_EXTERN(LogEngine);

Engine* Engine::GEngine = nullptr;

Engine::Engine(const CoreEngine::InitializeObject& Initilize) : Object(Initilize)
{
	GEngine = this;

	m_TimerManager = MakeUniquePtr<CoreEngine::TimerManager>();
	m_MemoryManager = CoreEngine::MemoryManager::Create();
	m_MemoryManager->GetGarbageCollector()->AddRootObject(this);
	m_Input = MakeUniquePtr<CoreEngine::InputDevice>();
}

void Engine::Init()
{
	if (!m_AssetManager)
	{
		m_AssetManager = CreateObject<AssetManager>(this);
	}
	m_Render = CoreEngine::Render::Render::Create();
	m_Render->Construct();
}

FVector2 Engine::GetScreenSize() const
{
	CoreEngine::Window& win = CoreEngine::Application::Get()->GetWindow();
	return FVector2(win.GetWidth(), win.GetHeight());
}

void Engine::PostInitialize()
{
	m_World->InitProperties();
}

void Engine::ConstructInitialize()
{
	m_World = CreateWorld();
	m_MemoryManager->GetGarbageCollector()->AddRootObject(m_World);
}

void Engine::TakeInputEvent(CoreEngine::Event& Input)
{
	m_Input->InviteEvent(Input);
}

Engine* Engine::Create()
{
	if (GEngine)
	{
		EG_LOG(LogEngine, ELevelLog::ERROR, "Engine already exists");
		return GEngine;
	}

	GEngine = CreateObject<ThisClass>();
	CHECK(GEngine);

	return GEngine;
}

void Engine::Update()
{
	m_World->WorldUpdate();
	m_TimerManager->Update(m_World->GetWorldDeltaTime());
}

Engine* Engine::Get()
{
	return GEngine;
}

World* Engine::CreateWorld() const
{
	return CreateObject<World>();
}

AssetManager* Engine::GetAssetManager() const
{
	return m_AssetManager;
}

UniquePtr<CoreEngine::InputDevice>& Engine::GetInputDevice() const
{
	return m_Input;
}
UniquePtr<CoreEngine::TimerManager>& Engine::GetTimerManager() const
{
	return m_TimerManager;
}
UniquePtr<CoreEngine::MemoryManager>& Engine::GetMemoryManager() const
{
	return m_MemoryManager;
}
UniquePtr<CoreEngine::Render::Render>& Engine::GetRender() const
{
	return m_Render;
}
const UniquePtr<CoreEngine::Render::RenderDevice>& Engine::GetRenderDevice() const
{
	return GetRender()->GetRenderDevice();
}
World* Engine::GetWorld() const
{
	return m_World;
}
UniquePtr<CoreEngine::Reflection::ReflectionManager>& Engine::GetReflectionManger() const
{
	return CoreEngine::Application::Get()->GetReflectionManager();
}
