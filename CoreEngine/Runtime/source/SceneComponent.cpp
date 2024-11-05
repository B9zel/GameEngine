#include <Runtime/includes/SceneComponent.h>




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

void CoreEngine::Runtime::SceneComponent::SetComponentRotation(const FVector& newRotation)
{
   transform.SetRotation(newRotation);
}

void CoreEngine::Runtime::SceneComponent::SetComponentLocation(const FVector& newLocation)
{
    transform.SetLocation(newLocation);
}

void CoreEngine::Runtime::SceneComponent::SetComponentScale(const FVector& newScale)
{
    transform.SetScale(newScale);
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
