#pragma once
#include <Runtime/includes/SceneComponent.h>
#include <Render/includes/Material.h>
#include <Render/includes/MaterialAsset.h>
#include <Render/includes/MaterialInstance.h>
#include <Core/includes/MeshAsset.h>
#include <PrimitiveComponent.generated.h>

class MaterialAsset;
class MaterialInstance;
class MeshAsset;

namespace CoreEngine::Render
{
	struct MaterialHandle;
}

namespace CoreEngine
{
	class PrimitiveProxy;

} // namespace CoreEngine
RCLASS()
class PrimitiveComponent : public SceneComponent
{
	GENERATED_BODY()

public:

	PrimitiveComponent(const CoreEngine::InitializeObject& Object);
	// Test
	virtual void SetMaterial(uint32 MaterialIndex, MaterialAsset* NewMaterial);
	virtual void SetMeshAsset(MeshAsset* NewAsset);

	virtual void PreEditChangeProperty(CoreEngine::Reflection::PropertyField& Property) override;
	virtual void PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property) override;

public:

	virtual CoreEngine::PrimitiveProxy* GetSceneProxy() const;
	virtual CoreEngine::PrimitiveProxy* GetUpdateProxy() const;

	// Test
	CoreEngine::PrimitiveProxy* sceneProxy;

private:

	void OnChangeMaterialShaderAsset(Asset* asset, CoreEngine::Reflection::PropertyField& Field);
	void PreOnChangeMaterialShaderAsset(Asset* asset, CoreEngine::Reflection::PropertyField& Field);

protected:

	RPROPERTY();
	DArray<MaterialAsset*> materials;

	RPROPERTY(EditorVisible);
	MeshAsset* AssetModel = nullptr;

	RPROPERTY(EditorVisible);
	MaterialAsset* material = nullptr;

	DArray<CoreEngine::Render::MaterialHandle> m_HandleMaterial;
	RPROPERTY();
	DArray<MaterialInstance*> m_MaterialInstance;

private:

	CoreEngine::Render::MaterialHandle HandlePrevMaterial;
};
