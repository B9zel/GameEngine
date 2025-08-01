#include <Runtime/includes/SceneComponent.h>




CoreEngine::Runtime::SceneComponent::SceneComponent()
{
	transform.SetLocation(FVector(0, 0, 0));
	transform.SetScale(FVector(1, 1, 1));
	transform.SetRotation(FVector(0, 0, 0));
	Front = FVector(0, 0, -1);

	parentAttach = nullptr;
}

const Transform& CoreEngine::Runtime::SceneComponent::GetTransform() const
{
	return transform;
}

void CoreEngine::Runtime::SceneComponent::SetTransform(const Transform& newTransform)
{
	transform = newTransform;
}

FVector CoreEngine::Runtime::SceneComponent::GetComponentLocation() const
{
	return transform.GetLocation();
}

FVector CoreEngine::Runtime::SceneComponent::GetComponentScale() const
{
	return transform.GetScale();
}

FVector CoreEngine::Runtime::SceneComponent::GetComponentRotation() const
{
	return transform.GetRotation();
}

FVector CoreEngine::Runtime::SceneComponent::GetForwardVector()
{
	return CalculateForwardDirection();
}

FVector CoreEngine::Runtime::SceneComponent::GetRightVector()
{
	return CalculateRightDirection();
}

void CoreEngine::Runtime::SceneComponent::SetComponentRotation(const FVector& newRotation)
{
	for (auto& Child : childrenAttach)
	{
		Child->SetComponentRotation(newRotation - Child->GetComponentRotation());
	}
	transform.SetRotation(newRotation);
}

void CoreEngine::Runtime::SceneComponent::SetComponentLocation(const FVector& newLocation)
{
	for (auto& Child : childrenAttach)
	{
		Child->SetComponentLocation(newLocation - Child->GetComponentLocation());
	}
	transform.SetLocation(newLocation);
}

void CoreEngine::Runtime::SceneComponent::SetComponentScale(const FVector& newScale)
{
	for (auto& Child : childrenAttach)
	{
		Child->SetComponentScale((Child->GetComponentScale() / Child->parentAttach->GetComponentScale()) * newScale);
	}
	transform.SetScale(newScale);
}

void CoreEngine::Runtime::SceneComponent::AddComponentRotation(const FVector& addRotation)
{
	for (auto& Child : childrenAttach)
	{
		Child->AddComponentRotation(addRotation);
	}
	transform.SetRotation(transform.GetRotation() + addRotation);
}

void CoreEngine::Runtime::SceneComponent::AddComponentLocation(const FVector& addLocation)
{
	for (auto& Child : childrenAttach)
	{
		Child->AddComponentLocation(addLocation);
	}
	transform.SetLocation(transform.GetLocation() + addLocation);
}

void CoreEngine::Runtime::SceneComponent::AddComponentScale(const FVector& addScale)
{
	for (auto& Child : childrenAttach)
	{
		Child->AddComponentScale(addScale);
	}
	transform.SetScale(transform.GetScale() + addScale);
}

void CoreEngine::Runtime::SceneComponent::SetupToAttachment(SceneComponent* attach)
{
	if (parentAttach)
	{
		auto& arrChildren = parentAttach->childrenAttach;
		arrChildren.erase(std::find(arrChildren.begin(), arrChildren.end(), this));
	}

	parentAttach = attach;
	attach->childrenAttach.push_back(this);
}

FVector CoreEngine::Runtime::SceneComponent::CalculateForwardDirection()
{
	const FVector& Rotation = transform.GetRotation();
	FVector direction(0, 0, 0);

	direction.SetZ(cos(Math::ToRadian(Rotation.GetY())) * cos(Math::ToRadian(Rotation.GetX())));
	direction.SetY(sin(Math::ToRadian(Rotation.GetX())));
	direction.SetX(sin(Math::ToRadian(Rotation.GetY())) * cos(Math::ToRadian(Rotation.GetX())));

	direction.SetZ(-direction.GetZ());
	direction.Normalize();

	return direction;
}

FVector CoreEngine::Runtime::SceneComponent::CalculateRightDirection()
{
	return FVector::UpVector.Cross(GetForwardVector()).SafeNormalize();
}
