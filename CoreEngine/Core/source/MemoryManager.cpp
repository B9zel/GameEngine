#include <Core/includes/MemoryManager.h>
#include <Core/includes/Memory/GarbageCollector.h>


namespace CoreEngine
{
    MemoryManager* MemoryManager::m_MemoryInstance = nullptr;

    GB::GarbageCollector* MemoryManager::m_collector = nullptr;


    UniquePtr<MemoryManager> MemoryManager::Create()
    {
        if (m_MemoryInstance)
        {
            EG_LOG(CORE, ELevelLog::WARNING, "Memory manager already exists");
            return UniquePtr<MemoryManager>(m_MemoryInstance);
        }

        m_MemoryInstance = new MemoryManager();
        m_MemoryInstance->m_collector = GB::GarbageCollector::Create();
       
        return UniquePtr<MemoryManager>(m_MemoryInstance);
    }

    MemoryManager::~MemoryManager()
    {
        Allocator::Deallocate<GB::GarbageCollector>(m_collector);
    }

}