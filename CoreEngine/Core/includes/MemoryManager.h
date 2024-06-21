#pragma once
#include <Core/includes/Memory/GarbageCollector.h>
#include <Core/includes/Memory/Allocator.h>

namespace CoreEngine
{
	class MemoryManager
	{
	public:

		MemoryManager* Create();

		virtual ~MemoryManager();

	private:

		MemoryManager();

	private:

		static MemoryManager* m_MemeoryInstance;

		GarbageCollector* m_collector;
	};
}