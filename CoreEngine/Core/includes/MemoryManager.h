#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/Memory/Allocator.h>

/**
 *
 *	Class MemoryManager responsibilities for interacting with memory and storing the garbage collector
 *
 *
 */

class Object;
namespace CoreEngine
{

	namespace GB
	{
		class GarbageCollector;
	}

	class MemoryManager
	{
	public:

		virtual ~MemoryManager();

	public:

		static UniquePtr<MemoryManager> Create();
		static MemoryManager* GetInstance()
		{
			return m_MemoryInstance;
		}
		static GB::GarbageCollector* GetGarbageCollector()
		{
			return m_collector;
		}

		template <class T> T* AllocateMemoryForObject();

		Object* AllocateMemory(const uint64 Byte);

	protected:

		MemoryManager();

	private:

		static MemoryManager* m_MemoryInstance;
		static GB::GarbageCollector* m_collector;
	};

	template <class T> inline T* MemoryManager::AllocateMemoryForObject()
	{
		return static_cast<T*>(Allocator::Allocate(sizeof(T)));
	}
} // namespace CoreEngine
