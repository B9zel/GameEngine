#include <Core/includes/Memory/GarbageCollector.h>
#include <Core/includes/Memory/Allocator.h>


namespace CoreEngine
{
	namespace GB
	{
		GarbageCollector* GarbageCollector::m_GBInstatnce = nullptr;


		void GarbageCollector::Init()
		{
			Application::Get()->GetTimerManager()->SetTimer(collectHandler, this, &GarbageCollector::Collect, 10.f, true);
		}


		GarbageCollector* GarbageCollector::Create()
		{
			if (m_GBInstatnce)
			{
				EG_LOG(CORE, ELevelLog::ERROR, "GarbageCollector already exists");
				return m_GBInstatnce;
			}

			m_GBInstatnce = (GarbageCollector*)(Allocator::Allocate(sizeof(GarbageCollector)));
			Allocator::Construct(m_GBInstatnce, GarbageCollector());

			m_GBInstatnce->Init();
			
			return m_GBInstatnce;
		}

		void GarbageCollector::AddProperty(GBNotify<Runtime::Object*>* property)
		{
			property->m_Mehtod.Assign(&GarbageCollector::OnChangePointer, this);
		}

		void GarbageCollector::AddObject(Runtime::Object* object)
		{
			m_ObjectsPtr.insert(object);
		}

		void GarbageCollector::Collect()
		{
			for (Runtime::Object* el : m_ObjectsPtr)
			{
				if (!el)
				{
					continue;
				}
				
				if (!el->GetReferenceCount())
				{
					m_deleteObject.emplace_back(el);
				}
			}

			for (Runtime::Object* delObj : m_deleteObject)
			{
				m_ObjectsPtr.erase(delObj);
				Allocator::Deallocate<Runtime::Object>(delObj);
			}
			m_deleteObject.clear();
		}
		void GarbageCollector::OnChangePointer(Runtime::Object* oldPtr, Runtime::Object* newPtr)
		{
			if (oldPtr == newPtr)
			{
				return;
			}
			if (oldPtr)
			{
				oldPtr->RemoveReference();
			}
			if (newPtr)
			{
				newPtr->AddReference();
			}
		}
	}
}