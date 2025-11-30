#include <Core/includes/Memory/GarbageCollector.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/Memory/Allocator.h>
#include <Core/includes/Engine.h>
#include <Core/includes/TimerManager.h>
#include <Core/includes/World.h>
#include <ReflectionSystem/Include/BaseField.h>


namespace CoreEngine
{
	namespace GB
	{
		GarbageCollector* GarbageCollector::m_GBInstance = nullptr;


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
			if (m_GBInstance)
			{
				EG_LOG(CORE, ELevelLog::ERROR, "GarbageCollector already exists");
				return m_GBInstance;
			}

			m_GBInstance = (GarbageCollector*)(Allocator::Allocate(sizeof(GarbageCollector)));
			Allocator::Construct(m_GBInstance, GarbageCollector());

			m_GBInstance->Init();
			
			return m_GBInstance;
		}

		
		void GarbageCollector::AddObject(Runtime::Object* object)
		{
			m_Objects.insert(object);
		}

		void GarbageCollector::AddRootObject(Runtime::Object* object)
		{
			m_RootObjects.insert(object);
			//AddObject(object);
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
			ResetMarks();
			MarkLiveObjects();
			EG_LOG(CORE, ELevelLog::WARNING, "Collect");

			static DArray<Runtime::Object*> deleteObjects;
			deleteObjects.clear();
			for (auto* obj : m_Objects)
			{
				if (!obj) continue;

				if (HasFlag(obj->GetGCState(), static_cast<uint32>(ObjectGCFlags::Unreachable)))
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

		void GarbageCollector::ResetMarks()
		{
			for (auto* RootObj : m_RootObjects)
			{
				SetFlag(RootObj->StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::Unreachable));
				RemoveFlag(RootObj->StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::LiveObject));
			}

			for (auto* obj : m_Objects)
			{
				SetFlag(obj->StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::Unreachable));
				RemoveFlag(obj->StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::LiveObject));
			}
		}

		void GarbageCollector::MarkObject(Runtime::Object* object)
		{
			if (!object || HasFlag(object->GetGCState(), static_cast<uint32>(ObjectGCFlags::LiveObject)) || HasFlag(object->GetGCState(), static_cast<uint32>(ObjectGCFlags::Garbage))) return;
			RemoveFlag(object->StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::Unreachable));
			SetFlag(object->StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::LiveObject));
			
			Reflection::ClassField* CurrentClass = object->GetClass();
			while (CurrentClass != nullptr)
			{
				for (auto* Variable : CurrentClass->PropertyFileds)
				{
					if (Variable->GetIsPointer() && Variable->GetIsSupportReflectionSystem())
					{
						//MarkObject(*Variable->GetSourcePropertyByName<Runtime::Object*>(object));
						if (Variable->GetPrimitiveType() == Reflection::EConteinType::ARRAY)
						{
							if (auto* ArrayField = dynamic_cast<Reflection::ArrayPropertyField*>(Variable))
							{
								for (uint64 i = 0; i < ArrayField->GetSizeArray<Runtime::Object*>(object); ++i)
								{
									MarkObject(*ArrayField->GetElement<Runtime::Object*>(object, i));
								}
							}
						}
						else if (Variable->GetPrimitiveType() == Reflection::EConteinType::PRIMITIVE)
						{
							MarkObject(*Variable->GetSourcePropertyByName<Runtime::Object*>(object));
						}
					}
				}
				CurrentClass = CurrentClass->ParentClass;
			}
			

		}

		void GarbageCollector::MarkLiveObjects()
		{
			for (auto& el : m_RootObjects)
			{
				MarkObject(el);
			}

			/*for (auto& el : m_ReferenceObjects)
			{
				MarkObject(el.first, outMarkedObjects);
			}*/
		}

		void GarbageCollector::MarkObject(Runtime::Object* object, HashTableSet<Runtime::Object*>& outMarkedObjects)
		{
			if (outMarkedObjects.find(object) != outMarkedObjects.end()) return;

			outMarkedObjects.insert(object);
			
		}


		void GarbageCollector::OnChangePointer(Runtime::Object* oldPtr, Runtime::Object* newPtr)
		{
			if (oldPtr == newPtr) return;
			
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