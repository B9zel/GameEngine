#include <Runtime/includes/SceneComponent.h>
#include <Math/includes/Matrix.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
//#include <glm/gtc/quaternion.hpp>

namespace CoreEngine
{
	namespace Runtime
	{

		SceneComponent::SceneComponent(const InitializeObject& Object) : ActorComponent(Object)
		{
			Transform.SetLocation(FVector(0, 0, 0));
			Transform.SetScale(FVector(1, 1, 1));
			Transform.SetRotation(FVector(0, 0, 0));
			Front = FVector(0, 0, -1);

			parentAttach = nullptr;
		}

		const FTransform& SceneComponent::GetTransform() const
		{
			return Transform;
		}

		void SceneComponent::SetTransform(const FTransform& newTransform)
		{
			if (Transform == newTransform) return;

			SetComponentLocation(newTransform.GetLocation());
			SetComponentRotation(newTransform.GetRotation());
			SetComponentScale(newTransform.GetScale());
		}

		FVector SceneComponent::GetComponentLocation() const
		{
			return Transform.GetLocation();
		}

		FVector SceneComponent::GetReletiveLocation() const
		{
			if (parentAttach)
			{
				return parentAttach->GetComponentLocation() - GetComponentLocation();
			}
			return GetComponentLocation();
		}


		FVector SceneComponent::GetComponentScale() const
		{
			return Transform.GetScale();
		}

		FVector SceneComponent::GetComponentRotation() const
		{
			return Transform.GetRotation();
		}

		FVector SceneComponent::GetForwardVector() const
		{
			return CalculateForwardDirection();
		}

		FVector SceneComponent::GetRightVector() const
		{
			return CalculateRightDirection();
		}

		void SceneComponent::SetComponentRotation(const FVector& newRotation)
		{
			//AddComponentRotation(newRotation - transform.GetRotationRef());

			for (auto& Child : childrenAttach)
			{
				Child->SetComponentRotation(newRotation - Transform.GetRotation() + Child->GetComponentRotation());
			}
			Transform.SetRotation(newRotation);
		}

		void SceneComponent::SetComponentLocation(const FVector& newLocation)
		{
			for (auto& Child : childrenAttach)
			{
				Child->SetComponentLocation(newLocation - Transform.GetLocationRef() + Child->GetComponentLocation());
			}
			Transform.SetLocation(newLocation);
		}

		void SceneComponent::SetComponentScale(const FVector& newScale)
		{

			for (auto& Child : childrenAttach)
			{
				Child->SetComponentScale((Child->GetComponentScale() / Child->parentAttach->GetComponentScale()) * newScale);
			}
			Transform.SetScale(newScale);
		}

		void SceneComponent::AddComponentRotation(const FVector& addRotation)
		{
			for (auto& Child : childrenAttach)
			{
				Child->AddComponentRotation(addRotation);
			}
			Transform.SetRotation(Transform.GetRotation() + addRotation);
		}

		void SceneComponent::AddComponentLocation(const FVector& addLocation)
		{
			for (auto& Child : childrenAttach)
			{
				Child->AddComponentLocation(addLocation);
			}
			Transform.SetLocation(Transform.GetLocation() + addLocation);
		}

		void SceneComponent::AddComponentScale(const FVector& addScale)
		{
			for (auto& Child : childrenAttach)
			{
				Child->AddComponentScale(addScale);
			}
			Transform.SetScale(Transform.GetScale() + addScale);
		}

		void SceneComponent::SetupToAttachment(SceneComponent* attach)
		{
			if (parentAttach)
			{
				auto& arrChildren = parentAttach->childrenAttach;
				arrChildren.erase(std::find(arrChildren.begin(), arrChildren.end(), this));
			}

			parentAttach = attach;
			attach->childrenAttach.push_back(this);
		}

		const DArray<SceneComponent*>& SceneComponent::GetChildrenAttaches() const
		{
			return childrenAttach;
		}

		static float NormalizeDeg(float a)
		{
			return a;
			float x = std::fmod(a + 180.0f, 360.0f);
			if (x < 0.0f) x += 360.0f;
			return x - 180.0f;
		}

		static FVector NormalizeDegVec3(const FVector& v)
		{
			return FVector(NormalizeDeg(v.GetX()), NormalizeDeg(v.GetY()), NormalizeDeg(v.GetZ()));
		}
		FVector SceneComponent::CalculateForwardDirection() const
		{
			const FVector& Rotation = (Transform.GetRotation());
			FVector direction(0, 0, 0);

			FVector RotInput = Rotation;
				RotInput = Math::ToRadianVector(RotInput);
			
			FMatrix4x4 Mat;
			if (!Math::LikelyRadians(Rotation))
			{

				//Mat = glm::toMat4( glm::normalize(glm::quat(RotInput.vector)));
			}
			else
			{

			}
			Mat = glm::eulerAngleXYZ(RotInput.GetX(), RotInput.GetY(), RotInput.GetZ());

			glm::quat q = glm::quat_cast(Mat);
			glm::mat4 ResMat = glm::toMat4(q);
			FVector4 fw = ResMat * FVector4(FVector::ForwardVector.vector, 0.0f).vector;
			direction = FVector(fw.vector).SafeNormalize();
			//direction.SetX(sin(Math::ToRadian(Rotation.GetY())) * cos(Math::ToRadian(Rotation.GetX())));
			//direction.SetY(sin(Math::ToRadian(Rotation.GetX())));
			//direction.SetZ(cos(Math::ToRadian(Rotation.GetY())) * cos(Math::ToRadian(Rotation.GetX())));
			//direction = glm::normalize(FVector::ForwardVector.vector * (glm::quat(Math::ToRadianVector(direction).vector)));
			/*direction.SetX(cos(Pitch) * cos(-Yaw));
			direction.SetY(sin(Pitch));
			direction.SetZ(cos(Pitch) * sin(-Yaw));*/

			//direction.SetZ(-direction.GetZ());
			//direction.Normalize();

			return direction;
		}

		FVector SceneComponent::CalculateRightDirection() const
		{
			return FVector::UpVector.Cross(GetForwardVector()).SafeNormalize();
		}
	}
}