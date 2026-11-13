#pragma once
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <btBulletCollisionCommon.h>
#include <Core/includes/Base.h>
#include <Math/includes/Matrix.h>
#include <Physics/Include/PhysData.h>
#include <Render/includes/Debug/PhysDebugDraw.h>

class btDiscreteDynamicsWorld;
class btBoxShape;

namespace CoreEngine::Render::Debug
{
	class PhysDebugDraw;
}

namespace CoreEngine
{
	namespace Physic
	{
		class PhysRigidBody;
		class BodyInstance;

		struct InitializeSceneData
		{
			UniquePtr<Render::Debug::PhysDebugDraw> Debugger;
		};

		class PhysScene
		{
		public:

			PhysScene();

		public:

			PhysBodyHandle CreateRigidBody(UniquePtr<btCollisionShape> Shape, const PhysDataSettings& data);
			UniquePtr<btBoxShape> CreateBoxShape(const FVector& HalfExtent);
			const UniquePtr<PhysRigidBody>& FindRigidBody(const PhysBodyHandle& Handle) const;
			void CreateScene(InitializeSceneData Init = InitializeSceneData());
			bool RegisterBody(BodyInstance* Body);

			void Simulate(float DeltaTime);

		private:

			uint64 GenerateId();

			void SetMassRigidBody(PhysRigidBody* RigidBody, const float Mass);

		private:

			HashTableMap<PhysBodyHandle, UniquePtr<PhysRigidBody>, PhysHandleHasher> SetBody;
			DArray<BodyInstance*> RegisterBodies;

			UniquePtr<btDiscreteDynamicsWorld> PhysWorld;
			btDefaultCollisionConfiguration config;
			btCollisionDispatcher dispatcher;
			btDbvtBroadphase broadphase;
			btSequentialImpulseConstraintSolver solver;
			btVector3 Gravity = btVector3(0, -9.81f, 0);

			UniquePtr<Render::Debug::PhysDebugDraw> DebugCollecter;

			static uint64 IdGenerator;
		};

	} // namespace Physic
} // namespace CoreEngine
