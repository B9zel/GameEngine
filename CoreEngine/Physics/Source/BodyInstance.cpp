#include <Physics/Include/BodyInstance.h>
#include <Physics/Include/PhysScene.h>
#include <Core/includes/Engine.h>
#include <Core/includes/World.h>
#include <Physics/Include/PhysRigidBody.h>

namespace CoreEngine::Physic
{
	void BodyInstance::SetRigidHandle(const PhysBodyHandle& NewHandle)
	{
		RigidHandle = NewHandle;
		Data.Handle = RigidHandle;
	}

	PhysBodyHandle BodyInstance::GetRigidHandle() const
	{
		return RigidHandle;
	}

	void BodyInstance::ClearDirt()
	{
		Data.ChangeData = EPhysDirtyData::NONE_CHANGE;
	}

	bool BodyInstance::IsDirtData() const
	{
		return Data.ChangeData != EPhysDirtyData::NONE_CHANGE;
	}

	void BodyInstance::RegistPhysBody()
	{
		World* world = GetWorld();

		world->GetPhysScene()->RegisterBody(this);
		SetRigidHandle(world->GetPhysScene()->CreateRigidBody(CreateShape(), Data.Data));
	}

	void BodyInstance::UpdateSimulation(PhysRigidBody* RigidBody)
	{
		const btTransform& transform = RigidBody->GetRigidBody()->getWorldTransform();
		transform.getOpenGLMatrix(Math::GetValuePtr(Data.Data.transform));

		CallbackUpdateDataRB callback;
		callback.Location = ConverBulletToCustomVector(transform.getOrigin());
		transform.getRotation().getEulerZYX(callback.Rotation.vector.z, callback.Rotation.vector.y, callback.Rotation.vector.x);

		callback.transform = Data.Data.transform;
		callback.Velocity = ConverBulletToCustomVector(RigidBody->GetRigidBody()->getLinearVelocity());

		OnCallbackSimulation.Call(std::forward<CallbackUpdateDataRB>(callback));
	}

	void BodyInstance::SetSimulate(const bool IsSimulate)
	{
		Data.Data.IsSimulatePhysics = IsSimulate;
		SetFlag(Data.ChangeData, EPhysDirtyData::SIMULATE);
	}

	void BodyInstance::SetHalfExtent(const FVector& NewExtent)
	{
		throw std::runtime_error("SetHalfExtent must be override");
	}

	const PhysDataSettings& BodyInstance::GetDataSettings() const
	{
		return Data.Data;
	}

	PhysBodyUpdateData BodyInstance::CollectPacketUpdateData()
	{
		PhysBodyUpdateData pocket = Data;
		ClearDirt();
		return pocket;
	}
	void BodyInstance::SetLocation(const FVector& newLocation)
	{
		FVector Location;
		Math::DecomposeLocationMatrix(Data.Data.transform, Location);
		if (Location == newLocation) return;

		Data.Data.transform = Math::SetMatrixTranslation(Data.Data.transform, newLocation);

		SetFlag(Data.ChangeData, EPhysDirtyData::TRANFORM);
	}
	void BodyInstance::SetRotation(const FVector& newRotation)
	{
		Data.Data.transform = Math::SetMatrixRotation(Data.Data.transform, newRotation);
		SetFlag(Data.ChangeData, EPhysDirtyData::TRANFORM);
	}

	void BodyInstance::SetMass(const float Mass)
	{
		if (!IsValidMass(Mass)) return;

		Data.Data.mass = Mass;
		SetFlag(Data.ChangeData, EPhysDirtyData::MASS);
	}

	World* BodyInstance::GetWorld() const
	{
		World* world = Engine::Get()->GetWorld();
		CORE_ASSERT(world, "World doesn't exist")

		return world;
	}

} // namespace CoreEngine::Physic
