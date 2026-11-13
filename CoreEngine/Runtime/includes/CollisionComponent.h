#pragma once
#include <Runtime/includes/SceneComponent.h>
#include <Physics/Include/BodyInstance.h>
#include <CollisionComponent.generated.h>

namespace CoreEngine::Physic
{
	class BodyInstance;
	struct CallbackUpdateDataRB;
} // namespace CoreEngine::Physic

RCLASS()
class CollisionComponent : public SceneComponent
{
	GENERATED_BODY()

public:

	CollisionComponent(const CoreEngine::InitializeObject& Object);

public:

	virtual void UpdateComponent(float deltaTime) override;

	virtual void SetComponentRotation(const FVector& newRotation) override;
	virtual void SetComponentLocation(const FVector& newLocation) override;

	virtual void AddComponentRotation(const FVector& addRotation) override;
	virtual void AddComponentLocation(const FVector& addLocation) override;

	virtual void PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property) override;
	void SetSimulatePhysics(const bool IsSimulate, const bool ApplyForce = false);

	bool GetIsSimulatePhysics() const;
	
protected:

	virtual UniquePtr<CoreEngine::Physic::BodyInstance> CreateBodyInstance();
	virtual void RegisteredComponent() override;

private:

	void CallbackSimulation(CoreEngine::Physic::CallbackUpdateDataRB Data);
	void OnChangeSimulate(bool IsSimulate);

public:

	Dispatcher<bool> ChangeSimulatePhysics;

protected:

	UniquePtr<CoreEngine::Physic::BodyInstance> m_BodyInstance;

	RPROPERTY(EditorVisible);
	bool IsSimulatePhysics = false;

	RPROPERTY(EditorVisible);
	float Mass = 0.001f;
};
