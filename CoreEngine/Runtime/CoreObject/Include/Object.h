#pragma once
#include <Core/includes/Memory/GarbageCollector.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/Base.h>
#include <Core/includes/GBNotify.h>
#include <Core/includes/Layer.h>



namespace CoreEngine
{
	class Layer;
	class Engine;
	namespace GB
	{
		template<class T>
		class GBNotify;

		class GarbageCollector;
	}
}
	

#define PROPERTY(type, name) CoreEngine::GB::GBNotify<type> name
	



namespace CoreEngine
{
	class World;

	namespace Runtime
	{
		class Object
		{
		public:

			Object() = default;
			virtual ~Object() { EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Destoy object"); }

		public:

			virtual void InitProperties() {};

			template<class ReturnType>
			ReturnType* CreateSubObject();

			void SetWorld(World* newWorld);
			World* GetWorld();

			virtual void StartDestroy() {}
			virtual void FinishDestroy() {}

		private:

			PROPERTY(Object*, m_Outer);
			World* m_World;

			//DArray<Object*> m_SubObjects;

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