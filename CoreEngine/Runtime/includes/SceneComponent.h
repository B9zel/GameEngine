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

			SceneComponent(const InitializeObject& Object);

		public:

			virtual void UpdateComponent(float deltaTime) override {}

			const FTransform& GetTransform() const;
			void SetTransform(const FTransform& newTransform);

			FVector GetComponentLocation() const;
			FVector GetReletiveLocation() const;
			FVector GetComponentScale() const;
			FVector GetComponentRotation() const;
			FVector GetForwardVector() const;
			FVector GetRightVector() const;

			virtual void SetComponentRotation(const FVector& newRotation);
			virtual void SetComponentLocation(const FVector& newLocation);
			virtual void SetComponentScale(const FVector& newScale);

			virtual void AddComponentRotation(const FVector& addRotation);
			virtual void AddComponentLocation(const FVector& addLocation);
			virtual void AddComponentScale(const FVector& addScale);

			virtual void SetupToAttachment(SceneComponent* attach);

			virtual const DArray<SceneComponent*>& GetChildrenAttaches() const;
			virtual SceneComponent* GetParentAttach() const;

			virtual FMatrix4x4 MakeMatrixMesh() const;
			virtual FMatrix4x4 MakeParentMatrix() const;

		protected:

			FVector CalculateForwardDirection(const FVector& VedDirect, const bool IsConvertToRadian = true) const;
			FVector CalculateRightDirection() const;

		protected:

			RPROPERTY(EditorVisible);
			FTransform Transform;
			FVector Front;
			SceneComponent* parentAttach;

			RPROPERTY()
			DArray<SceneComponent*> childrenAttach;
		};
	}
}