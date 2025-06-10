#pragma once
#include <Runtime/includes/BaseLightComponent.h>


namespace CoreEngine
{

	class SpotLightProxy;
	namespace Runtime
	{

		class SpotLightComponent : public BaseLightComponent
		{
		public:

			SpotLightComponent();

		public:

			virtual LightProxy* GetLightProxy() override;

			void SetConstant(const float NewConstant);
			void SetLinear(const float NewLinear);
			void SetQuadratic(const float NewQuadratic);
			void SetOuterCutOff(const float NewOuterCutOff);
			void SetCutOff(float NewCutOff);

			float GetConstant() const;
			float GetLinear() const;
			float GetQuadratic() const;
			float GetOuterCutOff() const;
			float GetCutOff() const;

		private:

			UniquePtr<SpotLightProxy> LightProxy;

			float m_Constant{ 1.0f };
			float m_Linear{ 0.1f };
			float m_Quadratic{ 0.034f };
			float m_CutOff{ 12.0f };
			float m_OuterCutOff{ 17.5f };
		};
	}
}