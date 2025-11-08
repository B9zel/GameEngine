#include <Runtime/includes/Actor.h>
#include <Runtime/includes/SceneComponent.h>

#include <Runtime/includes/Controller.h>



namespace CoreEngine
{
	namespace Runtime
	{
		Actor::Actor(const InitializeObject& Initilize) : Object(Initilize), actorUpdate(), RootComponent(nullptr)
		{
			RootComponent = CreateSubObject<SceneComponent>();

			isBeginedPlay = false;
			isRegister = false;
		}

		void Actor::InitProperties()
		{
		}

		void Actor::RegisterAll()
		{
			for (auto& Component : Components)
			{
				if (Component->GetIsActive())
				{
					Component->RegisteredComponent();
				}
			}
		}

		void Actor::DispatchBeginPlay()
		{
			if (GetWorld() && !isBeginedPlay)
			{
				BeginPlay();
				isBeginedPlay = true;
			}
		}

		void Actor::BeginPlay()
		{
			for (ActorComponent* Component : Components)
			{
				if (Component)
				{
					Component->BeginPlay();
				}
			}
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
			if (isRegister) return;

			OnRegistered();
			isRegister = true;
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

		void Actor::PostSpawnActor()
		{
			Registered();
			RegisterAll();
		}

		void Actor::OnRegistered()
		{
			GetWorld()->GetUpdateManager()->AddFunction(&actorUpdate);
			actorUpdate.SetUpdateMethod(&Actor::NativeUpdate, this);
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

		Transform Actor::GetActorTransform() const
		{
			return RootComponent->GetTransform();
		}

		FVector Actor::GetActorForwardVector() const
		{
			return RootComponent ? RootComponent->GetForwardVector() : FVector::ForwardVector;
		}

		FVector Actor::GetActorRightVector() const
		{
			return RootComponent ? RootComponent->GetRightVector() : FVector::RightVector;
		}

		Actor* Actor::GetOwner() const
		{
			return Owner;
		}

		void Actor::SetActorLocation(const FVector& newLocation)
		{
			RootComponent->SetComponentLocation(newLocation);
		}

		void Actor::SetActorScale(const FVector& newScale)
		{
			RootComponent->SetComponentScale(newScale);
		}

		void Actor::SetActorRotation(const FVector& newRotation)
		{
			RootComponent->SetComponentRotation(newRotation);
		}

		void Actor::SetActorTransform(const Transform& newTransform)
		{
			RootComponent->SetTransform(newTransform);
		}

		void Actor::AddActorLocation(const FVector& AddLocation)
		{
			RootComponent->AddComponentLocation(AddLocation);
		}

		void Actor::AddActorRotation(const FVector& AddValue)
		{
			RootComponent->AddComponentRotation(AddValue);
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