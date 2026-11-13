#include <Physics/Include/BoxBodyInstance.h>
#include <btBulletCollisionCommon.h>
#include <Core/includes/Engine.h>
#include <Core/includes/World.h>
#include <Physics/Include/PhysRigidBody.h>

namespace CoreEngine::Physic
{
	BodyInstance::BodyInstance(const PhysDataSettings& InitDataSetttings)
	{
		Data.Data = InitDataSetttings;
		if (!IsValidMass(Data.Data.mass)) Data.Data.mass = 0.0f;
	}

	BoxBodyInstance::BoxBodyInstance(const PhysDataSettings& InitDataSetttings) : BodyInstance(InitDataSetttings)
	{
	}

	UniquePtr<btCollisionShape> CoreEngine::Physic::BoxBodyInstance::CreateShape()
	{
		World* world = Engine::Get()->GetWorld();
		CORE_ASSERT(world, "World doesn't exist")

		return world->GetPhysScene()->CreateBoxShape(BoxHalfExtent);
	}

	ETypeShape BoxBodyInstance::GetTypeShape() const
	{
		return ETypeShape::BOX;
	}

	void BoxBodyInstance::SetHalfExtent(const FVector& NewExtent)
	{

		BoxHalfExtent = NewExtent;

		auto& RigidBody = GetWorld()->GetPhysScene()->FindRigidBody(GetRigidHandle());

		if (!RigidBody) return;

		if (auto* BoxShape = static_cast<btBoxShape*>(RigidBody->GetCollisionShape()))
		{
			const btVector3 Target(BoxHalfExtent.GetX(), BoxHalfExtent.GetY(), BoxHalfExtent.GetZ());

			const btScalar Margin = btMin(btScalar(0.04), btScalar(0.1) * Target[Target.minAxis()]);

			BoxShape->btConvexInternalShape::setLocalScaling(btVector3(1, 1, 1));

			BoxShape->btConvexInternalShape::setMargin(Margin);
			BoxShape->setImplicitShapeDimensions(Target - btVector3(Margin, Margin, Margin));

			SetFlag(Data.ChangeData, EPhysDirtyData::MASS);
		}
	}

} // namespace CoreEngine::Physic
