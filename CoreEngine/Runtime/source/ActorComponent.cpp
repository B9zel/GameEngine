#include <Runtime/includes/ActorComponent.h>
#include <Core/includes/World.h>



namespace CoreEngine
{
	namespace Runtime
	{
		void ActorComponent::InitProperties()
		{
			Object::InitProperties();



		}
		void ActorComponent::RegisteredComponent()
		{
			if (isRegistered) return;

			GetWorld()->GetUpdateManager()->AddFunction(updateFunc);
			isRegistered = true;
		}
	}
}
