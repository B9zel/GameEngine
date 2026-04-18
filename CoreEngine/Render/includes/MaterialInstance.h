#pragma once
#include <Render/includes/MaterialInterface.h>
#include <MaterialInstance.generated.h>

RCLASS()
class MaterialInstance : public MaterialInterface
{
	GENERATED_BODY()

public:

	MaterialInstance(const CoreEngine::InitializeObject& Object);

	void SetParentMaterial(const CoreEngine::Render::MaterialHandle& ParentMaterial);
	const CoreEngine::Render::MaterialHandle& GetParentMaterial() const;

private:

	UniquePtr<CoreEngine::Render::BaseMaterialProperty> CreatePropertyFromType(const EUniformType& Type);

private:

	CoreEngine::Render::MaterialHandle m_ParentMaterial;

	DArray<UniquePtr<CoreEngine::Render::BaseMaterialProperty>> m_MaterialProperties;
};
