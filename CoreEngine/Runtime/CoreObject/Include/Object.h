#pragma once

#include <Core/includes/Base.h>
#include <Core/includes/ObjectPtr.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/UUID.h>
#include <ReflectionSystem/Include/MetaClass.h>
#include <ReflectionSystem/Include/ReflectionMacros.h>
#include <ReflectionSystem/Include/RegistryMap/MapRegistryClass.h>
#include <Object.generated.h>



namespace CoreEngine
{
	class Layer;
	class Engine;
	template<class T>
	class ObjectPtr;
	namespace GB
	{
		class GarbageCollector;
	}
}


#define PROPERTY(type, name) CoreEngine::ObjectPtr<type> name

enum class ObjectGCFlags : uint8
{
	None = 0,
	RootObject = FLAG_OFFSET(0),
	LiveObject = FLAG_OFFSET(1),
	Unreachable = FLAG_OFFSET(2)
};




namespace CoreEngine
{
	class World;
	class SerializeAchive;
	template<typename T>
	class ObjectPtr;

	struct InitializeObject
	{
		Reflection::ClassField* Class = nullptr;
	};

	namespace Runtime
	{
		RCLASS();
		class Object
		{

			GENERATED_BODY();

		public:

			Object(const InitializeObject& Initilize);
			virtual ~Object() { EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Destroy object"); }

		public:

			virtual void InitProperties();
			virtual Reflection::ClassField* GetClass() const;

			template<class ReturnType>
			ReturnType* CreateSubObject(const String& Name);

			void SetWorld(World* newWorld);
			World* GetWorld();

			const UUID& GetUUID() const;
			const String& GetName() const;

			void SetName(const String& NewName);

			uint32 GetGCState() const;
			bool GetHasSerialized() const;

			virtual void StartDestroy() {}
			virtual void FinishDestroy() {}
			
			virtual void PreSerialize();
			virtual void Serialize(SerializeAchive& Archive);

		private:

			ObjectPtr<Object> m_Outer;
			World* m_World;

			UUID ObjectID;
			Reflection::ClassField* PrivateClass;
			String Name;

			// GC
			uint32 StateObjectFlagGC{ 0 };
			//

			// Serialize
			bool HasSerialize{ false };

			friend GB::GarbageCollector;
		};

		template<class ReturnType>
		inline ReturnType* Object::CreateSubObject(const String& Name)
		{
			ReturnType* obj = CreateObject<ReturnType>(this);
			obj->Name = Name;
			return obj;
		}


	}
}