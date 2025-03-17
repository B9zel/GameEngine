#pragma once
#include <Runtime/includes/SceneComponent.h>
#include <Math/includes/Matrix.h>


namespace CoreEngine
{
	namespace Runtime
	{
		enum class ETypeView : uint8
		{
			PERSPECTIVE = 0,
			ORTHOGRAPH
		};


		class CameraComponent : public SceneComponent
		{
		public:

			CameraComponent();
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