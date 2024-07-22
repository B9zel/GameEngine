#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/Application.h>


namespace CoreEngine
{
	namespace Runtime
	{

		template<class T>
		T* CreateObject(Object* Outer)
		{
			static_assert(std::is_same_v<T, Object>, "The class being created must be a descendant of the object.");

			MemoryManager* memManager = Application::Get()->GetMamoryManager().get();
			T* newObject = memManager->AllocateMemoryForObject<T>();

			Allocator::Construct(newObject);
			memManager->GetGarbageCollector()->AddObject(newObject);

			return newObject;
		}
	}
}