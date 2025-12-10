#include <Runtime/includes/SpotLightComponent.h>

#include <Core/includes/LightProxy.h>


namespace CoreEngine
{
	namespace Runtime
	{

		SpotLightComponent::SpotLightComponent(const InitializeObject& Object) : BaseLightComponent(Object)
		{
			LightProxy = MakeUniquePtr<SpotLightProxy>();
		}
		LightProxy* SpotLightComponent::GetLightProxy()
		{
			FVector Location, Rotation, Scale;
			Math::DecomposeMatrix(MakeMatrixMesh(), Location, Rotation, Scale);
			LightProxy->SetLocation(Location);
			LightProxy->SetColor(GetColor());
			LightProxy->SetIntencity(GetIntencity());
			LightProxy->SetConstant(GetConstant());
			LightProxy->SetLinear(GetLinear());
			LightProxy->SetQuadratic(GetQuadratic());
			LightProxy->SetCutOff(cos(Math::ToRadian(GetCutOff())));
			LightProxy->SetOuterCutOff(cos(Math::ToRadian(GetOuterCutOff())));
			LightProxy->SetDirection(CalculateForwardDirection(Rotation, false));
			LightProxy->SetID(GetUUID().GetID());

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
			OuterCutOff = Math::Max(NewOuterCutOff, 0.0f);
		}
		void SpotLightComponent::SetCutOff(float NewCutOff)
		{
			CutOff = Math::Max(NewCutOff, 0.0f);
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
			return OuterCutOff;
		}
		float SpotLightComponent::GetCutOff() const
		{
			return CutOff;
		}
	}
}
