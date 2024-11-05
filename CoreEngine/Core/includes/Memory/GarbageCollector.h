#pragma once
#include <Core/includes/Base.h>

#include <Core/includes/GBNotify.h>
#include <Core/includes/TimerManager.h>


/**
* 
*  To add a pointer to track the garbage collector, use the PROPERTY macro in the Object class
*  
*/

namespace CoreEngine
{
	namespace GB
	{
		template<class T>
		class GBNotify;
	}
}




namespace CoreEngine
{
	class TimerManager;
	class Timer;
	struct TimerHandle;
	class Application;

	namespace Runtime
	{
		class Object;
	}

	namespace GB
	{
		class GarbageCollector
		{
		public:

			static GarbageCollector* Create();
			
			void AddProperty(GBNotify<Runtime::Object*>* property);

			void AddObject(Runtime::Object* object);
			void AddRootObject(Runtime::Object* object);
			void AddReference(Runtime::Object* object);
			void RemoveObject(Runtime::Object* object);
			void RemoveReference(Runtime::Object* object);

			static GarbageCollector* GetGBInstance() { return m_GBInstatnce; }

		private:

			GarbageCollector();

		private:

			void Init();

			void Collect();
			void MarkLiveObjects(HashTableSet<Runtime::Object*>& outMarkedObjects);
			void MarkObject(Runtime::Object* object, HashTableSet<Runtime::Object*>& outMarkedObjects);

			void OnChangePointer(Runtime::Object* oldPtr, Runtime::Object* newPtr);

		private:

			static GarbageCollector* m_GBInstatnce;

			HashTableSet<Runtime::Object*> m_Objects;
			HashTableSet<Runtime::Object*> m_RootObjects;
			HashTableMap<Runtime::Object*, size_t> m_ReferenceObjects;


			float m_rateCollect;
			TimerHandle collectHandler;
		};
	}
}