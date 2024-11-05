#pragma once
#include <Core/includes/Base.h>
#include <Runtime/includes/ActorComponent.h>
#include <Math/includes/Transform.h>



namespace CoreEngine
{
	namespace Runtime
	{
		class SceneComponent : public ActorComponent
		{
		public:

			SceneComponent() = default;

		public:

			virtual void TickComponent(float deltaTime) override {}

			const Transform& GetTransform() const;
			void SetTransform(const Transform& newTransform);

			FVector GetComponentLocation() const;
			FVector GetComponentScale() const;
			FVector GetComponentRotation() const;

			void SetComponentRotation(const FVector& newRotation);
			void SetComponentLocation(const FVector& newLocation);
			void SetComponentScale(const FVector& newScale);

			void SetupToAttachment(SceneComponent* attach);

		protected:

			Transform transform;

			SceneComponent* parentAttach;
			DArray<SceneComponent*> childrenAttach;
		};
	}
}