#include <Runtime/includes/BaseLightComponent.h>


namespace CoreEngine
{
	namespace Runtime
	{
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
	}
}