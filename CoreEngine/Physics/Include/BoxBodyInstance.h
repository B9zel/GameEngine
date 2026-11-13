#pragma once
#include <Physics/Include/BodyInstance.h>

namespace CoreEngine::Physic
{
	class BoxBodyInstance : public BodyInstance
	{
	public:

		BoxBodyInstance(const PhysDataSettings& InitDataSetttings);

	public:

		virtual UniquePtr<btCollisionShape> CreateShape() override;
		virtual ETypeShape GetTypeShape() const override;

		virtual void SetHalfExtent(const FVector& NewExtent) override;

	private:

		FVector BoxHalfExtent = FVector(5.0f);
	};
} // namespace CoreEngine::Physic
