#include <Runtime/includes/Actor.h>
#include <Runtime/includes/SceneComponent.h>
#include <Runtime/includes/ActorComponent.h>
#include <Runtime/includes/Controller.h>




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
		}

		void Actor::DispatchBeginPlay()
		{
			if (GetWorld())
			{
				BeginPlay();
			}
		}

		void Actor::BeginPlay()
		{
			for (auto& Component : Components)
			{
				Component->RegisteredComponent();
			}

			Registered();
			isBeginedPlay = true;
		}

		void Actor::Update(float deltaTime)
		{
		}

		void Actor::NativeUpdate(float deltaTime)
		{
			Update(deltaTime);
		}

		void Actor::Registered()
		{
			GetWorld()->GetUpdateManager()->AddFunction(&actorUpdate);
			actorUpdate.SetUpdateMethod(&Actor::NativeUpdate, this);
		}

		void Actor::InitComponents()
		{
			for (ActorComponent* component : Components)
			{
				if (component->GetIsActive())
				{
					component->InitProperties();
				}
			}
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
		const DArray<ActorComponent*>& Actor::GetComponents() const
		{
			return Components;
		}
	}
}