#pragma once

#include "Runtime/CoreObject/Include/Object.h"
#include <Core/includes/Base.h>
#include <Render/includes/ShaderUtils.h>
#include <AssetManager.generated.h>

namespace CoreEngine
{
	namespace Render
	{
		class Texture2D;
		class Shader;
	} // namespace Render

} // namespace CoreEngine
RCLASS();
class AssetManager : public Object
{
	GENERATED_BODY()

public:

	AssetManager(const CoreEngine::InitializeObject& Initilize);
	virtual ~AssetManager();

public:

	CoreEngine::Render::Texture2D* LoadTexture(const String& Path);
	CoreEngine::Render::Shader* LoadShader(const String& VertexSh, const String& FragShad);
	CoreEngine::Render::Shader* LoadShader(const String& Path);

	SourceShader LoadStringShaderFromFile(const String& Path);
	static AssetManager& Get();

private:

	void ClearAllAssets();


private:

	HashTableSet<UniquePtr<CoreEngine::Render::Texture2D>> Textures;
	HashTableMap<String, UniquePtr<CoreEngine::Render::Shader>> Shaders;
};
