#include <Physics/Include/PhysScene.h>
#include <btBulletDynamicsCommon.h>
#include <Physics/Include/PhysRigidBody.h>
#include <Physics/Include/BodyInstance.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

namespace CoreEngine::Physic
{
	uint64 PhysScene::IdGenerator = 1;

	DECLARE_LOG_CATEGORY_EXTERN(PhysSceneLog)

	void CoreEngine::Physic::PhysScene::CreateScene(InitializeSceneData Init)
	{
		dispatcher.setCollisionConfiguration(&config);

		PhysWorld = MakeUniquePtr<btDiscreteDynamicsWorld>(&dispatcher, &broadphase, &solver, &config);
		PhysWorld->setGravity(Gravity);

		DebugCollecter = std::move(Init.Debugger);
		PhysWorld->setDebugDrawer(DebugCollecter.get());
	}

	PhysScene::PhysScene() : dispatcher(&config)
	{
	}

	PhysBodyHandle PhysScene::CreateRigidBody(UniquePtr<btCollisionShape> Shape, const PhysDataSettings& data)
	{
		PhysBodyHandle newHandle;
		newHandle.ID = GenerateId();

		PhysInitializationRigidBodySettings Init;
		Init.CollisionShape = std::move(Shape);
		Init.Data = data;

		auto newRigidBody = MakeUniquePtr<PhysRigidBody>(Init);

		PhysWorld->addRigidBody(newRigidBody->GetRigidBody().get());
		SetBody.emplace(newHandle, std::move(newRigidBody));

		return newHandle;
	}

	bool PhysScene::RegisterBody(BodyInstance* Body)
	{
		if (!Body) return false;

		auto It = std::find_if(RegisterBodies.begin(), RegisterBodies.end(), [&Body](BodyInstance* El) { return El == Body; });
		if (It != RegisterBodies.end())
		{
			return false;
		}
		RegisterBodies.push_back(Body);

		return true;
	}

	UniquePtr<btBoxShape> PhysScene::CreateBoxShape(const FVector& HalfExtent)
	{
		return MakeUniquePtr<btBoxShape>(btVector3(HalfExtent.GetX(), HalfExtent.GetY(), HalfExtent.GetZ()));
	}

	void PhysScene::Simulate(float DeltaTime)
	{
		DArray<PhysBodyUpdateData> UpdatedData;
		for (BodyInstance* Instance : RegisterBodies)
		{
			if (Instance->IsDirtData())
			{
				UpdatedData.push_back(Instance->CollectPacketUpdateData());
			}
		}

		for (auto& Data : UpdatedData)
		{
			const UniquePtr<PhysRigidBody>& RigidBody = FindRigidBody(Data.Handle);
			if (!RigidBody) continue;

			btRigidBody* BulletBody = RigidBody->GetRigidBody().get();
			const bool ChangeSimulation = HasFlag(Data.ChangeData, EPhysDirtyData::SIMULATE);
			const bool ChangeMass = HasFlag(Data.ChangeData, EPhysDirtyData::MASS);
			const bool ChangeStaticState = ChangeMass && IsValidMass(Data.Data.mass) && !BulletBody->isKinematicObject() &&
									   (BulletBody->isStaticObject() != (Data.Data.mass == 0.0f));
			const bool ReRegister = ChangeSimulation || ChangeStaticState;
			if (ReRegister)
			{
				PhysWorld->removeRigidBody(BulletBody);
			}

			if (HasFlag(Data.ChangeData, EPhysDirtyData::TRANFORM))
			{
				RigidBody->SetTransform(Data.Data.transform);
			}
			if (ChangeMass)
			{
				// World-space inertia must use this packet's final orientation.
				RigidBody->SetMass(Data.Data.mass);
			}
			if (ChangeSimulation)
			{
				RigidBody->SetSimulatePhysics(Data.Data.IsSimulatePhysics);
			}
			if (HasFlag(Data.ChangeData, EPhysDirtyData::VELOCITY))
			{
				RigidBody->SetVelocity(Data.Data.velocity);
			}
			if (ReRegister)
			{
				// Refresh gravity, simulation membership and collision filters only
				// when the motion type changes, including transitions through zero mass.
				PhysWorld->addRigidBody(BulletBody);
			}
		}

		PhysWorld->stepSimulation(DeltaTime, 1);

		for (BodyInstance* Instance : RegisterBodies)
		{
			if (!Instance->GetDataSettings().IsSimulatePhysics) continue;

			const UniquePtr<PhysRigidBody>& RigidBody = FindRigidBody(Instance->GetRigidHandle());
			if (!RigidBody) continue;
			Instance->UpdateSimulation(RigidBody.get());
		}

		PhysWorld->debugDrawWorld();
	}

	uint64 PhysScene::GenerateId()
	{
		return IdGenerator++;
	}

	const UniquePtr<PhysRigidBody>& PhysScene::FindRigidBody(const PhysBodyHandle& Handle) const
	{
		auto It = SetBody.find(Handle);
		if (It == SetBody.end())
		{
			EG_LOG(PhysSceneLog, ELevelLog::ERROR, "{} handle doesn't finded, please ensure that the rigid body is registered.");
			return nullptr;
		}

		return It->second;
	}
	void PhysScene::SetMassRigidBody(PhysRigidBody* RigidBody, const float Mass)
	{
		if (RigidBody) return;
	}
} // namespace CoreEngine::Physic
