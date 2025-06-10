#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/MemoryManager.h>
#include <Core/includes/Application.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class Object;

		template<class TParent, class TChild>
		constexpr bool IsParentClass()
		{
			return std::is_base_of<TParent, TChild>::value;
		}

		template<class TParent, class TChild>
		constexpr bool IsParentClass(const TParent& parent, const TChild& child)
		{
			return std::is_base_of_v<std::remove_reference_t<std::remove_const_t<decltype(parent)>>, std::remove_reference_t< std::remove_const_t<decltype(child)>>>;
		}

		template<class TFirst, class TSecond>
		constexpr bool IsEqualType()
		{
			return std::is_same<TFirst, TSecond>().value;
		}

		template<class T>
		T* CreateObject(Object* Outer = nullptr)
		{
			static_assert(IsParentClass<Object, T>(), "The class being created must be a descendant of the object.");

			MemoryManager* memManager = Engine::Get()->GetMemoryManager().get();
			T* newObject = memManager->AllocateMemoryForObject<T>();

			memManager->GetGarbageCollector()->AddObject(newObject);
			Allocator::Construct(newObject);

			return newObject;
		}
		template<class T>
		T* CreateObjectWithInit(Object* Outer = nullptr)
		{
			T* Obj = CreateObject<T>(Outer);
			Obj->InitProperties();

			return Obj;
		}

	}
}