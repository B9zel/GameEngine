#pragma once
#include <Runtime/includes/SceneComponent.h>
#include <Render/includes/Types/Color.h>
#include <BaseLightComponent.generated.h>

namespace CoreEngine
{
	class LightProxy;
}

RCLASS();
class BaseLightComponent : public SceneComponent
{
	GENERATED_BODY()

public:

	BaseLightComponent(const CoreEngine::InitializeObject& Object);

	virtual CoreEngine::LightProxy* GetLightProxy();

	void SetColor(const LinearColor& NewColor);
	const LinearColor& GetColor() const;

	void SetIntencity(const float NewIntencity);
	const float GetIntencity() const;

	bool GetIsVisible() const;

protected:

	RPROPERTY(EditorVisible);
	bool IsVisible{true};
	RPROPERTY(EditorVisible);
	float Intencity{1.0f};
	RPROPERTY(EditorVisible);
	LinearColor Color{1.0f, 1.0f, 1.0f};
};
