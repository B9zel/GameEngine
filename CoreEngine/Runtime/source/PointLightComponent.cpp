
#include <Runtime/includes/PointLightComponent.h>
#include <Runtime/includes/Actor.h>
#include <Core/includes/LightProxy.h>

PoinLightComponent::PoinLightComponent(const CoreEngine::InitializeObject& Object) : BaseLightComponent(Object)
{
	LightProxy = MakeUniquePtr<CoreEngine::PointLightProxy>();
}

CoreEngine::LightProxy* PoinLightComponent::GetLightProxy()
{
	FVector Location;
	Math::DecomposeLocationMatrix(MakeMatrixMesh(), Location);

	LightProxy->SetLocation(Location);
	LightProxy->SetColor(FVector(GetColor().R, GetColor().G, GetColor().B));
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
