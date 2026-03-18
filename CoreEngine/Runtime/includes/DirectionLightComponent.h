#pragma once
#include <Runtime/includes/BaseLightComponent.h>
#include <DirectionLightComponent.generated.h>


namespace CoreEngine
{
	class LightProxy;
	class DirectionLightProxy;

	namespace Runtime
	{
		RCLASS(EditorComponent)
		class DirectionLightComponent : public BaseLightComponent
		{
			GENERATED_BODY()

		public:

			DirectionLightComponent(const InitializeObject& Object);

		public:

			virtual LightProxy* GetLightProxy() override;

		private:

			UniquePtr<DirectionLightProxy> LightProxy;
		};
	}
}
