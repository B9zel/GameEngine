#include <Core/includes/MemoryManager.h>

namespace CoreEngine
{
    MemoryManager* MemoryManager::m_MemeoryInstance = nullptr;

    MemoryManager* MemoryManager::Create()
    {
        CORE_ASSERT(m_MemeoryInstance != nullptr, "Memory manager already exists");

        m_MemeoryInstance = Allocator::Allocate<MemoryManager>();

        return m_MemeoryInstance;
    }

    MemoryManager::~MemoryManager()
    {
        Allocator::Deallocate<GarbageCollector>(m_collector);
    }

}