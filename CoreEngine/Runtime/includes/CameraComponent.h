#pragma once
#include <Runtime/includes/SceneComponent.h>
#include <Math/includes/Matrix.h>
#include <Runtime/includes/Enums/ViewUtil.h>
#include <CameraComponent.generated.h>

namespace CoreEngine
{
	namespace Runtime
	{


		RCLASS()
			class CameraComponent : public SceneComponent
		{
			GENERATED_BODY()

		public:

			CameraComponent(const InitializeObject& Object);

		public:

			virtual void InitProperties() override;
			virtual void SetComponentRotation(const FVector& Rotate) override;

			FMatrix4x4 GetViewMatrix();
			const ETypeView& GetTypeView() const;

		protected:

			FMatrix4x4 viewMatrix;
			FVector direction;
			ETypeView typeProjection;
		};
	}
}