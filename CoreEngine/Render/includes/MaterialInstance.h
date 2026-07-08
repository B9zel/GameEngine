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
	const CoreEngine::Render::MaterialHandle& GetPotentialParentMaterial() const;

private:

	CoreEngine::Render::MaterialHandle m_ParentMaterial;
	CoreEngine::Render::MaterialHandle m_PotentialParentMaterial;

	DArray<UniquePtr<CoreEngine::Render::BaseMaterialProperty>> m_MaterialProperties;
};
