#include <Render/includes/MaterialManager.h>
#include <Render/includes/Material.h>
#include <Render/includes/MaterialAsset.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>

namespace CoreEngine::Render
{
	RMaterial* CoreEngine::Render::MaterialManager::GetMaterial(const MaterialHandle& Handle)
	{
		if (!Handle.IsValid())
		{
			return nullptr;
		}

		auto& material = m_Materials.find(Handle);
		if (material != m_Materials.end())
		{
			return material->second;
		}
		return nullptr;
	}

	MaterialHandle CoreEngine::Render::MaterialManager::CreateAndRegisterMaterial(MaterialAsset* Asset)
	{
		auto& handle = m_MaterialHandles.find(Asset);
		if (handle != m_MaterialHandles.end())
		{
			return handle->second;
		}

		MaterialHandle newHandle;
		newHandle.SetId(ID++);
		m_MaterialHandles.emplace(Asset, newHandle);

		auto* NewMaterial = CreateObject<RMaterial>();
		NewMaterial->SetShaderAsset(Asset->GetShaderAsset());
		NewMaterial->SetModeRender(Asset->GetModeRender());
		m_Materials.emplace(newHandle, NewMaterial);

		return newHandle;
	}

	MaterialManager& MaterialManager::Get()
	{
		static MaterialManager Instance;
		return Instance;
	}

} // namespace CoreEngine::Render
