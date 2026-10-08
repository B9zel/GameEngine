#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/MaterialInterface.h>
#include <MaterialInstance.generated.h>

namespace CoreEngine::Render
{
	class BaseMaterialProperty;
}

RCLASS()
class MaterialInstance : public MaterialInterface
{
	GENERATED_BODY()

public:

	MaterialInstance(const CoreEngine::InitializeObject& Object);

	void SetParentMaterial(const CoreEngine::Render::MaterialHandle& ParentMaterial);
	void UpdateUniform(const CoreEngine::Render::MaterialHandle& Handle);
	const CoreEngine::Render::MaterialHandle& GetParentMaterial() const;
	const CoreEngine::Render::MaterialHandle& GetPotentialParentMaterial() const;

	const DArray<SharedPtr<CoreEngine::Render::BaseMaterialProperty>>* GetMaterialProperties();

private:

	CoreEngine::Render::MaterialHandle m_ParentMaterial;
	CoreEngine::Render::MaterialHandle m_PotentialParentMaterial;

	DArray<SharedPtr<CoreEngine::Render::BaseMaterialProperty>> m_MaterialProperties;
};
