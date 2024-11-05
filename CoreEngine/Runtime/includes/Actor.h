#pragma once
#include <Core/includes/Base.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/UpdateFunction.h>


namespace CoreEngine
{
	class UpdateFunction;

	namespace Runtime
	{
		class SceneComponent;
		class ActorComponent;
		class Controller;


		class Actor : public Object
		{
		public:

			Actor();

		public:

			virtual void InitProperties() override;
			virtual void BeginPlay();
			virtual void Update(float deltaTime);
			virtual void NativeUpdate(float deltaTime);
			virtual void Registered();


			SceneComponent* GetRootComponent() const;
			void SetRootComponent(SceneComponent* root);

			FVector GetActorLocation() const;
			FVector GetActorScale() const;
			FVector GetActorRotation() const;
			Actor* GetOwner() const;

			void SetActorLocation(const FVector& newLocation);
			void SetActorSclae(const FVector& newScale);
			void SetActorRotation(const FVector& newRotation);
			void SetOwner(Actor* newOwner);

		protected:

			SceneComponent* RootComponent;
			UpdateFunction actorUpdate;

			DArray<ActorComponent*> OwnedComponents;
			Actor* Owner;

		private:

			bool isBeginedPlay;
			bool isRegister;
		};
	}
}