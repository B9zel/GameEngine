#include <Runtime/includes/SpotLightComponent.h>

#include <Core/includes/LightProxy.h>


namespace CoreEngine
{
	namespace Runtime
	{

		SpotLightComponent::SpotLightComponent()
		{
			LightProxy = MakeUniquePtr<SpotLightProxy>();
		}
		LightProxy* SpotLightComponent::GetLightProxy()
		{
			LightProxy->SetLocation(GetComponentLocation());
			LightProxy->SetColor(GetColor());
			LightProxy->SetIntencity(GetIntencity());
			LightProxy->SetConstant(GetConstant());
			LightProxy->SetLinear(GetLinear());
			LightProxy->SetQuadratic(GetQuadratic());
			LightProxy->SetCutOff(cos(Math::ToRadian(GetCutOff())));
			LightProxy->SetOuterCutOff(cos(Math::ToRadian(GetOuterCutOff())));
			LightProxy->SetDirection(GetForwardVector());

			return LightProxy.get();
		}
		void SpotLightComponent::SetConstant(const float NewConstant)
		{
			m_Constant = NewConstant;
		}
		void SpotLightComponent::SetLinear(const float NewLinear)
		{
			m_Linear = NewLinear;
		}
		void SpotLightComponent::SetQuadratic(const float NewQuadratic)
		{
			m_Quadratic = NewQuadratic;
		}
		void SpotLightComponent::SetOuterCutOff(const float NewOuterCutOff)
		{
			m_OuterCutOff = Math::Max(NewOuterCutOff, 0.0f);
		}
		void SpotLightComponent::SetCutOff(float NewCutOff)
		{
			m_CutOff = Math::Max(NewCutOff, 0.0f);
		}
		float SpotLightComponent::GetConstant() const
		{
			return m_Constant;
		}
		float SpotLightComponent::GetLinear() const
		{
			return m_Linear;
		}
		float SpotLightComponent::GetQuadratic() const
		{
			return m_Quadratic;
		}
		float SpotLightComponent::GetOuterCutOff() const
		{
			return m_OuterCutOff;
		}
		float SpotLightComponent::GetCutOff() const
		{
			return m_CutOff;
		}
	}
}
