#pragma once

#include <Core/includes/Base.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/ObjectPtr.h>
#include <Core/includes/Layer.h>
#include <ReflectionSystem/Include/MetaClass.h>
#include <ReflectionSystem/Include/ReflectionMacros.h>
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
	PendingKill = FLAG_OFFSET(1)
};




namespace CoreEngine
{
	class World;
	template<typename T>
	class ObjectPtr;

	namespace Runtime
	{
		RCLASS();
		class Object
		{

			GENERATED_BODY()

		public:

			Object();
			virtual ~Object() { EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Destroy object"); }

		public:

			virtual void InitProperties();

			template<class ReturnType>
			ReturnType* CreateSubObject();

			void SetWorld(World* newWorld);
			World* GetWorld();

			//Reflection::MetaClass* GetMetaClass() const;
			//static Reflection::MetaClass* GetStaticMetaClass();

			virtual void StartDestroy() {}
			virtual void FinishDestroy() {}

		public:

			RPROPERTY();
			int TestVar{ 0 };

		private:

			ObjectPtr<Object> m_Outer;
			World* m_World;
			// GC
			uint32 StateObjectFlagGC;
			bool IsMarkedGC;


			friend GB::GarbageCollector;
		};

		template<class ReturnType>
		inline ReturnType* Object::CreateSubObject()
		{
			ReturnType* obj = CreateObject<ReturnType>(this);
			return obj;
		}


	}
}