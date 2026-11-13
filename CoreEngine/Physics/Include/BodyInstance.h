#pragma once
#include <Physics/Include/PhysScene.h>

class World;	

namespace CoreEngine::Physic
{
	enum ETypeShape
	{
		BOX = 0,
	};

	struct CallbackUpdateDataRB
	{
		FVector Location;
		FVector Rotation;
		FMatrix4x4 transform;
		FVector Velocity;
	};

	class BodyInstance
	{
	public:

		BodyInstance(const PhysDataSettings& InitDataSetttings);

	public:

		virtual UniquePtr<btCollisionShape> CreateShape() = 0;
		virtual ETypeShape GetTypeShape() const = 0;
		virtual void RegistPhysBody();
		virtual void UpdateSimulation(PhysRigidBody* RigidBody);

		virtual void SetHalfExtent(const FVector& NewExtent);

		void SetSimulate(const bool IsSimulate);
		void SetLocation(const FVector& newLocation);
		void SetRotation(const FVector& newRotation);
		void SetMass(const float mass);

		const PhysDataSettings& GetDataSettings() const;
		PhysBodyUpdateData CollectPacketUpdateData();

		void SetRigidHandle(const PhysBodyHandle& NewHandle);
		PhysBodyHandle GetRigidHandle() const;

		void ClearDirt();
		bool IsDirtData() const;

	protected:

		World* GetWorld() const;

	public:

		Dispatcher<CallbackUpdateDataRB> OnCallbackSimulation;

	protected:

		PhysBodyHandle RigidHandle;
		PhysBodyUpdateData Data;
	};
} // namespace CoreEngine::Physic
