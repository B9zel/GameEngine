#pragma once
#include <Runtime/includes/BaseLightComponent.h>
#include <PointLightComponent.generated.h>

namespace CoreEngine
{
	class LightProxy;
	class PointLightProxy;

} // namespace CoreEngine

RCLASS(EditorComponent)
class PoinLightComponent : public BaseLightComponent
{
	GENERATED_BODY()

public:

	PoinLightComponent(const CoreEngine::InitializeObject& Object);

public:

	virtual CoreEngine::LightProxy* GetLightProxy() override;

	void SetConstant(const float NewConstant);
	void SetLinear(const float NewLinear);
	void SetQuadratic(const float NewQuadratic);

	float GetConstant() const;
	float GetLinear() const;
	float GetQuadratic() const;

private:

	UniquePtr<CoreEngine::PointLightProxy> LightProxy;

	float m_Constant{1.0f};
	float m_Linear{0.1f};
	float m_Quadratic{0.034f};
};
