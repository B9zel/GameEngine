#include <Core/includes/Engine.h>


#include <Core/includes/Memory/Allocator.h>
#include <Core/includes/Base.h>
#include <Core/includes/Log.h>
#include <Core/includes/World.h>

#include <Core/includes/MemoryManager.h>
#include <Core/includes/UpdateManager.h>
#include <Core/includes/TimerManager.h>
#include <Core/includes/InputDevice.h>

#include <Render/includes/Render.h>



DECLARE_LOG_CATEGORY_EXTERN(LogEngine);

namespace CoreEngine
{
	Engine* Engine::GEngine = nullptr;

	Engine::Engine()
	{
		GEngine = this;

		m_TimerManager = MakeUniquePtr<TimerManager>();
		m_Input			= MakeUniquePtr<InputDevice>();
		m_Render		= Render::Render::Create();
		m_MemoryManager = MemoryManager::Create();

		m_World.reset(Allocator::Allocate<World>());
		
	}


	UniquePtr<Engine> Engine::Create()
	{
		if (GEngine)
		{
			EG_LOG(LogEngine, ELevelLog::ERROR, "Engine already exists");
			return UniquePtr<Engine>(GEngine);
		}

		GEngine = new Engine();
		CHECK(GEngine);
		
		return UniquePtr<Engine>(GEngine);
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

}