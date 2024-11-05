#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/MemoryManager.h>
#include <Core/includes/Application.h>


namespace CoreEngine
{
	namespace Runtime
	{
		template<class TParent, class TChild>
		constexpr bool IsParentClass()
		{
			return std::is_base_of_v<TParent, TChild>;
		}

		template<class TParent, class TChild>
		constexpr bool IsParentClass(const TParent& parent,const TChild& child)
		{
			return std::is_base_of_v<std::remove_reference_t<std::remove_const_t<decltype(parent)>>, std::remove_reference_t< std::remove_const_t<decltype(child)>>>;
		}

		template<class T>
		T* CreateObject(Object* Outer=nullptr)
		{
			static_assert(IsParentClass<Object, T>(), "The class being created must be a descendant of the object.");

			MemoryManager* memManager = Engine::Get()->GetMamoryManager().get();
			T* newObject = memManager->AllocateMemoryForObject<T>();
			Object* convertObject = static_cast<Object*>(newObject);

			Allocator::Construct(newObject);
			memManager->GetGarbageCollector()->AddObject(newObject);
			convertObject->SetWorld(Engine::Get()->GetWorld().get());

			return newObject;
		}

	}
}