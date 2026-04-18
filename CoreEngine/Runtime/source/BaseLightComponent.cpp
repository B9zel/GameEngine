#include <Runtime/includes/BaseLightComponent.h>

BaseLightComponent::BaseLightComponent(const CoreEngine::InitializeObject& Object) : SceneComponent(Object)
{
}

CoreEngine::LightProxy* BaseLightComponent::GetLightProxy()
{
	return nullptr;
}

void BaseLightComponent::SetColor(const LinearColor& NewColor)
{
	Color = NewColor;
}
const LinearColor& BaseLightComponent::GetColor() const
{
	return Color;
}
void BaseLightComponent::SetIntencity(const float NewIntencity)
{
	Intencity = NewIntencity < 0.0f ? 0.0f : NewIntencity;
}
const float BaseLightComponent::GetIntencity() const
{
	return Intencity;
}
bool BaseLightComponent::GetIsVisible() const
{
	return IsVisible;
}
