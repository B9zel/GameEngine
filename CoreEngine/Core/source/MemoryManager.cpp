#include <Core/includes/MemoryManager.h>
#include <Core/includes/Memory/GarbageCollector.h>

namespace CoreEngine
{
	MemoryManager* MemoryManager::m_MemoryInstance = nullptr;

	GB::GarbageCollector* MemoryManager::m_collector = nullptr;

	Object* MemoryManager::AllocateMemory(const uint64 Byte)
	{
		return static_cast<Object*>(Allocator::Allocate(Byte));
	}

	MemoryManager::MemoryManager()
	{
	}

	UniquePtr<MemoryManager> MemoryManager::Create()
	{
		if (m_MemoryInstance)
		{
			EG_LOG(CORE, ELevelLog::ERROR, "Memory manager already exists");
			return nullptr;
		}

		auto newManager = UniquePtr<MemoryManager>(new MemoryManager());
		newManager->m_collector = GB::GarbageCollector::Create();
		m_MemoryInstance = newManager.get();

		return newManager;
	}

	MemoryManager::~MemoryManager()
	{
		if (m_collector)
		{
			m_collector->Shutdown();
			Allocator::DestroyAndDeallocate(m_collector);
			m_collector = nullptr;
		}

		m_MemoryInstance = nullptr;
	}

} // namespace CoreEngine
