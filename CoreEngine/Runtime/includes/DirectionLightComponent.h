#pragma once
#include <Runtime/includes/BaseLightComponent.h>



namespace CoreEngine
{
	class LightProxy;
	class DirectionLightProxy;

	namespace Runtime
	{
		class DirectionLightComponent : public BaseLightComponent
		{
		public:

			DirectionLightComponent(const InitializeObject& Object);

		public:

			virtual LightProxy* GetLightProxy() override;

		private:

			UniquePtr<DirectionLightProxy> LightProxy;
		};
	}
}
