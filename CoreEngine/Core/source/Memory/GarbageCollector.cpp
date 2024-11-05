#include <Core/includes/Memory/GarbageCollector.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/Memory/Allocator.h>
#include <Core/includes/Engine.h>
#include <Core/includes/TimerManager.h>
#include <Core/includes/World.h>



namespace CoreEngine
{
	namespace GB
	{
		GarbageCollector* GarbageCollector::m_GBInstatnce = nullptr;


		GarbageCollector::GarbageCollector()
		{
			m_rateCollect = 5;
		}

		void GarbageCollector::Init()
		{
			Engine::Get()->GetTimerManager()->SetTimer(collectHandler, this, &GarbageCollector::Collect, m_rateCollect, true);
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
			m_Objects.insert(object);
		}

		void GarbageCollector::AddRootObject(Runtime::Object* object)
		{
			m_RootObjects.insert(object);
			AddObject(object);
		}

		void GarbageCollector::AddReference(Runtime::Object* object)
		{
			if (m_ReferenceObjects.count(object))
			{
				m_ReferenceObjects[object]++;
			}
			else
			{
				m_ReferenceObjects.emplace(object, 1);
			}

		}

		void GarbageCollector::RemoveObject(Runtime::Object* object)
		{
			m_ReferenceObjects.erase(object);
			m_RootObjects.erase(object);
			m_Objects.erase(object);
		}

		void GarbageCollector::RemoveReference(Runtime::Object* object)
		{
			if (m_ReferenceObjects.count(object))
			{
				if (m_ReferenceObjects[object] <= 1)
				{
					m_ReferenceObjects.erase(object);
				}
				else
				{
					m_ReferenceObjects[object]--;
				}
			}
		}

		void GarbageCollector::Collect()
		{
			HashTableSet<Runtime::Object*> markedObjects;
			MarkLiveObjects(markedObjects);
			EG_LOG(CORE, ELevelLog::INFO, "Collect");

			DArray<Runtime::Object*> deleteObjects;
			for (auto* obj : m_Objects)
			{
				if (markedObjects.find(obj) == markedObjects.end())
				{
					deleteObjects.push_back(obj);
					
					EG_LOG(CORE, ELevelLog::INFO, "Delete");
				}
			}

			for (auto* delObj : deleteObjects)
			{
				RemoveObject(delObj);
				Allocator::Deallocate(delObj);
			}
		}

		void GarbageCollector::MarkLiveObjects(HashTableSet<Runtime::Object*>& outMarkedObjects)
		{
			for (auto& el : m_RootObjects)
			{
				MarkObject(el, outMarkedObjects);
			}

			for (auto& el : m_ReferenceObjects)
			{
				MarkObject(el.first, outMarkedObjects);
			}
		}

		void GarbageCollector::MarkObject(Runtime::Object* object, HashTableSet<Runtime::Object*>& outMarkedObjects)
		{
			if (outMarkedObjects.find(object) != outMarkedObjects.end()) return;

			outMarkedObjects.insert(object);
		}


		void GarbageCollector::OnChangePointer(Runtime::Object* oldPtr, Runtime::Object* newPtr)
		{
			if (oldPtr == newPtr)
			{
				return;
			}
			if (oldPtr)
			{
				RemoveReference(oldPtr);
			}
			if (newPtr)
			{
				AddReference(newPtr);
			}
		}
	}
}