#pragma once
#include <Core/includes/Base.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/GBNotify.h>
#include <Core/includes/Application.h>
#include <Core/includes/TimerManager.h>


/**
* 
*  To add a pointer to track the garbage collector, use the PROPERTY macro in the Object class
*  
*/


class TimerManager;
class Timer;
struct TimerHandle;


namespace CoreEngine
{
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


			static GarbageCollector* GetGBInstance() { return m_GBInstatnce; }

		private:

			GarbageCollector();

		private:

			void Init();

			void Collect();
			void OnChangePointer(Runtime::Object* oldPtr, Runtime::Object* newPtr);

		private:

			static GarbageCollector* m_GBInstatnce;

			HashTableSet<Runtime::Object*> m_ObjectsPtr;
			HashTableSet<Runtime::Object*> m_deleteObject;


			float m_rateCollect;
			TimerHandle collectHandler;
		};
	}
}