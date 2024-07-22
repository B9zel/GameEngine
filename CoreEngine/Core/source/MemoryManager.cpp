#include <Core/includes/MemoryManager.h>



namespace CoreEngine
{
    MemoryManager* MemoryManager::m_MemoryInstance = nullptr;

    GB::GarbageCollector* MemoryManager::m_collector = nullptr;


    MemoryManager* MemoryManager::Create()
    {
        if (m_MemoryInstance)
        {
            EG_LOG(CORE, ELevelLog::ERROR, "Memory manager already exists");
            return (m_MemoryInstance);
        }

        m_MemoryInstance = static_cast<MemoryManager*>(Allocator::Allocate(sizeof(MemoryManager)));
        new(m_MemoryInstance) MemoryManager();

        m_collector = GB::GarbageCollector::Create();
       
        return (m_MemoryInstance);
    }

    MemoryManager::~MemoryManager()
    {
        Allocator::Deallocate<GB::GarbageCollector>(m_collector);
    }

}