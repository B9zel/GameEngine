#pragma once

#include <Core/includes/Memory/GarbageCollector.h>
#include <Core/includes/Layer.h>
#include <Core/includes/Application.h>
#include <Core/includes/GBNotify.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>



#define PROPERTY(type, name) CoreEngine::GB::GBNotify<type> name
	
	


namespace CoreEngine
{
	namespace GB
	{
		class GarbageCollector;
	}

	namespace Runtime
	{
		class Object : public Layer
		{
		public:

			Object() = default;

		public:

			unsigned int GetReferenceCount() const { return m_referenceCount; }
			
			virtual void NativeUpdate(float deltaTime) override;
			virtual void OnAttach() override;
			virtual void OnDetach() override;

			virtual void Tick(float deltaTime) {}
			virtual void BeginPlay() {}
			virtual void EndPlay() {}


		private:

			void AddReference() { ++m_referenceCount; }
			void RemoveReference()  { --m_referenceCount; }

		private:

			PROPERTY(Object*, m_Outer);

			unsigned int m_referenceCount;

			friend GB::GarbageCollector;
		};
	}
}