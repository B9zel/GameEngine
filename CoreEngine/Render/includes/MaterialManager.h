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

		HashTableMap<MaterialAsset*, MaterialHandle> m_MaterialHandles;
		HashTableMap<MaterialHandle, RMaterial*> m_Materials;


		uint64 ID = CoreEngine::Render::RenderHandle::StartId;
	};
} // namespace CoreEngine::Render
