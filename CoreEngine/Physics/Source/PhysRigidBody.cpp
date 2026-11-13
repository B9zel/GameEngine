#include <Physics/Include/PhysRigidBody.h>
#include <btBulletDynamicsCommon.h>
#include <Core/includes/Base.h>
#include <Math/includes/Matrix.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

namespace CoreEngine::Physic
{

	PhysRigidBody::PhysRigidBody(const PhysInitializationRigidBodySettings& settings)
	{
		m_RigidBody = MakeUniquePtr<btRigidBody>(0, nullptr, settings.CollisionShape.get());
		m_CollisionShape = std::move(settings.CollisionShape);

		SetSettings(settings.Data);
	}

	void PhysRigidBody::SetSettings(const PhysDataSettings& data)
	{
		btTransform bulletTransform;
		bulletTransform.setFromOpenGLMatrix(Math::GetValuePtr(data.transform));
		GetRigidBody()->setWorldTransform(bulletTransform);

		GetRigidBody()->setLinearVelocity(btVector3(data.velocity.GetX(), data.velocity.GetY(), data.velocity.GetZ()));

		SetMass(data.mass);

		GetRigidBody()->setCollisionFlags(data.Mobility == EMotionType::STATIC ? btCollisionObject::CF_STATIC_OBJECT : 0);

		SetSimulatePhysics(data.IsSimulatePhysics);
	}

	void PhysRigidBody::SetMass(const float Mass)
	{
		if (!IsValidMass(Mass)) return;

		m_Mass = Mass;
		// Keep the requested mass for when simulation is enabled again.
		if (GetRigidBody()->isKinematicObject()) return;

		btVector3 inertia(0, 0, 0);
		if (Mass > 0.0f)
		{
			m_CollisionShape->calculateLocalInertia(Mass, inertia);
		}
		GetRigidBody()->setMassProps(Mass, inertia);
		GetRigidBody()->updateInertiaTensor();
		if (Mass > 0.0f)
		{
			GetRigidBody()->activate(true);
		}
	}

	void PhysRigidBody::SetTransform(const FMatrix4x4 transform)
	{
		btTransform bulletTransform;
		bulletTransform.setFromOpenGLMatrix(Math::GetValuePtr(transform));
		GetRigidBody()->setWorldTransform(bulletTransform);
	}
	void PhysRigidBody::SetVelocity(const FVector velocity)
	{
		GetRigidBody()->setLinearVelocity(btVector3(velocity.GetX(), velocity.GetY(), velocity.GetZ()));
	}

	void PhysRigidBody::SetSimulatePhysics(const bool IsSimulate)
	{
		int Flags = GetRigidBody()->getCollisionFlags();

		if (IsSimulate)
		{
			Flags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
			Flags &= ~btCollisionObject::CF_STATIC_OBJECT;

			GetRigidBody()->setCollisionFlags(Flags);

			SetMass(m_Mass);

			GetRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
			GetRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
			GetRigidBody()->clearForces();

			GetRigidBody()->forceActivationState(ACTIVE_TAG);
		}
		else
		{
			GetRigidBody()->setMassProps(0.0f, btVector3(0, 0, 0));
			GetRigidBody()->updateInertiaTensor();
			Flags &= ~btCollisionObject::CF_STATIC_OBJECT;
			Flags |= btCollisionObject::CF_KINEMATIC_OBJECT;

			GetRigidBody()->setCollisionFlags(Flags);
			GetRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
			GetRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
			GetRigidBody()->clearForces();

			GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
		}

		GetRigidBody()->activate(true);
	}

	const UniquePtr<btRigidBody>& PhysRigidBody::GetRigidBody() const
	{
		return m_RigidBody;
	}

	btCollisionShape* PhysRigidBody::GetCollisionShape() const
	{
		return m_CollisionShape.get();
	}

} // namespace CoreEngine::Physic
