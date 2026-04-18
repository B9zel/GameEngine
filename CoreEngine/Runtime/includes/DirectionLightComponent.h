#pragma once
#include <Runtime/includes/BaseLightComponent.h>
#include <DirectionLightComponent.generated.h>

namespace CoreEngine
{
	class LightProxy;
	class DirectionLightProxy;
} // namespace CoreEngine

RCLASS(EditorComponent)
class DirectionLightComponent : public BaseLightComponent
{
	GENERATED_BODY()

public:

	DirectionLightComponent(const CoreEngine::InitializeObject& Object);

public:

	virtual CoreEngine::LightProxy* GetLightProxy() override;

private:

	UniquePtr<CoreEngine::DirectionLightProxy> LightProxy;
};
