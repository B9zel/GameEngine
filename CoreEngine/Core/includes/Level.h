#pragma once
#include <Core/includes/Base.h>
#include <Runtime/CoreObject/Include/Object.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class Object;
		class Actor;
	}
	class World;

	class Level : public Runtime::Object
	{
	private:

		friend World;

	public:

		const DArray<Runtime::Actor*>& GetActors() const;

		virtual void ActorInitialize();
		virtual void InitProperties() override;
		
	private:
		
		void AddActor(Runtime::Actor* newActor);

	protected:

		DArray<Runtime::Object*> m_Objects;
		DArray<Runtime::Actor*> m_Actors;
		DArray<ObjectPtr<Runtime::Actor>> m_ActorsGC;
	};
}