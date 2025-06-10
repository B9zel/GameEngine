#include <Runtime/includes/ActorComponent.h>
#include <Core/includes/World.h>



namespace CoreEngine
{
	namespace Runtime
	{
		void ActorComponent::BeginPlay()
		{
		}
		void ActorComponent::InitProperties()
		{
			Object::InitProperties();

			updateFunc.SetUpdateMethod(&ActorComponent::UpdateComponent, this);
		}
		void ActorComponent::RegisteredComponent()
		{
			if (isRegistered) return;

			GetWorld()->GetUpdateManager()->AddFunction(&updateFunc);
			isRegistered = true;
		}
		void ActorComponent::SetOwner(Actor* Owner)
		{
			this->Owner = Owner;
		}

		Actor* ActorComponent::GetOwner() const
		{
			return Owner;
		}
		bool ActorComponent::GetIsActive() const
		{
			return isActivate;
		}
	}
}
