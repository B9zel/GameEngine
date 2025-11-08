#pragma once
#include <Core/includes/ObjectPtr.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/Base.h>
#include <Runtime/includes/Actor.h>
#include "Level.generated.h"


namespace CoreEngine
{
	namespace Runtime
	{
		class Object;
		class Actor;
	}
	class World;

	RCLASS()
	class Level : public Runtime::Object
	{
		GENERATED_BODY()

	private:

		friend World;

	public:

		Level(const InitializeObject& Object);

		const DArray<Runtime::Actor*>& GetActors() const;

		virtual void ActorInitialize();
		virtual void InitProperties() override;
		
	private:
		
		void AddActor(Runtime::Actor* newActor);

	protected:
		
		//RPROPERTY();
	//	DArray<float> m_Floats;
		RPROPERTY();
		DArray<Runtime::Object*> m_Objects;

		RPROPERTY();
		DArray<Runtime::Actor*> m_Actors;
		DArray<ObjectPtr<Runtime::Actor>> m_ActorsGC;
	};
}