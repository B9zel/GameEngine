#include <Core/includes/Memory/GarbageCollector.h>



namespace CoreEngine
{
	namespace GB
	{
		GarbageCollector* GarbageCollector::m_GBInstatnce = nullptr;


		GarbageCollector* GarbageCollector::Create()
		{
			CORE_ASSERT(m_GBInstatnce != nullptr, "GarbageCollector already exists");

			m_GBInstatnce = Allocator::Allocate<GarbageCollector>();
			return m_GBInstatnce;
		}

		void GarbageCollector::AddProperty(GBNotify<Runtime::Object*>* property)
		{
			property->m_Mehtod.Assign(&GarbageCollector::OnChangePointer, this);
		}

		void GarbageCollector::AddObject(Runtime::Object* object)
		{
			m_Objects.insert(object);
		}

		void GarbageCollector::Collect()
		{
			for (Runtime::Object* el : m_Objects)
			{
				if (!el->GetReferenceCount())
				{
					m_Objects.erase(el);
					Allocator::Deallocate<Runtime::Object>(el);
				}
			}
		}
		void GarbageCollector::OnChangePointer(Runtime::Object* oldPtr, Runtime::Object* newPtr)
		{
			oldPtr->RemoveReference();
			newPtr->AddReference();
		}
	}
}