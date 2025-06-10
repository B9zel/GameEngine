#pragma once
#include <Runtime/includes/BaseLightComponent.h>



namespace CoreEngine
{
	class LightProxy;
	class PointLightProxy;

	namespace Runtime
	{
		class PoinLightComponent : public BaseLightComponent
		{
		public:

			PoinLightComponent();

		public:

			virtual LightProxy* GetLightProxy() override;

			void SetConstant(const float NewConstant);
			void SetLinear(const float NewLinear);
			void SetQuadratic(const float NewQuadratic);

			float GetConstant() const;
			float GetLinear() const;
			float GetQuadratic() const;

		private:

			UniquePtr<PointLightProxy> LightProxy;

			float m_Constant{ 1.0f };
			float m_Linear{ 0.1f };
			float m_Quadratic{ 0.034f };
		};
	}
}
