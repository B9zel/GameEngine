#include <Runtime/includes/CameraComponent.h>
#include <Math/includes/Math.h>


namespace CoreEngine
{
	namespace Runtime
	{

		CameraComponent::CameraComponent(const InitializeObject& Object) : SceneComponent(Object)
		{
			typeProjection = ETypeView::PERSPECTIVE;
			SetComponentRotation(FVector(0.0f, 0.0f, 0.0f));
		}
		void CameraComponent::InitProperties()
		{
			SceneComponent::InitProperties();

			viewMatrix = CreateMatrixLookAt(GetComponentLocation(), FVector(0, 0, 0), FVector(0, 1, 0));
		}

		void CameraComponent::SetComponentRotation(const FVector& Rotate)
		{
			SceneComponent::SetComponentRotation(Rotate);

			/*direction.SetX(cos(Math::ToRadian(Rotate.GetY())) * cos(Math::ToRadian(Rotate.GetX())));
			direction.SetX(sin(Math::ToRadian(Rotate.GetX())));
			direction.SetX(sin(Math::ToRadian(Rotate.GetY())) * cos(Math::ToRadian(Rotate.GetX())));*/

			direction.SetX(cos(Math::ToRadian(Rotate.GetY())) * cos(Math::ToRadian(Rotate.GetX())));
			direction.SetY(sin(Math::ToRadian(Rotate.GetX())));
			direction.SetZ(sin(Math::ToRadian(Rotate.GetY())) * cos(Math::ToRadian(Rotate.GetX())));

			direction.Normalize();
		}

		const ETypeView& CameraComponent::GetTypeView() const
		{
			return typeProjection;
		}

		FMatrix4x4 CameraComponent::GetViewMatrix()
		{
			viewMatrix = CreateMatrixLookAt(GetComponentLocation(), GetComponentLocation() + direction, FVector(0, 1, 0));
			return viewMatrix;
		}


	}
}