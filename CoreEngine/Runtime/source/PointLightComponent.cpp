
#include <Runtime/includes/PointLightComponent.h>

#include <Core/includes/LightProxy.h>


namespace CoreEngine
{
	namespace Runtime
	{

		PoinLightComponent::PoinLightComponent(const InitializeObject& Object) : BaseLightComponent(Object)
		{
			LightProxy = MakeUniquePtr<PointLightProxy>();
		}

		LightProxy* PoinLightComponent::GetLightProxy()
		{
			LightProxy->SetLocation(GetComponentLocation());
			LightProxy->SetColor(GetColor());
			LightProxy->SetIntencity(GetIntencity());
			LightProxy->SetConstant(GetConstant());
			LightProxy->SetLinear(GetLinear());
			LightProxy->SetQuadratic(GetQuadratic());

			return LightProxy.get();
		}
		void PoinLightComponent::SetConstant(const float NewConstant)
		{
			m_Constant = NewConstant;
		}
		void PoinLightComponent::SetLinear(const float NewLinear)
		{
			m_Linear = NewLinear;
		}
		void PoinLightComponent::SetQuadratic(const float NewQuadratic)
		{
			m_Quadratic = NewQuadratic;
		}
		float PoinLightComponent::GetConstant() const
		{
			return m_Constant;
		}
		float PoinLightComponent::GetLinear() const
		{
			return m_Linear;
		}
		float PoinLightComponent::GetQuadratic() const
		{
			return m_Quadratic;
		}
	}
}

