#pragma once
#include <Runtime/includes/SceneComponent.h>



namespace CoreEngine
{
	class LightProxy;

	namespace Runtime
	{
		class BaseLightComponent : public SceneComponent
		{
		public:

			virtual LightProxy* GetLightProxy() = 0;

			void SetColor(const FVector& NewColor);
			const FVector& GetColor() const;

			void SetIntencity(const float NewIntencity);
			const float GetIntencity() const;

		protected:

			bool IsVisible{ true };
			float Intencity{ 1.0f };
			FVector Color{ 1.0f,1.0f,1.0f };
		};
	}
}