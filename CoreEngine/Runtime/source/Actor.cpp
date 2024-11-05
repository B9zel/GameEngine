#include <Runtime/includes/Actor.h>
#include <Runtime/includes/SceneComponent.h>
#include <Runtime/includes/ActorComponent.h>
#include <Runtime/includes/Controller.h>
#include <Core/includes/World.h>



namespace CoreEngine
{
	namespace Runtime
	{
		Actor::Actor() : actorUpdate()
		{
			RootComponent = CreateSubObject<SceneComponent>();

			isBeginedPlay = false;
		}

		void Actor::InitProperties()
		{
			for (ActorComponent* component : OwnedComponents)
			{
				component->InitProperties();
			}
		}

		void Actor::BeginPlay()
		{

		}

		void Actor::Update(float deltaTime)
		{
		}

		void Actor::Registered()
		{
			GetWorld()->GetUpdateManager()->AddFunction(actorUpdate);
			actorUpdate.SetUpdateMethod(this, &Actor::Update);
		}

		SceneComponent* Actor::GetRootComponent() const
		{
			return RootComponent;
		}

		void Actor::SetRootComponent(SceneComponent* root)
		{
			if (root)
			{
				RootComponent = root;
			}
		}
		FVector Actor::GetActorLocation() const
		{
			return RootComponent->GetComponentLocation();
		}

		FVector Actor::GetActorScale() const
		{
			return RootComponent->GetComponentScale();
		}

		FVector Actor::GetActorRotation() const
		{
			return RootComponent->GetComponentRotation();
		}

		Actor* Actor::GetOwner() const
		{
			return Owner;
		}

		void Actor::SetActorLocation(const FVector& newLocation)
		{
			RootComponent->SetComponentLocation(newLocation);
		}

		void Actor::SetActorSclae(const FVector& newScale)
		{
			RootComponent->SetComponentScale(newScale);
		}

		void Actor::SetActorRotation(const FVector& newRotation)
		{
			RootComponent->SetComponentRotation(newRotation);
		}

		void Actor::SetOwner(Actor* newOwner)
		{
			Owner = newOwner;
		}
	}
}