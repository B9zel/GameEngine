#pragma once
#include <Core/includes/Memory/GarbageCollector.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/Base.h>
#include <Core/includes/ObjectPtr.h>
#include <Core/includes/Layer.h>



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
	



namespace CoreEngine
{
	class World;

	namespace Runtime
	{
		class Object
		{
		public:

			Object();
			virtual ~Object() { EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Destroy object"); }

		public:

			virtual void InitProperties();

			template<class ReturnType>
			ReturnType* CreateSubObject();

			void SetWorld(World* newWorld);
			World* GetWorld();

			virtual void StartDestroy() {}
			virtual void FinishDestroy() {}

		private:

			ObjectPtr<Object> m_Outer;
			World* m_World;


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