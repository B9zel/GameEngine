#pragma once
#include <Core/includes/Base.h>
#include <Runtime/includes/ActorComponent.h>
#include <Math/includes/Transform.h>
#include <SceneComponent.generated.h>


namespace CoreEngine
{
	namespace Runtime
	{
		RCLASS();
		class SceneComponent : public ActorComponent
		{
			GENERATED_BODY()


		public:

			SceneComponent();

		public:

			virtual void UpdateComponent(float deltaTime) override {}

			const Transform& GetTransform() const;
			void SetTransform(const Transform& newTransform);

			FVector GetComponentLocation() const;
			FVector GetComponentScale() const;
			FVector GetComponentRotation() const;
			FVector GetForwardVector();
			FVector GetRightVector();

			virtual void SetComponentRotation(const FVector& newRotation);
			virtual void SetComponentLocation(const FVector& newLocation);
			virtual void SetComponentScale(const FVector& newScale);

			virtual void AddComponentRotation(const FVector& addRotation);
			virtual void AddComponentLocation(const FVector& addLocation);
			virtual void AddComponentScale(const FVector& addScale);

			virtual void SetupToAttachment(SceneComponent* attach);

		private:

			FVector CalculateForwardDirection();
			FVector CalculateRightDirection();

		protected:

			Transform transform;
			FVector Front;
			SceneComponent* parentAttach;
			DArray<SceneComponent*> childrenAttach;
		};
	}
}