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
		m_HandleMaterial.push_back(MaterialHandle);
		materials.push_back(NewMaterial);
	}
	else
	{
		m_MaterialInstance[MaterialIndex]->SetParentMaterial(MaterialHandle);
		materials[MaterialIndex] = NewMaterial;
		m_HandleMaterial[MaterialIndex] = MaterialHandle;
	}

	material = NewMaterial;
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
void PrimitiveComponent::PreEditChangeProperty(CoreEngine::Reflection::PropertyField& Property)
{
	if (Property.Name == STRINGCON_DETAILS(material))
	{
		material->PostChangeProperty.Remove(&PrimitiveComponent::OnChangeMaterialShaderAsset, this);
		material->PreChangeProperty.Remove(&PrimitiveComponent::PreOnChangeMaterialShaderAsset, this);
	}
}

void PrimitiveComponent::PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property)
{
	if (Property.Name == STRINGCON_DETAILS(material))
	{
		material->PostChangeProperty.AddBind(&PrimitiveComponent::OnChangeMaterialShaderAsset, this);
		material->PreChangeProperty.AddBind(&PrimitiveComponent::PreOnChangeMaterialShaderAsset, this);
		SetMaterial(0, material);
	}
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

void PrimitiveComponent::OnChangeMaterialShaderAsset(CoreEngine::Reflection::PropertyField& Field)
{
	auto& MaterialHandle = CoreEngine::Render::MaterialManager::Get().CreateAndRegisterMaterial(material);

	RMaterial* ParentMaterial = CoreEngine::Render::MaterialManager::Get().GetMaterial(MaterialHandle);
	ParentMaterial->SetShaderAsset(material->GetShaderAsset());
	CoreEngine::Render::Render::PrepareMaterial(*ParentMaterial);

	for (auto* Instace : m_MaterialInstance)
	{
		if (Instace->GetParentMaterial() == HandlePrevMaterial)
		{
			Instace->SetParentMaterial(MaterialHandle);
			break;
		}
	}

	// m_MaterialInstance[0]->SetParentMaterial(MaterialHandle);
	//  materials[0] = material;
	//  m_HandleMaterial[0] = MaterialHandle;
}

void PrimitiveComponent::PreOnChangeMaterialShaderAsset(CoreEngine::Reflection::PropertyField& Field)
{
	HandlePrevMaterial = CoreEngine::Render::MaterialManager::Get().CreateAndRegisterMaterial(material);
}
