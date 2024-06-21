#pragma once

#include <Core/includes/Memory/GarbageCollector.h>
#include <Core/includes/GBNotify.h>

#define PROPERTY(type, name) \
	CoreEngine::GB::GBNotify<type> name; \
	
	

class GarbageCollector;

namespace CoreEngine
{
	namespace Runtime
	{
		class Object
		{
		public:

			unsigned int GetReferenceCount() const { return m_referenceCount; }

		private:

			void AddReference() { ++m_referenceCount; }
			void RemoveReference()  { --m_referenceCount; }

		private:

			unsigned int m_referenceCount;

			friend GarbageCollector;
		};
	}
}