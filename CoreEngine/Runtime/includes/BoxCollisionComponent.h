#pragma once

#include <Runtime/includes/CollisionComponent.h>
#include <BoxCollisionComponent.generated.h>

RCLASS(EditorComponent)
class BoxCollisionComponent : public CollisionComponent
{
	GENERATED_BODY()

public:

	BoxCollisionComponent(const CoreEngine::InitializeObject& Object);

public:

	virtual void PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property) override;

protected:

	virtual UniquePtr<CoreEngine::Physic::BodyInstance> CreateBodyInstance() override;

private:

	RPROPERTY(EditorVisible);
	FVector BoxHalfExtent = FVector::OneVector;
};
