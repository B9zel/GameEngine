#include <Runtime/includes/BaseLightComponent.h>


namespace CoreEngine
{
	namespace Runtime
	{
		BaseLightComponent::BaseLightComponent(const InitializeObject& Object) : SceneComponent(Object)
		{
		}

		LightProxy* BaseLightComponent::GetLightProxy()
		{
			return nullptr;
		}

		void BaseLightComponent::SetColor(const FVector& NewColor)
		{
			Color = NewColor;
		}
		const FVector& BaseLightComponent::GetColor() const
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
	}
}