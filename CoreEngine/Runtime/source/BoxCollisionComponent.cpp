#include <Runtime/includes/BoxCollisionComponent.h>
#include <Physics/Include/BoxBodyInstance.h>
#include <Physics/Include/PhysData.h>

BoxCollisionComponent::BoxCollisionComponent(const CoreEngine::InitializeObject& Object) : CollisionComponent(Object)
{
}

void BoxCollisionComponent::PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property)
{
	CollisionComponent::PostEditChangeProperty(Property);

	if (Property.Name == STRINGCON_DETAILS(BoxHalfExtent))
	{
		if (auto BoxBody = dynamic_cast<CoreEngine::Physic::BoxBodyInstance*>(m_BodyInstance.get()))
		{
			BoxBody->SetHalfExtent(BoxHalfExtent);
		}
	}
}

UniquePtr<CoreEngine::Physic::BodyInstance> BoxCollisionComponent::CreateBodyInstance()
{
	CoreEngine::Physic::PhysDataSettings Data;
	Data.IsSimulatePhysics = IsSimulatePhysics;
	Data.mass = Mass;
	Data.Mobility = CoreEngine::Physic::EMotionType::DYNAMIC;
	Data.transform = GetTransform().ToMatrix();
	Data.velocity = GetVelocity();

	auto Body = MakeUniquePtr<CoreEngine::Physic::BoxBodyInstance>(Data);
	Body->SetHalfExtent(BoxHalfExtent);

	return std::move(Body);
}
