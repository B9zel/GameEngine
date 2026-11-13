#pragma once
#include <Physics/Include/PhysData.h>
#include <Core/includes/Base.h>

class btRigidBody;
class btCollisionShape;

namespace CoreEngine
{
	namespace Physic
	{
		struct PhysInitializationRigidBodySettings
		{
			mutable UniquePtr<btCollisionShape> CollisionShape;
			PhysDataSettings Data;
		};

		class PhysRigidBody
		{
		public:

			PhysRigidBody(const PhysInitializationRigidBodySettings& settings);

		public:

			const UniquePtr<btRigidBody>& GetRigidBody() const;
			btCollisionShape* GetCollisionShape() const;

			void SetSettings(const PhysDataSettings& data);

			void SetMass(const float Mass);
			void SetTransform(const FMatrix4x4 transform);
			void SetVelocity(const FVector velocity);
			void SetSimulatePhysics(const bool IsSimulate);

			

		private:

			UniquePtr<btRigidBody> m_RigidBody;
			UniquePtr<btCollisionShape> m_CollisionShape;

			float m_Mass = 0.0f;
		};
	} // namespace Physic
} // namespace CoreEngine
