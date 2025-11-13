#include <Runtime/includes/SceneComponent.h>




CoreEngine::Runtime::SceneComponent::SceneComponent(const InitializeObject& Object) : ActorComponent(Object)
{
	Transform.SetLocation(FVector(0, 0, 0));
	Transform.SetScale(FVector(1, 1, 1));
	Transform.SetRotation(FVector(0, 0, 0));
	Front = FVector(0, 0, -1);

	parentAttach = nullptr;
}

const FTransform& CoreEngine::Runtime::SceneComponent::GetTransform() const
{
	return Transform;
}

void CoreEngine::Runtime::SceneComponent::SetTransform(const FTransform& newTransform)
{
	if (Transform == newTransform) return;

	SetComponentLocation(newTransform.GetLocation());
	SetComponentRotation(newTransform.GetRotation());
	SetComponentScale(newTransform.GetScale());
}

FVector CoreEngine::Runtime::SceneComponent::GetComponentLocation() const
{
	return Transform.GetLocation();
}

FVector CoreEngine::Runtime::SceneComponent::GetComponentScale() const
{
	return Transform.GetScale();
}

FVector CoreEngine::Runtime::SceneComponent::GetComponentRotation() const
{
	return Transform.GetRotation();
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
	//AddComponentRotation(newRotation - transform.GetRotationRef());
	for (auto& Child : childrenAttach)
	{
		Child->SetComponentRotation(newRotation - Transform.GetRotation() + Child->GetComponentRotation());
	}
	Transform.SetRotation(newRotation);
}

void CoreEngine::Runtime::SceneComponent::SetComponentLocation(const FVector& newLocation)
{
	for (auto& Child : childrenAttach)
	{	
		Child->SetComponentLocation(newLocation - Transform.GetLocation() + Child->GetComponentLocation());
	}
	Transform.SetLocation(newLocation);
}

void CoreEngine::Runtime::SceneComponent::SetComponentScale(const FVector& newScale)
{
	
	for (auto& Child : childrenAttach)
	{
		Child->SetComponentScale((Child->GetComponentScale() / Child->parentAttach->GetComponentScale()) * newScale);
	}
	Transform.SetScale(newScale);
}

void CoreEngine::Runtime::SceneComponent::AddComponentRotation(const FVector& addRotation)
{
	for (auto& Child : childrenAttach)
	{
		Child->AddComponentRotation(addRotation);
	}
	Transform.SetRotation(Transform.GetRotation() + addRotation);
}

void CoreEngine::Runtime::SceneComponent::AddComponentLocation(const FVector& addLocation)
{
	for (auto& Child : childrenAttach)
	{
		Child->AddComponentLocation(addLocation);
	}
	Transform.SetLocation(Transform.GetLocation() + addLocation);
}

void CoreEngine::Runtime::SceneComponent::AddComponentScale(const FVector& addScale)
{
	for (auto& Child : childrenAttach)
	{
		Child->AddComponentScale(addScale);
	}
	Transform.SetScale(Transform.GetScale() + addScale);
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
	const FVector& Rotation = Transform.GetRotation();
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
