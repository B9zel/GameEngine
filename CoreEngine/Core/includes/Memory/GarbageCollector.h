#pragma once
#include <Core/includes/Base.h>
#include <Templates/Function.h>
#include <Core/includes/ObjectPtr.h>
#include <Core/includes/TimerManager.h>

/**
 *
 *  To add a pointer to track the garbage collector, use the PROPERTY macro in the Object class
 *
 */

namespace CoreEngine
{
	template <class T> class ObjectPtr;
}

class Object;

namespace CoreEngine
{
	class TimerManager;
	class Timer;
	struct TimerHandle;
	class Application;

	namespace GB
	{
		class GarbageCollector
		{
		public:

			static GarbageCollector* Create();

			template <class T> void AddProperty(ObjectPtr<T>* Property);

			void AddObject(Object* object);
			void AddRootObject(Object* object);
			void AddReference(Object* object);
			void RemoveObject(Object* object);
			void RemoveRootObject(Object* object);
			void RemoveReference(Object* object);

			static GarbageCollector* GetGBInstance()
			{
				return m_GBInstance;
			}

		private:

			GarbageCollector();

		private:

			void Init();

			void Collect();
			void ResetMarks();

			void MarkObject(Object* object);

			void MarkLiveObjects();
			void MarkObject(Object* object, HashTableSet<Object*>& outMarkedObjects);

			void OnChangePointer(Object* oldPtr, Object* newPtr);

		private:

			static GarbageCollector* m_GBInstance;

			HashTableSet<Object*> m_Objects;
			HashTableSet<Object*> m_RootObjects;
			HashTableMap<Object*, size_t> m_ReferenceObjects;

			float m_rateCollect;
			TimerHandle collectHandler;
		};
		template <class T> inline void GarbageCollector::AddProperty(ObjectPtr<T>* Property)
		{
			Property->m_Method.Assign(&GarbageCollector::OnChangePointer, this);
		}
	} // namespace GB
} // namespace CoreEngine
