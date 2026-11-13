#pragma once
#include <Core/includes/Base.h>
#include <Math/includes/Matrix.h>
#include <LinearMath/btVector3.h>
#include <cmath>

namespace CoreEngine::Physic
{
	inline bool IsValidMass(float Mass)
	{
		return std::isfinite(Mass) && Mass >= 0.0f;
	}

	FVector ConverBulletToCustomVector(const btVector3& Vec);

	struct PhysBodyHandle
	{
		friend class PhysScene;

	public:

		bool IsValid() const
		{
			return ID != 0;
		}
		void Invalide()
		{
			ID = 0;
		}
		uint64 GetId() const
		{
			return ID;
		}

	public:

		bool operator==(const PhysBodyHandle& Other) const
		{
			return ID == Other.ID;
		}

	private:

		uint64 ID = 0;
	};

	struct PhysHandleHasher
	{
		size_t operator()(const PhysBodyHandle& Handle) const
		{
			return std::hash<uint64>()(Handle.GetId());
		}
	};

	enum EPhysDirtyData : uint64
	{
		NONE_CHANGE = 0,
		SIMULATE = FLAG_OFFSET(1),
		TRANFORM = FLAG_OFFSET(2),
		VELOCITY = FLAG_OFFSET(3),
		MASS = FLAG_OFFSET(4),
	};

	enum EMotionType
	{
		STATIC = 0,
		DYNAMIC,
		KINEMATIC,
	};

	struct PhysDataSettings
	{
		bool IsSimulatePhysics = false;
		EMotionType Mobility = EMotionType::STATIC;
		FMatrix4x4 transform;
		FVector velocity = FVector::ZeroVector;
		float mass{0.0f};
	};

	struct PhysBodyUpdateData
	{
		PhysBodyHandle Handle;
		EPhysDirtyData ChangeData = EPhysDirtyData::NONE_CHANGE;
		PhysDataSettings Data;
	};
} // namespace CoreEngine::Physic
