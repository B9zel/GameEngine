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
		auto& handle = m_MaterialHandles.find(GetHashMaterialAsset(Asset));
		if (handle != m_MaterialHandles.end())
		{
			return handle->second;
		}

		MaterialHandle newHandle;
		newHandle.SetId(ID++);
		m_MaterialHandles.emplace(GetHashMaterialAsset(Asset), newHandle);
		Engine::Get()->GetMemoryManager()->GetGarbageCollector()->AddRootObject(Asset);

		auto* NewMaterial = CreateObject<RMaterial>();
		NewMaterial->SetShaderAsset(Asset->GetShaderAsset());
		NewMaterial->SetModeRender(Asset->GetModeRender());
		Engine::Get()->GetMemoryManager()->GetGarbageCollector()->AddRootObject(NewMaterial);
		m_Materials.emplace(newHandle, NewMaterial);

		return newHandle;
	}

	uint64 MaterialManager::GetHashMaterialAsset(MaterialAsset* Asset) const
	{
		if (!Asset) return 0;
		uint64 Hash = 0;

		Hash ^= std::hash<String>{}(Asset->GetPathToShaderAsset()) + 0x9e3779b9 + (Hash << 6) + (Hash >> 2);
		Hash ^= std::hash<String>{}(Asset->GetPathToAsset()) + 0x9e3779b9 + (Hash << 6) + (Hash >> 2);
		Hash ^= std::hash<EShaderRenderType>{}(Asset->GetModeRender()) + 0x9e3779b9 + (Hash << 6) + (Hash >> 2);

		return Hash;
	}

	MaterialManager& MaterialManager::Get()
	{
		static MaterialManager Instance;
		return Instance;
	}

} // namespace CoreEngine::Render
