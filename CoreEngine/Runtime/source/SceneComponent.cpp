#include <Runtime/includes/SceneComponent.h>
#include <Math/includes/Matrix.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Runtime/includes/Actor.h>
#include <cmath>
// #include <glm/gtc/quaternion.hpp>

namespace
{
	FVector GetWorldScale(const SceneComponent* component)
	{
		FVector worldScale = component->GetComponentScale();
		for (const SceneComponent* parent = component->GetParentAttach(); parent; parent = parent->GetParentAttach())
		{
			worldScale *= parent->GetComponentScale();
		}
		return worldScale;
	}

	float GetRelativeScaleAxis(float worldScale, float parentWorldScale)
	{
		constexpr float MinScale = 0.000001f;
		return std::abs(parentWorldScale) > MinScale ? worldScale / parentWorldScale : 0.0f;
	}

	FVector GetRelativeScale(const FVector& worldScale, const FVector& parentWorldScale)
	{
		return FVector(GetRelativeScaleAxis(worldScale.GetX(), parentWorldScale.GetX()), GetRelativeScaleAxis(worldScale.GetY(), parentWorldScale.GetY()),
					   GetRelativeScaleAxis(worldScale.GetZ(), parentWorldScale.GetZ()));
	}
} // namespace

SceneComponent::SceneComponent(const CoreEngine::InitializeObject& Object) : ActorComponent(Object)
{
	Transform.SetLocation(FVector(0, 0, 0));
	Transform.SetScale(FVector(1, 1, 1));
	Transform.SetRotation(FVector(0, 0, 0));
	Front = FVector(0, 0, -1);

	parentAttach = nullptr;
}

void SceneComponent::DestroyComponent()
{
	if (parentAttach)
	{
		SceneComponent* targetParent = parentAttach;
		while (!childrenAttach.empty())
		{
			SceneComponent* child = childrenAttach.front();
			if (!child)
			{
				childrenAttach.erase(childrenAttach.begin());
				continue;
			}

			child->SetupToAttachment(targetParent);
		}

		auto& siblings = targetParent->childrenAttach;
		const auto selfIt = std::find(siblings.begin(), siblings.end(), this);
		if (selfIt != siblings.end())
		{
			siblings.erase(selfIt);
		}

		parentAttach = nullptr;
		ActorComponent::DestroyComponent();
		return;
	}

	SceneComponent* NextRoot = childrenAttach.empty() ? nullptr : childrenAttach.front();
	if (NextRoot)
	{
		childrenAttach.erase(childrenAttach.begin());
		NextRoot->parentAttach = nullptr;

		while (!childrenAttach.empty())
		{
			SceneComponent* child = childrenAttach.front();
			if (!child)
			{
				childrenAttach.erase(childrenAttach.begin());
				continue;
			}

			child->SetupToAttachment(NextRoot);
		}

		GetOwner()->SetRootComponent(NextRoot);
	}
	else if (GetOwner() && GetOwner()->GetRootComponent() == this)
	{
		GetOwner()->SetRootComponent(nullptr);
	}

	ActorComponent::DestroyComponent();
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
		return parentAttach->GetReletiveLocation() - GetComponentLocation();
	}
	return FVector::ZeroVector;
}

FVector SceneComponent::GetComponentScale() const
{
	return Transform.GetScale();
}

FVector SceneComponent::GetReletiveScale() const
{
	if (parentAttach)
	{
		return parentAttach->GetReletiveScale() - GetComponentScale();
	}
	return FVector::ZeroVector;
}

FVector SceneComponent::GetComponentRotation() const
{
	return Transform.GetRotation();
}

FVector SceneComponent::GetReletiveRotation() const
{
	if (parentAttach)
	{
		return parentAttach->GetReletiveRotation() - GetComponentRotation();
	}
	return FVector::ZeroVector;
}

FVector SceneComponent::GetForwardVector() const
{
	return CalculateForwardDirection(Transform.GetRotation());
}

FVector SceneComponent::GetRightVector() const
{
	return CalculateRightDirection();
}

void SceneComponent::SetComponentRotation(const FVector& newRotation)
{
	// AddComponentRotation(newRotation - transform.GetRotationRef());

	/*for (auto& Child : childrenAttach)
	{
		Child->SetComponentRotation(newRotation - Transform.GetRotation() + Child->GetComponentRotation());
	}*/
	Transform.SetRotation(newRotation);
}

void SceneComponent::SetComponentLocation(const FVector& newLocation)
{
	/*for (auto& Child : childrenAttach)
	{
		Child->SetComponentLocation(newLocation - Transform.GetLocationRef() + Child->GetComponentLocation());
	}*/
	Transform.SetLocation(newLocation);
}

void SceneComponent::SetComponentScale(const FVector& newScale)
{
	/*for (auto& Child : childrenAttach)
	{
		Child->SetComponentScale((Child->GetComponentScale() / Child->parentAttach->GetComponentScale()) * newScale);
	}*/
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
	if (!attach || attach == this) return;
	if (attach == parentAttach) return;

	for (SceneComponent* ancestor = attach; ancestor; ancestor = ancestor->parentAttach)
	{
		if (ancestor == this) return;
	}

	const FVector worldScale = GetWorldScale(this);

	if (parentAttach)
	{
		auto& arrChildren = parentAttach->childrenAttach;
		const auto selfIt = std::find(arrChildren.begin(), arrChildren.end(), this);
		if (selfIt != arrChildren.end())
		{
			arrChildren.erase(selfIt);
		}
	}

	if (parentAttach)
	{
		SetComponentLocation(attach->GetReletiveLocation() - GetReletiveLocation());
		SetComponentRotation(attach->GetReletiveRotation() - GetReletiveRotation());
	}

	parentAttach = attach;
	SetComponentScale(GetRelativeScale(worldScale, GetWorldScale(attach)));
	if (std::find(attach->childrenAttach.begin(), attach->childrenAttach.end(), this) == attach->childrenAttach.end())
	{
		attach->childrenAttach.push_back(this);
	}
}

const DArray<SceneComponent*>& SceneComponent::GetChildrenAttaches() const
{
	return childrenAttach;
}

SceneComponent* SceneComponent::GetParentAttach() const
{
	return parentAttach;
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
FVector SceneComponent::CalculateForwardDirection(const FVector& VedDirect, const bool IsConvertToRadian) const
{
	FVector direction(0, 0, 0);

	FVector RotInput = VedDirect;
	RotInput = IsConvertToRadian ? Math::ToRadianVector(RotInput) : RotInput;

	FMatrix4x4 Mat;
	Mat = glm::eulerAngleXYZ(RotInput.GetX(), RotInput.GetY(), RotInput.GetZ());

	glm::quat q = glm::quat_cast(Mat);
	glm::mat4 ResMat = glm::toMat4(q);
	FVector4 fw = ResMat * FVector4(FVector::ForwardVector.vector, 0.0f).vector;
	direction = FVector(fw.vector).SafeNormalize();
	// direction.SetX(sin(Math::ToRadian(Rotation.GetY())) * cos(Math::ToRadian(Rotation.GetX())));
	// direction.SetY(sin(Math::ToRadian(Rotation.GetX())));
	// direction.SetZ(cos(Math::ToRadian(Rotation.GetY())) * cos(Math::ToRadian(Rotation.GetX())));
	// direction = glm::normalize(FVector::ForwardVector.vector * (glm::quat(Math::ToRadianVector(direction).vector)));
	/*direction.SetX(cos(Pitch) * cos(-Yaw));
	direction.SetY(sin(Pitch));
	direction.SetZ(cos(Pitch) * sin(-Yaw));*/

	// direction.SetZ(-direction.GetZ());
	// direction.Normalize();

	return direction;
}

FVector SceneComponent::CalculateRightDirection() const
{
	return FVector::UpVector.Cross(GetForwardVector()).SafeNormalize();
}

FMatrix4x4 SceneComponent::MakeMatrixMesh() const
{

	return MakeParentMatrix() * GetTransform().ToMatrix();
}
FMatrix4x4 SceneComponent::MakeParentMatrix() const
{
	FMatrix4x4 ResultMat(1);
	SceneComponent* Parent = GetParentAttach();
	while (Parent)
	{
		ResultMat = Parent->GetTransform().ToMatrix() * ResultMat;
		Parent = Parent->GetParentAttach();
	}
	return ResultMat;
}
