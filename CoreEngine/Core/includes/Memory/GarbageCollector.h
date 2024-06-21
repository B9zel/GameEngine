#pragma once
#include <Core/includes/Base.h>
#include <Runtime/includes/Object.h>
#include <Core/includes/GBNotify.h>
#include <Core/includes/Memory/Allocator.h>




namespace CoreEngine
{
	namespace Runtime
	{
		class Object;
	}

	namespace GB
	{
		class GarbageCollector
		{
		public:

			GarbageCollector* Create();
			
			void AddProperty(GBNotify<Runtime::Object*>* property);

			void AddObject(Runtime::Object* object);

			void Collect();

		private:

			GarbageCollector() = default;

			void OnChangePointer(Runtime::Object* oldPtr, Runtime::Object* newPtr);

		private:

			//friend GBNotify<Runtime::Object*>;

			static GarbageCollector* m_GBInstatnce;

			HashTableSet<Runtime::Object*> m_Objects;
		};
	}
}