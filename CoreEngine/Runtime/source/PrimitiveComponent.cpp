#include <Runtime/includes/PrimitiveComponent.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Render/includes/MaterialInterface.h>
#include <Runtime/includes/Actor.h>
#include <Render/includes/MaterialManager.h>
#include <Render/includes/MaterialInstance.h>
#include <Render/includes/Material.h>

DECLARE_LOG_CATEGORY_EXTERN(PRIMITIVE_COMPONENT_Log);

PrimitiveComponent::PrimitiveComponent(const CoreEngine::InitializeObject& Object) : SceneComponent(Object)
{
	try
	{
		sceneProxy = new CoreEngine::PrimitiveProxy();
	}
	catch (const std::exception& error)
	{
		EG_LOG(PRIMITIVE_COMPONENT_Log, ELevelLog::ERROR, error.what());
		throw error;
	}
}
void PrimitiveComponent::SetMaterial(uint32 MaterialIndex, MaterialAsset* NewMaterial)
{
	if (!NewMaterial) return;

	auto& MaterialHandle = CoreEngine::Render::MaterialManager::Get().CreateAndRegisterMaterial(NewMaterial);

	RMaterial* ParentMaterial = CoreEngine::Render::MaterialManager::Get().GetMaterial(MaterialHandle);
	CoreEngine::Render::Render::PrepareMaterial(*ParentMaterial);

	const bool IsCreateNewMaterialInstance = MaterialIndex >= m_HandleMaterial.size();

	if (IsCreateNewMaterialInstance)
	{
		MaterialInstance* NewMaterialInstance = CreateObject<MaterialInstance>();
		NewMaterialInstance->SetParentMaterial(MaterialHandle);
		m_MaterialInstance.push_back(NewMaterialInstance);
		materials.push_back(NewMaterial);
	}
	else
	{
		m_MaterialInstance[MaterialIndex]->SetParentMaterial(MaterialHandle);
		materials[MaterialIndex] = NewMaterial;

	}

	/*auto* NewInstance = CreateObject<MaterialInstance>();
	NewInstance

		if ()
	{
		m_HandleMaterial.push_back(CoreEngine::Render::MaterialManager::Get().CreateAndRegisterMaterial(NewMaterial));
	}
	else
	{
		m_HandleMaterial[MaterialIndex] = CoreEngine::Render::MaterialManager::Get().CreateAndRegisterMaterial(NewMaterial);
	}*/
}
CoreEngine::PrimitiveProxy* PrimitiveComponent::GetSceneProxy() const
{
	// Actor* owner = GetOwner();
	/*if (sceneProxy)
	{
		sceneProxy->SetTransform(GetTransform());

	}*/
	sceneProxy->SetTransformMatrix(MakeMatrixMesh());

	return sceneProxy;
}

CoreEngine::PrimitiveProxy* PrimitiveComponent::GetUpdateProxy() const
{
	/*Transform ProxyTransform = GetTransform();
	ProxyTransform.SetRotation(Math::ToDegreesVector(ProxyTransform.GetRotation()));*/
	sceneProxy->SetTransformMatrix(MakeMatrixMesh());
	return sceneProxy;
}
