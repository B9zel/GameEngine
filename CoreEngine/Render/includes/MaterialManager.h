#pragma once
#include <Render/includes/MaterialInterface.h>
#include <Core/includes/Base.h>

class MaterialAsset;
class RMaterial;

namespace CoreEngine::Render
{

	class MaterialManager
	{
	protected:

		MaterialManager() = default;

	public:

		static MaterialManager& Get();

		RMaterial* GetMaterial(const MaterialHandle& Handle);
		MaterialHandle CreateAndRegisterMaterial(MaterialAsset* Asset);

	private:

		uint64 GetHashMaterialAsset(MaterialAsset* Asset) const;

	private:

		HashTableMap<uint64, MaterialHandle> m_MaterialHandles;
		HashTableMap<MaterialHandle, RMaterial*, RenderHandleHasher> m_Materials;

		uint64 ID = CoreEngine::Render::RenderHandle::StartId;
	};
} // namespace CoreEngine::Render
