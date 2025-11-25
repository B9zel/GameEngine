#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/MemoryManager.h>
#include <Core/includes/Application.h>
#include <ReflectionSystem/Include/ReflectionMacros.h>
#include <ReflectionSystem/Include/RegistryMap/MapRegistryClass.h>


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
			
			if (!T::GetStaticClass())
			{
				EG_LOG(CORE, ELevelLog::ERROR, "Static class don't exist");
				return nullptr;
			}
			MemoryManager* memManager = Engine::Get()->GetMemoryManager().get();
			T* newObject = memManager->AllocateMemoryForObject<T>();

			memManager->GetGarbageCollector()->AddObject(newObject);
			InitializeObject InitParam;
			InitParam.Class = T::GetStaticClass();
			Allocator::Construct(newObject, InitParam);

			return newObject;
		}

		template<class T>
		T* CreateObject(Reflection::ClassField* Class, Object* Outer = nullptr)
		{
			if (!Class) return nullptr;

			if (!T::GetStaticClass())
			{
				EG_LOG(CORE, ELevelLog::ERROR, "Static class don't exist");
				return nullptr;
			}

			MemoryManager* memManager = Engine::Get()->GetMemoryManager().get();
			Runtime::Object* NewObject = memManager->AllocateMemoryForObject(Class->Size);

			InitializeObject InitParam;
			InitParam.Class = Class;
			Class->ConstructInstanceObject(NewObject, InitParam);
			memManager->GetGarbageCollector()->AddObject(newObject);

			return dynamic_cast<T*>(NewObject);
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