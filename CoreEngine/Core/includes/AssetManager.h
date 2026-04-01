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

	// class Runtime::Object;
	//  class Render::Model;

	RCLASS();
	class AssetManager : public CoreEngine::Runtime::Object
	{
		GENERATED_BODY()

	public:

		AssetManager(const InitializeObject& Initilize);
		virtual ~AssetManager();

	public:

		Render::Texture2D* LoadTexture(const String& Path);
		Render::Shader* LoadShader(const String& VertexSh, const String& FragShad);
		Render::Shader* LoadShader(const String& Path);

		static AssetManager& Get();

	private:

		void ClearAllAssets();

		SourceShader LoadShaderFromFile(const String& Path);

	private:

		HashTableSet<UniquePtr<Render::Texture2D>> Textures;
		HashTableMap<String, UniquePtr<Render::Shader>> Shaders;
	};
} // namespace CoreEngine
