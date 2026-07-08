#include <Core/includes/Memory/GarbageCollector.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/Memory/Allocator.h>
#include <Core/includes/Engine.h>
#include <Core/includes/TimerManager.h>
// #include <Core/includes/World.h>
// #include <ReflectionSystem/Include/BaseField.h>

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

		void GarbageCollector::AddObject(Object* object)
		{
			m_Objects.insert(object);
		}

		void GarbageCollector::AddRootObject(Object* object)
		{
			m_RootObjects.insert(object);
			// AddObject(object);
		}

		void GarbageCollector::AddReference(Object* object)
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

		void GarbageCollector::RemoveObject(Object* object)
		{
			m_ReferenceObjects.erase(object);
			m_RootObjects.erase(object);
			m_Objects.erase(object);
		}

		void GarbageCollector::RemoveRootObject(Object* object)
		{
			auto It = m_RootObjects.find(object);
			if (It != m_RootObjects.end())
			{
				m_RootObjects.erase(object);
				// m_Objects.insert(object);
			}
		}

		void GarbageCollector::RemoveReference(Object* object)
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

			static DArray<Object*> deleteObjects;
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

		void GarbageCollector::MarkObject(Object* object)
		{
			if (!object || HasFlag(object->GetGCState(), static_cast<uint32>(ObjectGCFlags::LiveObject)) ||
				HasFlag(object->GetGCState(), static_cast<uint32>(ObjectGCFlags::Garbage)))
				return;
			RemoveFlag(object->StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::Unreachable));
			SetFlag(object->StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::LiveObject));

			Reflection::ClassField* CurrentClass = object->GetClass();
			while (CurrentClass != nullptr)
			{
				for (auto* Variable : CurrentClass->PropertyFileds)
				{
					if (Variable->GetIsPointer() && Variable->GetIsSupportReflectionSystem())
					{
						// MarkObject(*Variable->GetSourcePropertyByName<Runtime::Object*>(object));
						if (Variable->GetPrimitiveType() == Reflection::EConteinType::ARRAY)
						{
							if (auto* ArrayField = dynamic_cast<Reflection::ArrayPropertyField*>(Variable))
							{
								for (uint64 i = 0; i < ArrayField->GetSizeArray<Object*>(object); ++i)
								{
									MarkObject(*ArrayField->GetElement<Object*>(object, i));
								}
							}
						}
						else if (Variable->GetPrimitiveType() == Reflection::EConteinType::PRIMITIVE)
						{
							MarkObject(*Variable->GetSourcePropertyByName<Object*>(object));
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

		void GarbageCollector::MarkObject(Object* object, HashTableSet<Object*>& outMarkedObjects)
		{
			if (outMarkedObjects.find(object) != outMarkedObjects.end()) return;

			outMarkedObjects.insert(object);
		}

		void GarbageCollector::OnChangePointer(Object* oldPtr, Object* newPtr)
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

		const HashTableSet<Object*>& GarbageCollector::GetObjects() const
		{
			return m_Objects;
		}
		const HashTableSet<Object*>& GarbageCollector::GetRootObjects() const
		{
			return m_RootObjects;
		}
		const HashTableSet<Object*>& GarbageCollector::GetAllObjects() const
		{
			static HashTableSet<Object*> AllObjects;
			AllObjects.clear();

			AllObjects = m_Objects;
			AllObjects.insert(m_RootObjects.begin(), m_RootObjects.end());
			return AllObjects;
		}
	} // namespace GB
} // namespace CoreEngine
